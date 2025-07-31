// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ETInventoryEntry.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "Combination/ETCombinationBlueprintLibrary.h"
#include "Inventory/ETTileInventoryComponent.h"
#include "Data/ETInventoryItemDefinition.h"
#include "Inventory/ETInventoryStatics.h"


UETInventoryEntry::UETInventoryEntry(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Amount(0)
	, Rotated(false) {
}

void UETInventoryEntry::IncrementAmount(int32 AmountToAdd) {
	Amount += AmountToAdd;
}

void UETInventoryEntry::SetDefinition(UETInventoryItemDefinition* Def) {
	Definition = Def;
}

void UETInventoryEntry::SetAmount(int32 Am) {
	Amount = IsStackable() ? FMath::Clamp(Am, 0, Definition->GetStackSize()) : 1;
	OnItemAmountChanged.Broadcast(this, Amount);
}

FIntPoint UETInventoryEntry::GetDimensions() {
	FIntPoint Dimensions = Definition->GetDimensions();
	return Rotated ? FIntPoint(Dimensions.Y, Dimensions.X) : Dimensions;
}

UMaterialInterface* UETInventoryEntry::GetImage() {
	return Rotated ? Definition->GetImageRotated() : Definition->GetImage();
}

void UETInventoryEntry::Rotate() {
	Rotated = !Rotated;
	OnItemRotated.Broadcast(this);
}

void UETInventoryEntry::SetOwningInventoryComponent(UETInventoryComponentBase* InInventoryComponent) {
	if (OwningInventoryComponent == InInventoryComponent) return;

	UAbilitySystemComponent* ASC = FindAbilitySystemComponent(OwningInventoryComponent);
	if (ASC) {
		ASC->RemoveActiveGameplayEffect(InInventoryEffectHandle);
	}

	OwningInventoryComponent = InInventoryComponent;

	ASC = FindAbilitySystemComponent(OwningInventoryComponent);
	if (ASC) {
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		AActor* Owner = OwningInventoryComponent->GetOwner();
		EffectContext.AddInstigator(Owner, Owner);
		UClass* EffectClass = Definition->GetInInventoryEffect().LoadSynchronous();
		if (EffectClass) {
			FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectClass, 1.f, EffectContext);
			InInventoryEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

bool UETInventoryEntry::IsSameItem(UETInventoryEntry* Item) const {
	if (!Item || !Item->GetDefinition() || !Definition) {
		return false;
	}
	return Item->GetDefinition()->GetItemID().IsEqual(Definition->GetItemID());
}

bool UETInventoryEntry::IsStackFull() const {
	if (!Definition) {
		return true;
	}
	if (!Definition->IsStackable()) {
		return true;
	}
	return GetAmount() >= Definition->GetStackSize();
}

int32 UETInventoryEntry::GetStackLimit() const {
	if (!Definition) {
		return 0;
	}
	if (!Definition->IsStackable()) {
		return 0;
	}
	return FMath::Max(0, Definition->GetStackSize() - GetAmount());
}

void UETInventoryEntry::Activate(AActor* ActivatorActor) {
	if (OwningInventoryComponent && Definition && Definition->IsConsumable()) {
		TSubclassOf<UGameplayEffect> EffectClass = Definition->GetConsumeEffect();
		if (ActivatorActor && ActivatorActor->Implements<UAbilitySystemInterface>() && EffectClass) {
			UAbilitySystemComponent* ASC = Cast<IAbilitySystemInterface>(ActivatorActor)->GetAbilitySystemComponent();
			FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
			FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectClass, 1.f, EffectContext);
			ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
		SetAmount(Amount - 1);
		FName ConsumeReplaceItemID = Definition->GetConsumeReplaceItemID();
		UETInventoryItemDefinition* ReplaceItemDef = UETInventoryStatics::FindItemDefinitionByID(this, ConsumeReplaceItemID);
		UETInventoryEntry* ItemToAdd = UETInventoryStatics::CreateItemByDefinition(ReplaceItemDef, OwningInventoryComponent);
		OwningInventoryComponent->TryAddItem(ItemToAdd);
	}
}

bool UETInventoryEntry::TryCombineWith(UETInventoryEntry* ItemToCombineWith) {
	FCombinationResult CombinationResult;
	if (UETCombinationBlueprintLibrary::CanCombineItems(this, this, ItemToCombineWith, CombinationResult)) {
		SetAmount(GetAmount() - 1);
		ItemToCombineWith->SetAmount(ItemToCombineWith->GetAmount() - 1);

		// TODO Extract item creation code?
		UETInventoryItemDefinition* ResultItemDef = UETInventoryStatics::FindItemDefinitionByID(this, CombinationResult.ResultId);
		UETInventoryEntry* ResultItem = UETInventoryStatics::CreateItemByDefinition(ResultItemDef, ItemToCombineWith->GetOwningInventoryComponent());
		bool bTryAddItem = ItemToCombineWith->GetOwningInventoryComponent()->TryAddItem(ResultItem);

		if (CombinationResult.RemainderId != NAME_None) {
			UETInventoryItemDefinition* RemainderItemDef = UETInventoryStatics::FindItemDefinitionByID(this, CombinationResult.RemainderId);
			UETInventoryEntry* RemainderItem = UETInventoryStatics::CreateItemByDefinition(RemainderItemDef, ItemToCombineWith->GetOwningInventoryComponent());
			bTryAddItem = bTryAddItem && ItemToCombineWith->GetOwningInventoryComponent()->TryAddItem(RemainderItem);
		}

		return bTryAddItem;
	}
	return false;
}

UAbilitySystemComponent* UETInventoryEntry::FindAbilitySystemComponent(UETInventoryComponentBase* InventoryComponent) {
	if (InventoryComponent) {
		AActor* Owner = InventoryComponent->GetOwner();
		if (Owner && Owner->Implements<UAbilitySystemInterface>()) {
			if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Owner)) {
				return ASI->GetAbilitySystemComponent();
			}
		}
	}
	return nullptr;
}
