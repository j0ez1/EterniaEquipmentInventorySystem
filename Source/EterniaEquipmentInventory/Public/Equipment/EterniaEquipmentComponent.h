// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentSlot.h"
#include "Components/ActorComponent.h"
#include "Data/EquipmentTypes.h"
#include "EterniaEquipmentComponent.generated.h"

class UEterniaInventoryItemDefinition;
class UEquipmentSlot;
class UEterniaInventoryEntry;
class UEterniaInventoryWeaponDefinition;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ETERNIAEQUIPMENTINVENTORY_API UEterniaEquipmentComponent : public UActorComponent {
	GENERATED_BODY()

public:

	UEterniaEquipmentComponent();

	/*
	 * Try to equip item to first valid empty slot or if bForceEquip == true then try to replace an item in first valid occupied slot  
	 */
	bool TryEquipItem(UEterniaInventoryEntry* InventoryEntry, bool bForceEquip, UEterniaInventoryEntry*& RemainingItem);

	FORCEINLINE TArray<TObjectPtr<UEquipmentSlot>> GetSlots() const { return Slots; }

	UFUNCTION(BlueprintCallable)
	UEquipmentSlot* FindSlotByName(const FName& Name) const;

	UFUNCTION(BlueprintCallable)
	UEquipmentSlot* FindSlotByType(const FETEquipmentSlot& SlotType) const;

	UFUNCTION(BlueprintCallable)
	TArray<UEquipmentSlot*> FindAllValidSlotsForItemType(const FETItemType& ItemType) const;

	UFUNCTION(BlueprintCallable)
	UEquipmentSlot* FindSlotByInputAction(const UInputAction* InputAction) const;

	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TArray<TObjectPtr<UEquipmentSlot>> Slots;

	void UpdateSlotBlockState();

	UFUNCTION()
	void OnEquippedItemChanged_EquipmentSlot(UEquipmentSlot* Slot, UEterniaInventoryEntry* OldItem, bool bSilent);
};
