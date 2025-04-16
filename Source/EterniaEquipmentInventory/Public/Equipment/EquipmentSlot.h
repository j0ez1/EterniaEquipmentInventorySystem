// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentTypes.h"
#include "UObject/Object.h"
#include "EquipmentSlot.generated.h"

class IAbilitySystemInterface;
class UInputAction;
enum EEquipType : uint8;
class UEterniaInventoryEntry;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquippedItemChanged, UEquipmentSlot*, Slot, UEterniaInventoryEntry*, Item);

/**
 * 
 */
UCLASS(BlueprintType, EditInlineNew)
class ETERNIAEQUIPMENTINVENTORY_API UEquipmentSlot : public UObject {
	GENERATED_BODY()

public:

	UEquipmentSlot();

	void Init(EEquipmentSlotType Type);

	FORCEINLINE UEterniaInventoryEntry* GetInventoryEntry() const { return InventoryEntry; }

	UFUNCTION()
	void HandleItemAmountChanged(UEterniaInventoryEntry* UpdatedItem, int32 NewAmount);

	UFUNCTION(BlueprintCallable)
	bool SetItem(UEterniaInventoryEntry* Item);

	FORCEINLINE FName GetSlotName() const { return SlotName; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE TEnumAsByte<EEquipmentSlotType> GetSlotType() const { return SlotType; }

	UFUNCTION(BlueprintCallable)
	bool IsValidForType(EEquipType Equip) const;

	bool IsEmpty();

	UFUNCTION(BlueprintCallable)
	void Clear();

	bool ActivateItem(AActor* ActivatorActor);

	UPROPERTY(BlueprintAssignable)
	FOnEquippedItemChanged OnEquippedItemChanged;

	FORCEINLINE UInputAction* GetInputAction() const { return InputAction; }

	bool IsActivatable() const { return bIsActivatable; }

protected:

	UPROPERTY(BlueprintReadOnly)
	UEterniaInventoryEntry* InventoryEntry = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FName SlotName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<EEquipmentSlotType> SlotType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(InlineEditConditionToggle))
	bool bIsActivatable = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bIsActivatable"))
	UInputAction* InputAction;
};
