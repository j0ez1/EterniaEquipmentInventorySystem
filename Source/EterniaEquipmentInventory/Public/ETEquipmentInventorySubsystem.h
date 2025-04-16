// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	FORCEINLINE UDataTable* GetItemDatabase() const { return ItemDatabase; }

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

protected:
	UPROPERTY()
	UDataTable* ItemDatabase;
};
