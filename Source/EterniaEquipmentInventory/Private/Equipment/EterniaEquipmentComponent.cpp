// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/EterniaEquipmentComponent.h"

#include "Equipment/EquipmentSlot.h"
#include "Helpers/ETLogging.h"
#include "Inventory/EterniaInventoryEntry.h"

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

UEquipmentSlot* UEterniaEquipmentComponent::FindSlotByType(const FETEquipmentSlot& SlotType) const {
	for (auto EquipmentSlot : Slots) {
		if (EquipmentSlot->GetType() == SlotType) {
			return EquipmentSlot;
		}
	}
	return nullptr;
}

TArray<UEquipmentSlot*> UEterniaEquipmentComponent::FindAllValidSlotsForItemType(const FETItemType& ItemType) const {
	TArray<UEquipmentSlot*> ResultArray;
	for (auto EquipmentSlot : Slots) {
		if (EquipmentSlot->IsValidForItemType(ItemType)) {
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

bool UEterniaEquipmentComponent::TryEquipItem(UEterniaInventoryEntry* NewItem, UEquipmentSlot* Slot, bool bForceEquip,
                                              UEterniaInventoryEntry*& RemainingItem) {
	if (!NewItem || !Slot) return false;

	if (!Slots.Contains(Slot)) {
		EEIS_ULOGO_ERROR(TEXT("Tried to equip item to not owned slot"));
		return false;
	}

	return Slot->SetItem(NewItem, bForceEquip, RemainingItem);
}

bool UEterniaEquipmentComponent::TryEquipItem(UEterniaInventoryEntry* InventoryEntry, bool bForceEquip, UEterniaInventoryEntry*& RemainingItem) {
	if (InventoryEntry && InventoryEntry->GetDefinition()) {
		TArray<UEquipmentSlot*> FoundSlots = FindAllValidSlotsForItemType(InventoryEntry->GetDefinition()->GetItemType());

		// Try empty slots first regardless of bForceEquip ...
		for (UEquipmentSlot* Slot : FoundSlots) {
			if (Slot->IsEmpty()) {
				if (TryEquipItem(InventoryEntry, Slot, bForceEquip, RemainingItem)) {
					return true;
				}
			}
		}

		// ... then try occupied slots
		for (UEquipmentSlot* Slot : FoundSlots) {
			if (!Slot->IsEmpty()) {
				if (TryEquipItem(InventoryEntry, Slot, bForceEquip, RemainingItem)) {
					return true;
				}
			}
		}
	}
	return false;
}
