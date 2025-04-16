// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ETEquipmentInventorySettings.generated.h"

/**
 * 
 */
UCLASS(Config = EterniaSettings, DefaultConfig, DisplayName="Eternia Equipment & Inventory System", meta=(ScriptName="EquipmentInventorySettings"))
class ETERNIAEQUIPMENTINVENTORY_API UETEquipmentInventorySettings : public UDeveloperSettings {
	GENERATED_BODY()

public:

	UETEquipmentInventorySettings(const FObjectInitializer& ObjInit);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, meta = (RequiredAssetDataTags = "RowStructure=/Script/EterniaEquipmentInventory.EtItemDefinition"))
	TSoftObjectPtr<UDataTable> ItemDatabase;
};
