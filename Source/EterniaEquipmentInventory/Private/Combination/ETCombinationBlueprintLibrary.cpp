// Fill out your copyright notice in the Description page of Project Settings.


#include "Combination/ETCombinationBlueprintLibrary.h"

#include "ETEquipmentInventorySubsystem.h"
#include "Inventory/EterniaInventoryEntry.h"

bool UETCombinationBlueprintLibrary::CanCombineItems(UObject* WorldContextObject,
                                                     UEterniaInventoryEntry* InitiatorItem,
                                                     UEterniaInventoryEntry* ItemToCombineWith,
                                                     FCombinationResult& CombinationResult) {
	if (InitiatorItem != nullptr && ItemToCombineWith != nullptr) {
		const FName CombinatorId = InitiatorItem->GetDefinition()->GetItemID();
		const FName CombineableId = ItemToCombineWith->GetDefinition()->GetItemID();
		return CanCombine(WorldContextObject, CombinatorId, CombineableId, CombinationResult);
	}
	return false;
}

bool UETCombinationBlueprintLibrary::CanCombine(UObject* WorldContextObject,
                                                FName CombinatorId,
                                                FName CombineableId,
                                                FCombinationResult& CombinationResult) {
	const UDataTable* CombinationDataTable = GetItemCombinationDataTable(WorldContextObject);
	if (CombinationDataTable) {
		TArray<FItemCombinationTableRow*> CombinationRows;
		CombinationDataTable->GetAllRows<FItemCombinationTableRow>("", CombinationRows);
		for (const FItemCombinationTableRow* Row : CombinationRows) {
			if (Row->CombinableId == CombinatorId) {
				TMap<FName, FCombinationResult> CombinationResults = Row->CombinationResults;
				const FCombinationResult* Result = CombinationResults.Find(CombineableId);
				if (Result) {
					CombinationResult = *Result;
				}
				return Result != nullptr;
			}
			if (Row->CombinableId == CombineableId) {
				TMap<FName, FCombinationResult> CombinationResults = Row->CombinationResults;
				const FCombinationResult* Result = CombinationResults.Find(CombinatorId);
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
	const UETEquipmentInventorySubsystem* Subsystem = UETEquipmentInventorySubsystem::GetCurrent(WorldContextObject);
	return Subsystem ? Subsystem->GetItemCombinationDataTable() : nullptr;
}
