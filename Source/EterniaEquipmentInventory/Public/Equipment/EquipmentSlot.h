// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/EquipmentTypes.h"
#include "UObject/Object.h"
#include "EquipmentSlot.generated.h"

class IAbilitySystemInterface;
class UInputAction;
class UEterniaInventoryEntry;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquippedItemChanged, UEquipmentSlot*, Slot, UEterniaInventoryEntry*, OldItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIsBlockedChanged_EquipmentSlot, UEquipmentSlot*, Slot);

/**
 * 
 */
UCLASS(BlueprintType, EditInlineNew)
class ETERNIAEQUIPMENTINVENTORY_API UEquipmentSlot : public UObject {
	GENERATED_BODY()

public:

	UEquipmentSlot();

	FORCEINLINE UEterniaInventoryEntry* GetInventoryEntry() const { return InventoryEntry; }

	UFUNCTION()
	void HandleItemAmountChanged(UEterniaInventoryEntry* UpdatedItem, int32 NewAmount);

	UFUNCTION(BlueprintCallable)
	bool TryEquipItem(UEterniaInventoryEntry* NewItem, bool bForceEquip, UEterniaInventoryEntry*& RemainingItem);

	FORCEINLINE FName GetSlotName() const { return SlotName; }

	UFUNCTION(BlueprintCallable)
	bool IsValidForItemType(const FETItemType& ItemType) const;

	UFUNCTION(BlueprintCallable)
	bool IsEmpty() const { return InventoryEntry == nullptr; }

	UFUNCTION(BlueprintCallable)
	UEterniaInventoryEntry* Clear();

	UPROPERTY(BlueprintAssignable)
	FOnEquippedItemChanged OnEquippedItemChanged;

	FORCEINLINE UInputAction* GetInputAction() const { return InputAction; }

	bool IsActivatable() const { return bIsActivatable; }

	UFUNCTION(BlueprintCallable)
	FETEquipmentSlot GetType() const;

	UFUNCTION(BlueprintCallable)
	bool IsBlocked() const { return bIsBlocked; }

	UFUNCTION(BlueprintCallable)
	void SetIsBlocked(bool InbIsBlocked);

	UPROPERTY(BlueprintAssignable)
	FOnIsBlockedChanged_EquipmentSlot OnIsBlockedChanged;

protected:

	UPROPERTY(BlueprintReadOnly)
	UEterniaInventoryEntry* InventoryEntry = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FName SlotName;

	UPROPERTY(EditDefaultsOnly, meta=(RequiredAssetDataTags="RowStructure=/Script/EterniaEquipmentInventory.ETEquipmentSlot"))
	FDataTableRowHandle SlotTypeRowHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(InlineEditConditionToggle))
	bool bIsActivatable = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsBlocked = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bIsActivatable"))
	UInputAction* InputAction;

	bool DoSetItem(UEterniaInventoryEntry* NewItem);
};
