// Fill out your copyright notice in the Description page of Project Settings.


#include "Combination/ETCombinationBlueprintLibrary.h"

#include "ETEquipmentInventorySubsystem.h"
#include "Inventory/EterniaInventoryEntry.h"

bool UETCombinationBlueprintLibrary::CanCombineItems(UObject* WorldContextObject,
                                                     UEterniaInventoryEntry* InitiatorItem,
                                                     UEterniaInventoryEntry* ItemToCombineWith,
                                                     FCombinationResult& CombinationResult) {
	if (InitiatorItem != nullptr && ItemToCombineWith != nullptr) {
		return CanCombine(WorldContextObject, InitiatorItem->GetDefinition()->GetItemID(), ItemToCombineWith->GetDefinition()->GetItemID(), CombinationResult);
	}
	return false;
}

bool UETCombinationBlueprintLibrary::CanCombine(UObject* WorldContextObject,
                                                FName CombinatorId,
                                                FName CombineableId,
                                                FCombinationResult& CombinationResult) {
	UDataTable* CombinationDataTable = GetItemCombinationDataTable(WorldContextObject);
	if (CombinationDataTable) {
		TArray<FItemCombinationTableRow*> CombinationRows;
		CombinationDataTable->GetAllRows<FItemCombinationTableRow>("", CombinationRows);
		for (FItemCombinationTableRow* Row : CombinationRows) {
			if (Row->CombinableId == CombinatorId) {
				TMap<FName, FCombinationResult> CombinationResults = Row->CombinationResults;
				FCombinationResult* Result = CombinationResults.Find(CombineableId);
				if (Result) {
					CombinationResult = *Result;
				}
				return Result != nullptr;
			}
		}
	}
	return false;
}

UDataTable* UETCombinationBlueprintLibrary::GetItemCombinationDataTable(UObject* WorldContextObject) {
	UETEquipmentInventorySubsystem* Subsystem = UETEquipmentInventorySubsystem::GetCurrent(WorldContextObject);
	if (Subsystem) {
		return Subsystem->GetItemDatabase();
	}
	return nullptr;
}
