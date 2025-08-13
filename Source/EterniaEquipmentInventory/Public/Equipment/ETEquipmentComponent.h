// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ETEquipmentSlot.h"
#include "Components/ActorComponent.h"
#include "Data/ETDAItemType.h"
#include "ETEquipmentComponent.generated.h"

class UETDAEquipmentSlotType;
class UETEquipmentSlot;
class UETInventoryEntry;
class UEterniaInventoryWeaponDefinition;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ETERNIAEQUIPMENTINVENTORY_API UETEquipmentComponent : public UActorComponent {
	GENERATED_BODY()

public:

	UETEquipmentComponent(const FObjectInitializer& ObjectInitializer);

	/*
	 * Try to equip item to first valid empty slot or if bForceEquip == true then try to replace an item in first valid occupied slot  
	 */
	bool TryEquipItem(UETInventoryEntry* InventoryEntry, bool bForceEquip, UETInventoryEntry*& RemainingItem);

	FORCEINLINE TArray<TObjectPtr<UETEquipmentSlot>> GetSlots() const { return Slots; }

	UFUNCTION(BlueprintCallable)
	UETEquipmentSlot* FindSlotByName(const FName& Name) const;

	UFUNCTION(BlueprintCallable)
	UETEquipmentSlot* FindSlotByType(const UETDAEquipmentSlotType* SlotType) const;

	UFUNCTION(BlueprintCallable)
	TArray<UETEquipmentSlot*> FindAllValidSlotsForItemType(const UETDAItemType* ItemType) const;

	UFUNCTION(BlueprintCallable)
	UETEquipmentSlot* FindSlotByInputAction(const UInputAction* InputAction) const;

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Replicated)
	TArray<TObjectPtr<UETEquipmentSlot>> Slots;

	void UpdateSlotBlockState();

	UFUNCTION()
	void OnEquippedItemChanged_EquipmentSlot(UETEquipmentSlot* Slot, UETInventoryEntry* OldItem, bool bSilent);
};
