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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquipmentComponentOnSlotUpdatedDelegate, UEquipmentSlot*, Slot, UEterniaInventoryEntry*, Item);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ETERNIAEQUIPMENTINVENTORY_API UEterniaEquipmentComponent : public UActorComponent {
	GENERATED_BODY()

public:

	UEterniaEquipmentComponent();

	bool TryEquipItem(UEterniaInventoryEntry* NewItem, UEquipmentSlot* Slot, bool ForceEquip);

	bool TryEquipItem(UEterniaInventoryEntry* InventoryEntry, bool ForceEquip);

	FORCEINLINE TArray<TObjectPtr<UEquipmentSlot>> GetSlots() const { return Slots; }

	UFUNCTION(BlueprintCallable)
	UEquipmentSlot* FindSlotByName(const FName& Name) const;

	UFUNCTION(BlueprintCallable)
	UEquipmentSlot* FindSlotByType(const FETEquipmentSlot& SlotType) const;

	UFUNCTION(BlueprintCallable)
	TArray<UEquipmentSlot*> FindAllValidSlotsForItemType(const FETItemType& ItemType) const;

	UFUNCTION(BlueprintCallable)
	UEquipmentSlot* FindSlotByInputAction(const UInputAction* InputAction) const;

	void ClearSlot(UEquipmentSlot* EquipmentSlot);

	FEquipmentComponentOnSlotUpdatedDelegate OnSlotUpdated;

	UFUNCTION()
	void NotifySlotUpdated(UEquipmentSlot* Slot, UEterniaInventoryEntry* Item);

	virtual void BeginPlay() override;

	bool ActivateSlotByInputAction(const UInputAction* InputAction);

protected:

	bool AreRequirementsMet(UEterniaInventoryItemDefinition* InventoryWeapon) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TArray<TObjectPtr<UEquipmentSlot>> Slots;
};
