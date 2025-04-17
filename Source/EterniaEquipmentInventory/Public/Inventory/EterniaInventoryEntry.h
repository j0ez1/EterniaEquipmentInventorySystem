// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/EterniaInventoryItemDefinition.h"
#include "UObject/Object.h"
#include "EterniaInventoryEntry.generated.h"

class IAbilitySystemInterface;
class UEterniaInventoryEntry;
class UETInventoryComponent;
class UEterniaInventoryItemDefinition;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemAmountChangedDelegate, UEterniaInventoryEntry*, UpdatedItem, int32, NewAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemRotated_InventoryEntry, UEterniaInventoryEntry*, Item);

/**
 * 
 */
UCLASS(BlueprintType)
class ETERNIAEQUIPMENTINVENTORY_API UEterniaInventoryEntry : public UObject {
	GENERATED_BODY()

public:

	FORCEINLINE UEterniaInventoryItemDefinition* GetDefinition() const { return Definition; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetAmount() const { return Amount; }

	void IncrementAmount(int32 AmountToAdd);

	void SetDefinition(UEterniaInventoryItemDefinition* Definition);

	void SetAmount(int32 Am);

	UPROPERTY(BlueprintAssignable)
	FOnItemAmountChangedDelegate OnItemAmountChanged;

	UFUNCTION(BlueprintCallable)
	FIntPoint GetDimensions();

	UFUNCTION(BlueprintCallable)
	UMaterialInterface* GetImage();

	UFUNCTION(BlueprintCallable)
	void Rotate();

	UPROPERTY(BlueprintAssignable)
	FOnItemRotated_InventoryEntry OnItemRotated;

	UFUNCTION(BlueprintCallable)
	bool IsRotated() const { return Rotated; }

	FORCEINLINE UETInventoryComponent* GetOwningInventoryComponent() const { return OwningInventoryComponent; }

	void SetOwningInventoryComponent(UETInventoryComponent* InInventoryComponent) { OwningInventoryComponent = InInventoryComponent; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsStackable() const { return Definition ? Definition->IsStackable() : false; }

	UFUNCTION(BlueprintPure)
	bool IsSameItem(UEterniaInventoryEntry* Item) const;

	UFUNCTION(BlueprintPure)
	bool IsStackFull() const;

	UFUNCTION(BlueprintPure)
	int32 GetStackLimit() const;

	UFUNCTION(BlueprintCallable)
	void Activate(AActor* ActivatorActor);

protected:

	UPROPERTY(BlueprintReadOnly)
	UEterniaInventoryItemDefinition* Definition;

	UPROPERTY(BlueprintReadOnly)
	int32 Amount;

	UPROPERTY(BlueprintReadOnly)
	bool Rotated = false;

	UPROPERTY(BlueprintReadOnly)
	UETInventoryComponent* OwningInventoryComponent;
};
