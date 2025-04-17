// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ETInventoryStatics.h"

#include "ETEquipmentInventorySubsystem.h"
#include "Helpers/ETLogging.h"
#include "Inventory/EterniaInventoryEntry.h"
#include "Data/EterniaInventoryItemDefinition.h"
#include "Inventory/ETInventoryComponent.h"

UEterniaInventoryItemDefinition* UETInventoryStatics::FindItemDefinitionByRepresentation(UObject* WorldContextObject, UClass* ItemClass) {
	UDataTable* ItemDataTable = GetItemDataTable(WorldContextObject);
	if (ItemDataTable) {
		TArray<FEtItemDefinition*> OutRowArray;
		ItemDataTable->GetAllRows<FEtItemDefinition>("", OutRowArray);
		for (FEtItemDefinition* Definition : OutRowArray) {
			if (Definition && Definition->Representation == ItemClass) {
				return UEterniaInventoryItemDefinition::Convert(*Definition);
			}
		}
	} else {
		EEIS_ULOGS_ERROR(TEXT("Item DataTable is null"))
	}
	return nullptr;
}

UEterniaInventoryItemDefinition* UETInventoryStatics::FindItemDefinitionByID(UObject* WorldContextObject, FName ItemID) {
	UDataTable* ItemDataTable = GetItemDataTable(WorldContextObject);
	if (ItemDataTable) {
		TArray<FEtItemDefinition*> OutRowArray;
		ItemDataTable->GetAllRows<FEtItemDefinition>("", OutRowArray);
		for (FEtItemDefinition* Definition : OutRowArray) {
			if (Definition && Definition->ItemID == ItemID) {
				return UEterniaInventoryItemDefinition::Convert(*Definition);
			}
		}
	} else {
		EEIS_ULOGS_ERROR(TEXT("Item DataTable is null"))
	}
	return nullptr;
}

UEterniaInventoryEntry* UETInventoryStatics::CreateItemByDefinition(UEterniaInventoryItemDefinition* Definition,
																		  UETInventoryComponent* OwningInventoryComponent,
																		  int32 Amount) {
	if (Definition) {
		UEterniaInventoryEntry* NewItem = NewObject<UEterniaInventoryEntry>(OwningInventoryComponent);
		NewItem->SetAmount(Amount);
		NewItem->SetDefinition(Definition);
		NewItem->SetOwningInventoryComponent(OwningInventoryComponent);
		return NewItem;
	}
	return nullptr;
}

UDataTable* UETInventoryStatics::GetItemDataTable(UObject* WorldContextObject) {
	UETEquipmentInventorySubsystem* Subsystem = UETEquipmentInventorySubsystem::GetCurrent(WorldContextObject);
	if (Subsystem) {
		return Subsystem->GetItemDatabase();
	}
	return nullptr;
}
