// Fill out your copyright notice in the Description page of Project Settings.


#include "ETEquipmentInventorySubsystem.h"

#include "ETEquipmentInventorySettings.h"
#include "Helpers/ETLogging.h"

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
		ItemDatabase = Settings->ItemDatabase;
		ItemCombinationDataTable = Settings->ItemCombinationDataTable;
	}
}

UEterniaInventoryItemDefinition* UETEquipmentInventorySubsystem::FindItemDefinitionById(FName ItemID) {
	if (!ItemDatabase || !ItemDatabase.LoadSynchronous()) {
		EEIS_ULOGS_ERROR(TEXT("Item DataTable is null"))
		return nullptr;
	}

	TArray<FEtItemDefinition*> OutRowArray;
	ItemDatabase->GetAllRows<FEtItemDefinition>("", OutRowArray);
	for (const FEtItemDefinition* Definition : OutRowArray) {
		if (Definition && Definition->ItemID == ItemID) {
			return UEterniaInventoryItemDefinition::Convert(*Definition);
		}
	}
	return nullptr;
}
