// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/EterniaEquipmentComponent.h"

#include "Equipment/EquipmentBlueprintLibrary.h"
#include "Equipment/EquipmentSlot.h"
#include "Inventory/EterniaInventoryEntry.h"
#include "Inventory/InventoryInterface.h"

UEterniaEquipmentComponent::UEterniaEquipmentComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}

UEquipmentSlot* UEterniaEquipmentComponent::FindSlotByName(const FName& Name) const {
	for (auto EquipmentSlot : Slots) {
		if (EquipmentSlot->GetSlotName() == Name) {
			return EquipmentSlot;
		}
	}
	return nullptr;
}

UEquipmentSlot* UEterniaEquipmentComponent::FindSlotByType(EEquipmentSlotType SlotType) const {
	for (auto EquipmentSlot : Slots) {
		if (EquipmentSlot->GetSlotType() == SlotType) {
			return EquipmentSlot;
		}
	}
	return nullptr;
}

TArray<UEquipmentSlot*> UEterniaEquipmentComponent::FindAllSlotsByType(EEquipmentSlotType SlotType) const {
	TArray<UEquipmentSlot*> ResultArray;
	for (auto EquipmentSlot : Slots) {
		if (EquipmentSlot->GetSlotType() == SlotType) {
			ResultArray.Add(EquipmentSlot);
		}
	}
	return ResultArray;
}

UEquipmentSlot* UEterniaEquipmentComponent::FindSlotByInputAction(const UInputAction* InputAction) const {
	for (auto EquipmentSlot : Slots) {
		if (EquipmentSlot->GetInputAction() == InputAction) {
			return EquipmentSlot;
		}
	}
	return nullptr;
}

void UEterniaEquipmentComponent::ClearSlot(UEquipmentSlot* EquipmentSlot) {
	if (EquipmentSlot) {
		EquipmentSlot->Clear();
	}
}

void UEterniaEquipmentComponent::NotifySlotUpdated(UEquipmentSlot* Slot, UEterniaInventoryEntry* Item) {
	OnSlotUpdated.Broadcast(Slot, Item);
}

void UEterniaEquipmentComponent::BeginPlay() {
	Super::BeginPlay();

	for (auto Slot : Slots) {
		Slot->OnEquippedItemChanged.AddDynamic(this, &UEterniaEquipmentComponent::NotifySlotUpdated);
	}
}

bool UEterniaEquipmentComponent::ActivateSlotByInputAction(const UInputAction* InputAction) {
	UEquipmentSlot* EquipmentSlot = FindSlotByInputAction(InputAction);
	return EquipmentSlot ? EquipmentSlot->ActivateItem(GetOwner()) : false;
}

bool UEterniaEquipmentComponent::AreRequirementsMet(UEterniaInventoryItemDefinition* InventoryWeapon) const {
	return true;
}

bool UEterniaEquipmentComponent::TryEquipItem(UEterniaInventoryEntry* NewItem, UEquipmentSlot* Slot, bool ForceEquip) {
	if (!NewItem || !Slot) return false;

	UEterniaInventoryEntry* CurrentItemInSlot = Slot->GetInventoryEntry();
	if (Slot->IsEmpty() || !CurrentItemInSlot->IsSameItem(NewItem) && ForceEquip) {
		if (AreRequirementsMet(NewItem->GetDefinition())) {
			bool bSuccess = Slot->SetItem(NewItem);
			if (bSuccess && GetOwner()->Implements<UInventoryInterface>()) {
				UETInventoryComponent* IC = IInventoryInterface::Execute_GetInventoryComponent(GetOwner());
				NewItem->SetOwningInventoryComponent(IC);
			}
			return bSuccess;
		}
	} else {
		if (CurrentItemInSlot && CurrentItemInSlot->IsStackable() && CurrentItemInSlot->IsSameItem(NewItem)) {
			int32 CurrentStackLimit = CurrentItemInSlot->GetStackLimit();
			if (NewItem->GetAmount() <= CurrentStackLimit) {
				CurrentItemInSlot->SetAmount(CurrentItemInSlot->GetAmount() + NewItem->GetAmount());
				return true;
			}
			CurrentItemInSlot->SetAmount(CurrentItemInSlot->GetAmount() + CurrentStackLimit);
			NewItem->SetAmount(NewItem->GetAmount() - CurrentStackLimit);
		}
	}
	return false;
}

bool UEterniaEquipmentComponent::TryEquipItem(UEterniaInventoryEntry* InventoryEntry, bool ForceEquip) {
	bool bResult = false;
	if (InventoryEntry && InventoryEntry->GetDefinition()) {
		TArray<UEquipmentSlot*> FoundSlots = FindAllSlotsByType(UEquipmentBlueprintLibrary::GetSlotType(InventoryEntry->GetDefinition()->GetEquipType()));
		for (UEquipmentSlot* Slot : FoundSlots) {
			bResult = TryEquipItem(InventoryEntry, Slot, ForceEquip);
			if (bResult) {
				break;
			}
		}
	}
	return bResult;
}
