// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/EterniaInventoryEntry.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "Inventory/ETInventoryComponent.h"
#include "Data/EterniaInventoryItemDefinition.h"
#include "Inventory/ETInventoryStatics.h"


void UEterniaInventoryEntry::IncrementAmount(int32 AmountToAdd) {
	Amount += AmountToAdd;
}

void UEterniaInventoryEntry::SetDefinition(UEterniaInventoryItemDefinition* Def) {
	Definition = Def;
}

void UEterniaInventoryEntry::SetAmount(int32 Am) {
	Amount = FMath::Max(0, Am);
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
