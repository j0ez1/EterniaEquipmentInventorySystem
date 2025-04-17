// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/EterniaEquipmentComponent.h"

#include "Equipment/EquipmentSlot.h"
#include "Inventory/EterniaInventoryEntry.h"

UEterniaEquipmentComponent::UEterniaEquipmentComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}

bool UEterniaEquipmentComponent::TryEquipItem(UEterniaInventoryEntry* InventoryEntry, bool bForceEquip, UEterniaInventoryEntry*& RemainingItem) {
	if (InventoryEntry && InventoryEntry->GetDefinition()) {
		TArray<UEquipmentSlot*> FoundSlots = FindAllValidSlotsForItemType(InventoryEntry->GetDefinition()->GetItemType());

		// Try empty slots first regardless of bForceEquip ...
		for (UEquipmentSlot* Slot : FoundSlots) {
			if (Slot->IsEmpty()) {
				if (Slot->TryEquipItem(InventoryEntry, bForceEquip, RemainingItem)) {
					return true;
				}
			}
		}

		// ... then try occupied slots
		for (UEquipmentSlot* Slot : FoundSlots) {
			if (!Slot->IsEmpty()) {
				if (Slot->TryEquipItem(InventoryEntry, bForceEquip, RemainingItem)) {
					return true;
				}
			}
		}
	}
	return false;
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

void UEterniaEquipmentComponent::BeginPlay() {
	Super::BeginPlay();

	for (auto Slot : Slots) {
		Slot->OnEquippedItemChanged.AddDynamic(this, &UEterniaEquipmentComponent::OnEquippedItemChanged_EquipmentSlot);
		Slot->OnIsBlockedChanged.AddDynamic(this, &UEterniaEquipmentComponent::OnIsBlockedChanged_EquipmentSlot);
	}
}

void UEterniaEquipmentComponent::UpdateSlotBlockState() {
	TMap<FName, int32> BlockedSlotTypesMap;
	for (UEquipmentSlot* Slot : Slots) {
		UEterniaInventoryEntry* OccupyingItem = Slot->GetInventoryEntry();
		if (OccupyingItem && OccupyingItem->GetDefinition()) {
			TArray<FETEquipmentSlot> SlotTypesToBlock = OccupyingItem->GetDefinition()->GetItemType().GetBlocksEquipmentSlotTypes();
			for (const FETEquipmentSlot& SlotType : SlotTypesToBlock) {
				if (BlockedSlotTypesMap.Contains(SlotType.Identifier)) {
					BlockedSlotTypesMap[SlotType.Identifier]++;
				} else {
					BlockedSlotTypesMap.Add(SlotType.Identifier, 1);
				}
			}
		}
	}

	for (UEquipmentSlot* Slot : Slots) {
		if (Slot->IsEmpty()) {
			if (BlockedSlotTypesMap.Contains(Slot->GetType().Identifier)) {
				BlockedSlotTypesMap[Slot->GetType().Identifier]--;
				if (BlockedSlotTypesMap[Slot->GetType().Identifier] <= 0) {
					BlockedSlotTypesMap.Remove(Slot->GetType().Identifier);
				}
				Slot->SetIsBlocked(true);
			} else {
				Slot->SetIsBlocked(false);
			}
		}
	}

	for (UEquipmentSlot* Slot : Slots) {
		if (!Slot->IsEmpty()) {
			if (BlockedSlotTypesMap.Contains(Slot->GetType().Identifier)) {
				BlockedSlotTypesMap[Slot->GetType().Identifier]--;
				if (BlockedSlotTypesMap[Slot->GetType().Identifier] <= 0) {
					BlockedSlotTypesMap.Remove(Slot->GetType().Identifier);
				}
				Slot->SetIsBlocked(true);
			} else {
				Slot->SetIsBlocked(false);
			}
		}
	}
}

void UEterniaEquipmentComponent::OnEquippedItemChanged_EquipmentSlot(UEquipmentSlot* Slot, UEterniaInventoryEntry* OldItem) {
	UpdateSlotBlockState();
}

void UEterniaEquipmentComponent::OnIsBlockedChanged_EquipmentSlot(UEquipmentSlot* Slot) {
	if (Slot && Slot->IsBlocked()) {
		Slot->Clear();
	}
}
