// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/EterniaInventoryEntry.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "Combination/ETCombinationBlueprintLibrary.h"
#include "Inventory/ETInventoryComponent.h"
#include "Data/EterniaInventoryItemDefinition.h"
#include "Inventory/ETInventoryStatics.h"


UEterniaInventoryEntry::UEterniaInventoryEntry(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Amount(0)
	, Rotated(false) {
}

void UEterniaInventoryEntry::IncrementAmount(int32 AmountToAdd) {
	Amount += AmountToAdd;
}

void UEterniaInventoryEntry::SetDefinition(UEterniaInventoryItemDefinition* Def) {
	Definition = Def;
}

void UEterniaInventoryEntry::SetAmount(int32 Am) {
	Amount = IsStackable() ? FMath::Clamp(Am, 0, Definition->GetStackSize()) : 1;
	OnItemAmountChanged.Broadcast(this, Amount);
}

FIntPoint UEterniaInventoryEntry::GetDimensions() {
	FIntPoint Dimensions = Definition->GetDimensions();
	return Rotated ? FIntPoint(Dimensions.Y, Dimensions.X) : Dimensions;
}

UMaterialInterface* UEterniaInventoryEntry::GetImage() {
	return Rotated ? Definition->GetImageRotated() : Definition->GetImage();
}

void UEterniaInventoryEntry::Rotate() {
	Rotated = !Rotated;
	OnItemRotated.Broadcast(this);
}

void UEterniaInventoryEntry::SetOwningInventoryComponent(UETInventoryComponent* InInventoryComponent) {
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

bool UEterniaInventoryEntry::IsSameItem(UEterniaInventoryEntry* Item) const {
	if (!Item || !Item->GetDefinition() || !Definition) {
		return false;
	}
	return Item->GetDefinition()->GetItemID().IsEqual(Definition->GetItemID());
}

bool UEterniaInventoryEntry::IsStackFull() const {
	if (!Definition) {
		return true;
	}
	if (!Definition->IsStackable()) {
		return true;
	}
	return GetAmount() >= Definition->GetStackSize();
}

int32 UEterniaInventoryEntry::GetStackLimit() const {
	if (!Definition) {
		return 0;
	}
	if (!Definition->IsStackable()) {
		return 0;
	}
	return FMath::Max(0, Definition->GetStackSize() - GetAmount());
}

void UEterniaInventoryEntry::Activate(AActor* ActivatorActor) {
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
		UEterniaInventoryItemDefinition* ReplaceItemDef = UETInventoryStatics::FindItemDefinitionByID(this, ConsumeReplaceItemID);
		UEterniaInventoryEntry* ItemToAdd = UETInventoryStatics::CreateItemByDefinition(ReplaceItemDef, OwningInventoryComponent);
		OwningInventoryComponent->TryAddItem(ItemToAdd);
	}
}

bool UEterniaInventoryEntry::TryCombineWith(UEterniaInventoryEntry* ItemToCombineWith) {
	FCombinationResult CombinationResult;
	if (UETCombinationBlueprintLibrary::CanCombineItems(this, this, ItemToCombineWith, CombinationResult)) {
		SetAmount(GetAmount() - 1);
		ItemToCombineWith->SetAmount(ItemToCombineWith->GetAmount() - 1);

		// TODO Extract item creation code?
		UEterniaInventoryItemDefinition* ResultItemDef = UETInventoryStatics::FindItemDefinitionByID(this, CombinationResult.ResultId);
		UEterniaInventoryEntry* ResultItem = UETInventoryStatics::CreateItemByDefinition(ResultItemDef, ItemToCombineWith->GetOwningInventoryComponent());
		bool bTryAddItem = ItemToCombineWith->GetOwningInventoryComponent()->TryAddItem(ResultItem);

		if (CombinationResult.RemainderId != NAME_None) {
			UEterniaInventoryItemDefinition* RemainderItemDef = UETInventoryStatics::FindItemDefinitionByID(this, CombinationResult.RemainderId);
			UEterniaInventoryEntry* RemainderItem = UETInventoryStatics::CreateItemByDefinition(RemainderItemDef, ItemToCombineWith->GetOwningInventoryComponent());
			bTryAddItem = bTryAddItem && ItemToCombineWith->GetOwningInventoryComponent()->TryAddItem(RemainderItem);
		}

		return bTryAddItem;
	}
	return false;
}

UAbilitySystemComponent* UEterniaInventoryEntry::FindAbilitySystemComponent(UETInventoryComponent* InventoryComponent) {
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
