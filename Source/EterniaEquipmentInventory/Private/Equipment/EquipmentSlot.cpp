// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/EquipmentSlot.h"

#include "Equipment/EquipmentBlueprintLibrary.h"
#include "Inventory/EterniaInventoryEntry.h"

bool UEquipmentSlot::IsValidForType(EEquipType EquipType) const {
	return UEquipmentBlueprintLibrary::GetSlotType(EquipType) == GetSlotType();
}

UEquipmentSlot::UEquipmentSlot() {
}

void UEquipmentSlot::Init(EEquipmentSlotType Type) {
	SlotType = Type;
	Clear();
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
	if (Item && Item != InventoryEntry && Item->GetDefinition() && Item->GetDefinition()->IsEquippable()) {
		EEquipType EquipType = Item->GetDefinition()->GetEquipType();
		if (IsValidForType(EquipType)) {
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
