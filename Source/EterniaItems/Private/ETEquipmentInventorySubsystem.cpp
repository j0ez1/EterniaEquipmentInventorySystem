// Fill out your copyright notice in the Description page of Project Settings.


#include "ETEquipmentInventorySubsystem.h"

#include "ETEquipmentInventorySettings.h"
#include "Data/ETDAItemDefinition.h"
#include "Inventory/ETInventoryStatics.h"

UETEquipmentInventorySubsystem* UETEquipmentInventorySubsystem::GetCurrent(UObject* WorldContextObject) {
	UWorld* World = Cast<UWorld>(WorldContextObject);
	if (World == nullptr && WorldContextObject != nullptr) {
		World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	}

	if (World) {
		UGameInstance* GameInstance = World->GetGameInstance();
		if (GameInstance) {
			return GameInstance->GetSubsystem<UETEquipmentInventorySubsystem>();
		}
	}

	return nullptr;
}

void UETEquipmentInventorySubsystem::Initialize(FSubsystemCollectionBase& Collection) {
	const UETEquipmentInventorySettings* Settings = GetDefault<UETEquipmentInventorySettings>();
	if (Settings) {
		ItemCombinationDataTable = Settings->ItemCombinationDataTable;
	}
}