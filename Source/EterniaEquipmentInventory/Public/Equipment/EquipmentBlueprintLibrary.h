// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentSlot.h"
#include "Inventory/EterniaInventoryItemDefinition.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EquipmentBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ETERNIAEQUIPMENTINVENTORY_API UEquipmentBlueprintLibrary : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category="Equipment")
	static EEquipType GetEquipType(const EEquipmentSlotType& SlotType);

	UFUNCTION(BlueprintPure, Category="Equipment")
	static EEquipmentSlotType GetSlotType(const EEquipType& EquipType);

	UFUNCTION(BlueprintPure, Category="Equipment")
	static TArray<TEnumAsByte<EEquipmentSlotType>> GetOccupiedSlotTypes(const EEquipType& EquipType);
};
