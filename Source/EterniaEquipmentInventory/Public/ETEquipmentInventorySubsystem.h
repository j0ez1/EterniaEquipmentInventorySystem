// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/EterniaInventoryItemDefinition.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ETEquipmentInventorySubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ETERNIAEQUIPMENTINVENTORY_API UETEquipmentInventorySubsystem : public UGameInstanceSubsystem {
	GENERATED_BODY()

public:

	static UETEquipmentInventorySubsystem* GetCurrent(UObject* WorldContextObject);

	FORCEINLINE UDataTable* GetItemDatabase() const { return ItemDatabase.LoadSynchronous(); }

	FORCEINLINE UDataTable* GetItemCombinationDataTable() const { return ItemCombinationDataTable.LoadSynchronous(); }

	UEterniaInventoryItemDefinition* FindItemDefinitionById(FName ItemID);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

protected:

	UPROPERTY()
	TSoftObjectPtr<UDataTable> ItemDatabase;

	UPROPERTY()
	TSoftObjectPtr<UDataTable> ItemCombinationDataTable;
};
