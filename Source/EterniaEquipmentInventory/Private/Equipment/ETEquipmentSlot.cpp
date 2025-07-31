// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/ETEquipmentSlot.h"

#include "Inventory/ETInventoryEntry.h"

UETEquipmentSlot::UETEquipmentSlot(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	bIsActivatable(false), bIsBlocked(false) {
}

bool UETEquipmentSlot::TryEquipItem(UETInventoryEntry* NewItem, bool bForceEquip, UETInventoryEntry*& RemainingItem) {
	if (bIsBlocked) {
		RemainingItem = NewItem;
		return false;
	}

	UETInventoryEntry* OccupyingItem = InventoryEntry;
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

bool UETEquipmentSlot::IsValidForItemType(const FETItemType& ItemType) const {
	FETEquipmentSlotType Type = GetType();
	for (FETEquipmentSlotType ValidType : ItemType.GetValidEquipmentSlotTypes()) {
		if (ValidType == Type) {
			return true;
		}
	}
	return false;
}

UETInventoryEntry* UETEquipmentSlot::Clear(bool bSilent) {
	UETInventoryEntry* OldItem = InventoryEntry;
	if (InventoryEntry) {
		InventoryEntry->OnItemAmountChanged.RemoveDynamic(this, &UETEquipmentSlot::HandleItemAmountChanged);
	}
	InventoryEntry = nullptr;
	OnEquippedItemChanged.Broadcast(this, OldItem, bSilent);
	return OldItem;
}

FETEquipmentSlotType UETEquipmentSlot::GetType() const {
	FETEquipmentSlotType* Row = SlotTypeRowHandle.GetRow<FETEquipmentSlotType>("");
	if (Row) {
		return *Row;
	}
	return FETEquipmentSlotType();
}

void UETEquipmentSlot::SetIsBlocked(bool InbIsBlocked) {
	if (bIsBlocked == InbIsBlocked)
		return;

	bIsBlocked = InbIsBlocked;
	if (bIsBlocked) {
		Clear();
	}
	OnIsBlockedChanged.Broadcast(this);
}

bool UETEquipmentSlot::DoSetItem(UETInventoryEntry* NewItem) {
	if (NewItem && NewItem != InventoryEntry && NewItem->GetDefinition()) {
		if (IsValidForItemType(NewItem->GetDefinition()->GetItemType())) {
			UETInventoryEntry* OldItem = InventoryEntry;
			if (InventoryEntry) {
				InventoryEntry->OnItemAmountChanged.RemoveDynamic(this, &UETEquipmentSlot::HandleItemAmountChanged);
			}
			InventoryEntry = NewItem;
			InventoryEntry->OnItemAmountChanged.AddUniqueDynamic(this, &UETEquipmentSlot::HandleItemAmountChanged);
			OnEquippedItemChanged.Broadcast(this, OldItem, false);
			return true;
		}
	}
	return false;
}

void UETEquipmentSlot::HandleItemAmountChanged(UETInventoryEntry* UpdatedItem, int32 NewAmount) {
	if (InventoryEntry == UpdatedItem) {
		if (NewAmount <= 0) {
			Clear();
		} else {
			OnEquippedItemChanged.Broadcast(this, nullptr, false);
		}
	}
}
