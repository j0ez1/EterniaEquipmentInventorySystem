// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ETDAItemDefinition.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ETEquipmentInventorySubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ETERNIAITEMS_API UETEquipmentInventorySubsystem : public UGameInstanceSubsystem {
	GENERATED_BODY()

public:

	static UETEquipmentInventorySubsystem* GetCurrent(UObject* WorldContextObject);

	FORCEINLINE UDataTable* GetItemCombinationDataTable() const { return ItemCombinationDataTable.LoadSynchronous(); }

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

protected:

	UPROPERTY()
	TSoftObjectPtr<UDataTable> ItemCombinationDataTable;
};
