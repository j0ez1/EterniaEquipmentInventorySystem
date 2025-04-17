// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/EquipmentSlot.h"

#include "Inventory/EterniaInventoryEntry.h"

UEquipmentSlot::UEquipmentSlot() {
}

void UEquipmentSlot::HandleItemAmountChanged(UEterniaInventoryEntry* UpdatedItem, int32 NewAmount) {
	if (InventoryEntry == UpdatedItem) {
		if (NewAmount <= 0) {
			Clear();
		} else {
			OnEquippedItemChanged.Broadcast(this, InventoryEntry);
		}
	}
}

bool UEquipmentSlot::SetItem(UEterniaInventoryEntry* Item) {
	if (Item && Item != InventoryEntry && Item->GetDefinition()) {
		if (IsValidForItemType(Item->GetDefinition()->GetItemType())) {
			if (InventoryEntry) {
				InventoryEntry->OnItemAmountChanged.RemoveDynamic(this, &UEquipmentSlot::HandleItemAmountChanged);
			}
			InventoryEntry = Item;
			InventoryEntry->OnItemAmountChanged.AddDynamic(this, &UEquipmentSlot::HandleItemAmountChanged);
			OnEquippedItemChanged.Broadcast(this, Item);
			return true;
		}
	}
	return false;
}

bool UEquipmentSlot::IsValidForItemType(const FETItemType& ItemType) const {
	FETEquipmentSlot Type = GetType();
	for (FETEquipmentSlot ValidType : ItemType.GetValidEquipmentSlotTypes()) {
		if (ValidType == Type) {
			return true;
		}
	}
	return false;
}

bool UEquipmentSlot::IsEmpty() {
	return InventoryEntry == nullptr;
}

void UEquipmentSlot::Clear() {
	if (InventoryEntry) {
		InventoryEntry->OnItemAmountChanged.RemoveDynamic(this, &UEquipmentSlot::HandleItemAmountChanged);
	}
	InventoryEntry = nullptr;
	OnEquippedItemChanged.Broadcast(this, InventoryEntry);
}

bool UEquipmentSlot::ActivateItem(AActor* ActivatorActor) {
	if (bIsActivatable && InventoryEntry) {
		InventoryEntry->Activate(ActivatorActor);
		return true;
	}
	return false;
}

FETEquipmentSlot UEquipmentSlot::GetType() const {
	FETEquipmentSlot* Row = SlotTypeRowHandle.GetRow<FETEquipmentSlot>("");
	if (Row) {
		return *Row;
	}
	return FETEquipmentSlot();
}
