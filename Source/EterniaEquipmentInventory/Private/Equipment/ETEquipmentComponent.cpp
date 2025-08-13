// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/ETEquipmentComponent.h"

#include "Data/ETDAEquipmentSlotType.h"
#include "Data/ETDAItemDefinition.h"
#include "Equipment/ETEquipmentSlot.h"
#include "Inventory/ETInventoryEntry.h"
#include "Net/UnrealNetwork.h"

UETEquipmentComponent::UETEquipmentComponent(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer) {
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

bool UETEquipmentComponent::TryEquipItem(UETInventoryEntry* InventoryEntry, bool bForceEquip, UETInventoryEntry*& RemainingItem) {
	if (InventoryEntry && InventoryEntry->GetDefinition()) {
		TArray<UETEquipmentSlot*> FoundSlots = FindAllValidSlotsForItemType(InventoryEntry->GetDefinition()->GetType());

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

UETEquipmentSlot* UETEquipmentComponent::FindSlotByType(const UETDAEquipmentSlotType* SlotType) const {
	for (auto EquipmentSlot : Slots) {
		if (EquipmentSlot->GetType() == SlotType) {
			return EquipmentSlot;
		}
	}
	return nullptr;
}

TArray<UETEquipmentSlot*> UETEquipmentComponent::FindAllValidSlotsForItemType(const UETDAItemType* ItemType) const {
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

	if (GetOwner()->HasAuthority()) {
		for (auto Slot : Slots) {
			Slot->OnEquippedItemChanged.AddDynamic(this, &UETEquipmentComponent::OnEquippedItemChanged_EquipmentSlot);
		}
	}
}

void UETEquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UETEquipmentComponent, Slots, COND_None, REPNOTIFY_Always);
}

void UETEquipmentComponent::UpdateSlotBlockState() {
	TMap<FGuid, int32> BlockedSlotTypesMap;
	for (UETEquipmentSlot* Slot : Slots) {
		UETInventoryEntry* OccupyingItem = Slot->GetInventoryEntry();
		if (OccupyingItem && OccupyingItem->GetDefinition() && OccupyingItem->GetDefinition()->GetType()) {
			TArray<TSoftObjectPtr<UETDAEquipmentSlotType>> SlotTypesToBlock = OccupyingItem->GetDefinition()->GetType()->GetBlocksEquipmentSlotTypes();
			for (const TSoftObjectPtr<UETDAEquipmentSlotType> SlotType : SlotTypesToBlock) {
				if (BlockedSlotTypesMap.Contains(SlotType->GetIdentifier())) {
					BlockedSlotTypesMap[SlotType->GetIdentifier()]++;
				} else {
					BlockedSlotTypesMap.Add(SlotType->GetIdentifier(), 1);
				}
			}
		}
	}

	for (UETEquipmentSlot* Slot : Slots) {
		if (Slot->IsEmpty()) {
			FGuid Identifier = Slot->GetType()->GetIdentifier();
			if (BlockedSlotTypesMap.Contains(Identifier)) {
				BlockedSlotTypesMap[Identifier]--;
				if (BlockedSlotTypesMap[Identifier] <= 0) {
					BlockedSlotTypesMap.Remove(Identifier);
				}
				Slot->SetIsBlocked(true);
			} else {
				Slot->SetIsBlocked(false);
			}
		}
	}

	for (UETEquipmentSlot* Slot : Slots) {
		if (!Slot->IsEmpty()) {
			FGuid Identifier = Slot->GetType()->GetIdentifier();
			if (BlockedSlotTypesMap.Contains(Identifier)) {
				BlockedSlotTypesMap[Identifier]--;
				if (BlockedSlotTypesMap[Identifier] <= 0) {
					BlockedSlotTypesMap.Remove(Identifier);
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
