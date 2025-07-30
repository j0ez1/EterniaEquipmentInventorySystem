// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ETInventoryStatics.h"

#include "ETEquipmentInventorySubsystem.h"
#include "Helpers/ETLogging.h"
#include "Inventory/ETInventoryEntry.h"
#include "Data/ETInventoryItemDefinition.h"
#include "Inventory/ETInventoryComponent.h"

UETInventoryItemDefinition* UETInventoryStatics::FindItemDefinitionByRepresentation(UObject* WorldContextObject, UClass* ItemClass) {
	UDataTable* ItemDataTable = GetItemDataTable(WorldContextObject);
	if (ItemDataTable) {
		TArray<FEtItemDefinition*> OutRowArray;
		ItemDataTable->GetAllRows<FEtItemDefinition>("", OutRowArray);
		for (FEtItemDefinition* Definition : OutRowArray) {
			if (Definition && Definition->Representation == ItemClass) {
				return UETInventoryItemDefinition::Convert(*Definition);
			}
		}
	} else {
		EEIS_ULOGS_ERROR(TEXT("Item DataTable is null"))
	}
	return nullptr;
}

UETInventoryItemDefinition* UETInventoryStatics::FindItemDefinitionByID(UObject* WorldContextObject, FName ItemID) {
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (World) {
		UGameInstance* GameInstance = World->GetGameInstance();
		if (GameInstance) {
			if (UETEquipmentInventorySubsystem* Subsystem = GameInstance->GetSubsystem<UETEquipmentInventorySubsystem>()) {
				return Subsystem->FindItemDefinitionById(ItemID);
			}
		}
	}
	return nullptr;
}

UETInventoryEntry* UETInventoryStatics::CreateItemByDefinition(UETInventoryItemDefinition* Definition,
                                                                    UETInventoryComponentBase* OwningInventoryComponent,
                                                                    int32 Amount) {
	if (Definition) {
		UETInventoryEntry* NewItem = NewObject<UETInventoryEntry>(OwningInventoryComponent);
		NewItem->SetAmount(Amount);
		NewItem->SetDefinition(Definition);
		NewItem->SetOwningInventoryComponent(OwningInventoryComponent);
		return NewItem;
	}
	return nullptr;
}

UDataTable* UETInventoryStatics::GetItemDataTable(UObject* WorldContextObject) {
	UETEquipmentInventorySubsystem* Subsystem = UETEquipmentInventorySubsystem::GetCurrent(WorldContextObject);
	return Subsystem ? Subsystem->GetItemDatabase() : nullptr;
}
