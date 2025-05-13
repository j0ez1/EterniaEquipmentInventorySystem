// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/EquipmentTypes.h"
#include "UObject/Object.h"
#include "EquipmentSlot.generated.h"

class IAbilitySystemInterface;
class UInputAction;
class UEterniaInventoryEntry;

// FIXME bSilent usage is an ugly approach but it is needed for dependent code. Find a way to get rid of it
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnEquippedItemChanged, UEquipmentSlot*, Slot, UEterniaInventoryEntry*, OldItem, bool, bSilent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIsBlockedChanged_EquipmentSlot, UEquipmentSlot*, Slot);

/**
 * 
 */
UCLASS(BlueprintType, EditInlineNew)
class ETERNIAEQUIPMENTINVENTORY_API UEquipmentSlot : public UObject {
	GENERATED_BODY()

public:

	UEquipmentSlot(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	bool TryEquipItem(UEterniaInventoryEntry* NewItem, bool bForceEquip, UEterniaInventoryEntry*& RemainingItem);

	UFUNCTION(BlueprintCallable)
	bool IsValidForItemType(const FETItemType& ItemType) const;

	UFUNCTION(BlueprintCallable)
	bool IsEmpty() const { return InventoryEntry == nullptr; }

	UFUNCTION(BlueprintCallable)
	UEterniaInventoryEntry* Clear(bool bSilent = false);

#pragma region GettersSetters

	FORCEINLINE UEterniaInventoryEntry* GetInventoryEntry() const { return InventoryEntry; }

	FORCEINLINE FName GetSlotName() const { return SlotName; }

	FORCEINLINE UInputAction* GetInputAction() const { return InputAction; }

	FORCEINLINE bool IsActivatable() const { return bIsActivatable; }

	UFUNCTION(BlueprintCallable)
	FETEquipmentSlot GetType() const;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsBlocked() const { return bIsBlocked; }

	UFUNCTION(BlueprintCallable)
	void SetIsBlocked(bool InbIsBlocked);

#pragma endregion

#pragma region Delegates

	UPROPERTY(BlueprintAssignable)
	FOnEquippedItemChanged OnEquippedItemChanged;

	UPROPERTY(BlueprintAssignable)
	FOnIsBlockedChanged_EquipmentSlot OnIsBlockedChanged;

#pragma endregion 

protected:

	UPROPERTY(BlueprintReadOnly)
	UEterniaInventoryEntry* InventoryEntry;

	UPROPERTY(EditDefaultsOnly)
	FName SlotName;

	UPROPERTY(EditDefaultsOnly, meta=(RequiredAssetDataTags="RowStructure=/Script/EterniaEquipmentInventory.ETEquipmentSlot"))
	FDataTableRowHandle SlotTypeRowHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(InlineEditConditionToggle))
	bool bIsActivatable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsBlocked;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bIsActivatable"))
	UInputAction* InputAction;

	bool DoSetItem(UEterniaInventoryEntry* NewItem);

	UFUNCTION()
	void HandleItemAmountChanged(UEterniaInventoryEntry* UpdatedItem, int32 NewAmount);
};
