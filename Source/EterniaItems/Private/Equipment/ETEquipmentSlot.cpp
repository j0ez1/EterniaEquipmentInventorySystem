// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/ETEquipmentSlot.h"

#include "Data/ETDAItemDefinition.h"
#include "Items/ETItem.h"

UETEquipmentSlot::UETEquipmentSlot(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	bIsActivatable(false), bIsBlocked(false) {
}

bool UETEquipmentSlot::TryEquipItem(UETItem* NewItem, bool bForceEquip, UETItem*& RemainingItem) {
	if (bIsBlocked) {
		RemainingItem = NewItem;
		return false;
	}

	UETItem* OccupyingItem = Item;
	if (IsEmpty() || !Item->IsSameItem(NewItem) && bForceEquip) {
		bool bSuccess = DoSetItem(NewItem);
		RemainingItem = bSuccess ? OccupyingItem : NewItem;
		return bSuccess;
	}

	bool bIsCurrItemSameAndStackable = Item && Item->IsSameItem(NewItem) && Item->IsStackable();
	if (bIsCurrItemSameAndStackable) {
		int32 CurrentStackLimit = Item->GetStackLimit();
		if (NewItem->GetAmount() <= CurrentStackLimit) {
			Item->SetAmount(Item->GetAmount() + NewItem->GetAmount());
			return true;
		}
		Item->SetAmount(Item->GetAmount() + CurrentStackLimit);
		NewItem->SetAmount(NewItem->GetAmount() - CurrentStackLimit);
		RemainingItem = NewItem;
	}
	return false;
}

bool UETEquipmentSlot::IsValidForItem(const UETItem* Item) const {
	UETDAItemDefinition* Definition = Item->GetDefinition();
	UETDAItemType* ItemType = Definition->GetType();
	return Definition && IsValidForItemType(ItemType);
}

bool UETEquipmentSlot::IsValidForItemType(const UETDAItemType* ItemType) const {
	for (TSoftObjectPtr<UETDAEquipmentSlotType> ValidType : ItemType->GetValidEquipmentSlotTypes()) {
		return ValidType.LoadSynchronous() == Type.LoadSynchronous();
	}
	return false;
}

UETItem* UETEquipmentSlot::Clear(bool bSilent) {
	UETItem* OldItem = Item;
	if (Item) {
		Item->OnItemAmountChanged.RemoveDynamic(this, &UETEquipmentSlot::HandleItemAmountChanged);
	}
	Item = nullptr;
	OnEquippedItemChanged.Broadcast(this, OldItem, bSilent);
	return OldItem;
}

UETDAEquipmentSlotType* UETEquipmentSlot::GetType() const {
	return Type.LoadSynchronous();
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

bool UETEquipmentSlot::DoSetItem(UETItem* NewItem) {
	if (NewItem && NewItem != Item && NewItem->GetDefinition()) {
		if (IsValidForItemType(NewItem->GetDefinition()->GetType())) {
			UETItem* OldItem = Item;
			if (Item) {
				Item->OnItemAmountChanged.RemoveDynamic(this, &UETEquipmentSlot::HandleItemAmountChanged);
			}
			Item = NewItem;
			Item->OnItemAmountChanged.AddUniqueDynamic(this, &UETEquipmentSlot::HandleItemAmountChanged);
			OnEquippedItemChanged.Broadcast(this, OldItem, false);
			return true;
		}
	}
	return false;
}

void UETEquipmentSlot::HandleItemAmountChanged(UETItem* UpdatedItem, int32 NewAmount) {
	if (Item == UpdatedItem) {
		if (NewAmount <= 0) {
			Clear();
		} else {
			OnEquippedItemChanged.Broadcast(this, nullptr, false);
		}
	}
}
