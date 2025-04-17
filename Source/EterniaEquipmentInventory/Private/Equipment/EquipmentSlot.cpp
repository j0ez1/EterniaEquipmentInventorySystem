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
			OnEquippedItemChanged.Broadcast(this, nullptr);
		}
	}
}

bool UEquipmentSlot::TryEquipItem(UEterniaInventoryEntry* NewItem, bool bForceEquip, UEterniaInventoryEntry*& RemainingItem) {
	if (bIsBlocked) {
		RemainingItem = NewItem;
		return false;
	}

	UEterniaInventoryEntry* OccupyingItem = InventoryEntry;
	if (IsEmpty() || !InventoryEntry->IsSameItem(NewItem) && bForceEquip) {
		bool bSuccess = DoSetItem(NewItem);
		RemainingItem = bSuccess ? OccupyingItem : NewItem;
		return bSuccess;
	}

	bool bIsCurrItemSameAndStackable = InventoryEntry && InventoryEntry->IsSameItem(NewItem) && InventoryEntry->IsStackable();
	if (bIsCurrItemSameAndStackable) {
		int32 CurrentStackLimit = InventoryEntry->GetStackLimit();
		if (NewItem->GetAmount() <= CurrentStackLimit) {
			InventoryEntry->SetAmount(InventoryEntry->GetAmount() + NewItem->GetAmount());
			return true;
		}
		InventoryEntry->SetAmount(InventoryEntry->GetAmount() + CurrentStackLimit);
		NewItem->SetAmount(NewItem->GetAmount() - CurrentStackLimit);
		RemainingItem = NewItem;
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

UEterniaInventoryEntry* UEquipmentSlot::Clear() {
	UEterniaInventoryEntry* OldItem = InventoryEntry;
	if (InventoryEntry) {
		InventoryEntry->OnItemAmountChanged.RemoveDynamic(this, &UEquipmentSlot::HandleItemAmountChanged);
	}
	InventoryEntry = nullptr;
	OnEquippedItemChanged.Broadcast(this, OldItem);
	return OldItem;
}

FETEquipmentSlot UEquipmentSlot::GetType() const {
	FETEquipmentSlot* Row = SlotTypeRowHandle.GetRow<FETEquipmentSlot>("");
	if (Row) {
		return *Row;
	}
	return FETEquipmentSlot();
}

void UEquipmentSlot::SetIsBlocked(bool InbIsBlocked) {
	if (bIsBlocked == InbIsBlocked) return;

	bIsBlocked = InbIsBlocked;
	OnIsBlockedChanged.Broadcast(this);
}

bool UEquipmentSlot::DoSetItem(UEterniaInventoryEntry* NewItem) {
	if (NewItem && NewItem != InventoryEntry && NewItem->GetDefinition()) {
		if (IsValidForItemType(NewItem->GetDefinition()->GetItemType())) {
			UEterniaInventoryEntry* OldItem = InventoryEntry;
			if (InventoryEntry) {
				InventoryEntry->OnItemAmountChanged.RemoveDynamic(this, &UEquipmentSlot::HandleItemAmountChanged);
			}
			InventoryEntry = NewItem;
			InventoryEntry->OnItemAmountChanged.AddUniqueDynamic(this, &UEquipmentSlot::HandleItemAmountChanged);
			OnEquippedItemChanged.Broadcast(this, OldItem);
			return true;
		}
	}
	return false;
}
