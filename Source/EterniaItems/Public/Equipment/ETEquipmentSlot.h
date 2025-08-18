// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ETDAItemType.h"
#include "UObject/Object.h"
#include "ETEquipmentSlot.generated.h"

class UETDAEquipmentSlotType;
class IAbilitySystemInterface;
class UInputAction;
class UETItem;

// FIXME bSilent usage is an ugly approach but it is needed for dependent code. Find a way to get rid of it
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnEquippedItemChanged_EquipmentSlot, UETEquipmentSlot*, Slot, UETItem*, OldItem, bool, bSilent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIsBlockedChanged_EquipmentSlot, UETEquipmentSlot*, Slot);

/**
 * 
 */
UCLASS(BlueprintType, EditInlineNew)
class ETERNIAITEMS_API UETEquipmentSlot : public UObject {
	GENERATED_BODY()

public:

	UETEquipmentSlot(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	bool TryEquipItem(UETItem* NewItem, bool bForceEquip, UETItem*& RemainingItem);

	UFUNCTION(BlueprintCallable)
	bool IsValidForItem(const UETItem* Item) const;

	UFUNCTION(BlueprintCallable)
	bool IsValidForItemType(const UETDAItemType* ItemType) const;

	UFUNCTION(BlueprintCallable)
	bool IsEmpty() const { return Item == nullptr; }

	UFUNCTION(BlueprintCallable)
	UETItem* Clear(bool bSilent = false);

#pragma region GettersSetters

	FORCEINLINE UETItem* GetInventoryEntry() const { return Item; }

	FORCEINLINE FName GetSlotName() const { return Name; }

	FORCEINLINE UInputAction* GetInputAction() const { return InputAction; }

	FORCEINLINE bool IsActivatable() const { return bIsActivatable; }

	UFUNCTION(BlueprintCallable)
	UETDAEquipmentSlotType* GetType() const;

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
	UETItem* Item;

	UPROPERTY(EditDefaultsOnly)
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UETDAEquipmentSlotType> Type;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(InlineEditConditionToggle))
	bool bIsActivatable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsBlocked;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition="bIsActivatable"))
	UInputAction* InputAction;

	bool DoSetItem(UETItem* NewItem);

	UFUNCTION()
	void HandleItemAmountChanged(UETItem* UpdatedItem, int32 NewAmount);
};
