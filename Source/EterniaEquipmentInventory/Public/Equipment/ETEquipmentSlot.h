// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/EquipmentTypes.h"
#include "UObject/Object.h"
#include "ETEquipmentSlot.generated.h"

class IAbilitySystemInterface;
class UInputAction;
class UETInventoryEntry;

// FIXME bSilent usage is an ugly approach but it is needed for dependent code. Find a way to get rid of it
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnEquippedItemChanged_EquipmentSlot, UETEquipmentSlot*, Slot, UETInventoryEntry*, OldItem, bool, bSilent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIsBlockedChanged_EquipmentSlot, UETEquipmentSlot*, Slot);

/**
 * 
 */
UCLASS(BlueprintType, EditInlineNew)
class ETERNIAEQUIPMENTINVENTORY_API UETEquipmentSlot : public UObject {
	GENERATED_BODY()

public:

	UETEquipmentSlot(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	bool TryEquipItem(UETInventoryEntry* NewItem, bool bForceEquip, UETInventoryEntry*& RemainingItem);

	UFUNCTION(BlueprintCallable)
	bool IsValidForItemType(const FETItemType& ItemType) const;

	UFUNCTION(BlueprintCallable)
	bool IsEmpty() const { return InventoryEntry == nullptr; }

	UFUNCTION(BlueprintCallable)
	UETInventoryEntry* Clear(bool bSilent = false);

#pragma region GettersSetters

	FORCEINLINE UETInventoryEntry* GetInventoryEntry() const { return InventoryEntry; }

	FORCEINLINE FName GetSlotName() const { return SlotName; }

	FORCEINLINE UInputAction* GetInputAction() const { return InputAction; }

	FORCEINLINE bool IsActivatable() const { return bIsActivatable; }

	UFUNCTION(BlueprintCallable)
	FETEquipmentSlotType GetType() const;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsBlocked() const { return bIsBlocked; }

	UFUNCTION(BlueprintCallable)
	void SetIsBlocked(bool InbIsBlocked);

#pragma endregion

#pragma region Delegates

	UPROPERTY(BlueprintAssignable)
	FOnEquippedItemChanged_EquipmentSlot OnEquippedItemChanged;

	UPROPERTY(BlueprintAssignable)
	FOnIsBlockedChanged_EquipmentSlot OnIsBlockedChanged;

#pragma endregion 

protected:

	UPROPERTY(BlueprintReadOnly)
	UETInventoryEntry* InventoryEntry;

	UPROPERTY(EditDefaultsOnly)
	FName SlotName;

	UPROPERTY(EditDefaultsOnly, meta=(RequiredAssetDataTags="RowStructure=/Script/EterniaEquipmentInventory.ETEquipmentSlotType"))
	FDataTableRowHandle SlotTypeRowHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(InlineEditConditionToggle))
	bool bIsActivatable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsBlocked;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bIsActivatable"))
	UInputAction* InputAction;

	bool DoSetItem(UETInventoryEntry* NewItem);

	UFUNCTION()
	void HandleItemAmountChanged(UETInventoryEntry* UpdatedItem, int32 NewAmount);
};
