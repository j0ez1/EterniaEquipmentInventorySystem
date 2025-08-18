// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ETItem.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "Combination/ETCombinationBlueprintLibrary.h"
#include "Inventory/ETTileInventoryComponent.h"
#include "Inventory/ETInventoryStatics.h"


UETItem::UETItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Amount(0)
	, Rotated(false) {
}

void UETItem::IncrementAmount(int32 AmountToAdd) {
	Amount += AmountToAdd;
}

void UETItem::SetDefinition(UETDAItemDefinition* Def) {
	Definition = Def;
}

void UETItem::SetAmount(int32 Am) {
	Amount = IsStackable() ? FMath::Clamp(Am, 0, Definition->GetStackSize()) : 1;
	OnItemAmountChanged.Broadcast(this, Amount);
}

FIntPoint UETItem::GetDimensions() {
	FIntPoint Dimensions = Definition->GetDimensions();
	return Rotated ? FIntPoint(Dimensions.Y, Dimensions.X) : Dimensions;
}

UMaterialInterface* UETItem::GetImage() {
	return Rotated ? Definition->GetImageRotated() : Definition->GetImage();
}

void UETItem::Rotate() {
	Rotated = !Rotated;
	OnItemRotated.Broadcast(this);
}

void UETItem::SetOwningInventoryComponent(UETInventoryComponentBase* InInventoryComponent) {
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

bool UETItem::IsSameItem(UETItem* Item) const {
	if (!Item || !Item->Definition || !Definition) {
		return false;
	}
	return Item->Definition->GetItemID().IsEqual(Definition->GetItemID());
}

bool UETItem::IsStackFull() const {
	if (!Definition) {
		return true;
	}
	if (!Definition->IsStackable()) {
		return true;
	}
	return GetAmount() >= Definition->GetStackSize();
}

int32 UETItem::GetStackLimit() const {
	if (!Definition) {
		return 0;
	}
	if (!Definition->IsStackable()) {
		return 0;
	}
	return FMath::Max(0, Definition->GetStackSize() - GetAmount());
}

void UETItem::Activate(AActor* ActivatorActor) {
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
		UETDAItemDefinition* ReplaceItemDef = UETInventoryStatics::FindItemDefinitionByID(ConsumeReplaceItemID);
		UETItem* ItemToAdd = UETInventoryStatics::CreateItemByDefinition(ReplaceItemDef, OwningInventoryComponent);
		OwningInventoryComponent->TryAddItem(ItemToAdd);
	}
}

UAbilitySystemComponent* UETItem::FindAbilitySystemComponent(UETInventoryComponentBase* InventoryComponent) {
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
