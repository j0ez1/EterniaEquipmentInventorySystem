// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/ETEquipmentComponent.h"

#include "Equipment/ETEquipmentSlot.h"
#include "Inventory/ETInventoryEntry.h"

UETEquipmentComponent::UETEquipmentComponent(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer) {
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

bool UETEquipmentComponent::TryEquipItem(UETInventoryEntry* InventoryEntry, bool bForceEquip, UETInventoryEntry*& RemainingItem) {
	if (InventoryEntry && InventoryEntry->GetDefinition()) {
		TArray<UETEquipmentSlot*> FoundSlots = FindAllValidSlotsForItemType(InventoryEntry->GetDefinition()->GetItemType());

		// Try empty slots first regardless of bForceEquip ...
		for (UETEquipmentSlot* Slot : FoundSlots) {
			if (Slot->IsEmpty()) {
				if (Slot->TryEquipItem(InventoryEntry, bForceEquip, RemainingItem)) {
					return true;
				}
			}
		}

		// ... then try occupied slots
		for (UETEquipmentSlot* Slot : FoundSlots) {
			if (!Slot->IsEmpty()) {
				if (Slot->TryEquipItem(InventoryEntry, bForceEquip, RemainingItem)) {
					return true;
				}
			}
		}
	}
	return false;
}

UETEquipmentSlot* UETEquipmentComponent::FindSlotByName(const FName& Name) const {
	for (auto EquipmentSlot : Slots) {
		if (EquipmentSlot->GetSlotName() == Name) {
			return EquipmentSlot;
		}
	}
	return nullptr;
}

UETEquipmentSlot* UETEquipmentComponent::FindSlotByType(const FETEquipmentSlotType& SlotType) const {
	for (auto EquipmentSlot : Slots) {
		if (EquipmentSlot->GetType() == SlotType) {
			return EquipmentSlot;
		}
	}
	return nullptr;
}

TArray<UETEquipmentSlot*> UETEquipmentComponent::FindAllValidSlotsForItemType(const FETItemType& ItemType) const {
	TArray<UETEquipmentSlot*> ResultArray;
	for (auto EquipmentSlot : Slots) {
		if (EquipmentSlot->IsValidForItemType(ItemType)) {
			ResultArray.Add(EquipmentSlot);
		}
	}
	return ResultArray;
}

UETEquipmentSlot* UETEquipmentComponent::FindSlotByInputAction(const UInputAction* InputAction) const {
	for (auto EquipmentSlot : Slots) {
		if (EquipmentSlot->GetInputAction() == InputAction) {
			return EquipmentSlot;
		}
	}
	return nullptr;
}

void UETEquipmentComponent::BeginPlay() {
	Super::BeginPlay();

	for (auto Slot : Slots) {
		Slot->OnEquippedItemChanged.AddDynamic(this, &UETEquipmentComponent::OnEquippedItemChanged_EquipmentSlot);
	}
}

void UETEquipmentComponent::UpdateSlotBlockState() {
	TMap<FName, int32> BlockedSlotTypesMap;
	for (UETEquipmentSlot* Slot : Slots) {
		UETInventoryEntry* OccupyingItem = Slot->GetInventoryEntry();
		if (OccupyingItem && OccupyingItem->GetDefinition()) {
			TArray<FETEquipmentSlotType> SlotTypesToBlock = OccupyingItem->GetDefinition()->GetItemType().GetBlocksEquipmentSlotTypes();
			for (const FETEquipmentSlotType& SlotType : SlotTypesToBlock) {
				if (BlockedSlotTypesMap.Contains(SlotType.Identifier)) {
					BlockedSlotTypesMap[SlotType.Identifier]++;
				} else {
					BlockedSlotTypesMap.Add(SlotType.Identifier, 1);
				}
			}
		}
	}

	for (UETEquipmentSlot* Slot : Slots) {
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

	for (UETEquipmentSlot* Slot : Slots) {
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

void UETEquipmentComponent::OnEquippedItemChanged_EquipmentSlot(UETEquipmentSlot* Slot, UETInventoryEntry* OldItem, bool bSilent) {
	UpdateSlotBlockState();
}
