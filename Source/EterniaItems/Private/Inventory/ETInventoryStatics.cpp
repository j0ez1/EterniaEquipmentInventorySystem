// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ETInventoryStatics.h"

#include "Data/ETDAItemDefinition.h"
#include "Inventory/ETInventoryEntry.h"
#include "Util/ETEISStatics.h"

UETDAItemDefinition* UETInventoryStatics::FindItemDefinitionByRepresentation(UClass* ItemClass) {
	TArray<UETDAItemDefinition*> ItemDefinitions;
	UETEISStatics::GetObjectsOfClass<UETDAItemDefinition>(ItemDefinitions);
	for (const FAssetData& Data : ItemDefinitions) {
		if (UETDAItemDefinition* DataAsset = Cast<UETDAItemDefinition>(Data.GetAsset())) {
			if (DataAsset->GetRepresentation() == ItemClass) {
				return DataAsset;
			}
		}
	}

	return nullptr;
}

UETDAItemDefinition* UETInventoryStatics::FindItemDefinitionByID(FName ItemID) {
	TArray<UETDAItemDefinition*> ItemDefinitions;
	UETEISStatics::GetObjectsOfClass<UETDAItemDefinition>(ItemDefinitions);
	for (const FAssetData& Data : ItemDefinitions) {
		if (UETDAItemDefinition* DataAsset = Cast<UETDAItemDefinition>(Data.GetAsset())) {
			if (DataAsset->GetItemID() == ItemID) {
				return DataAsset;
			}
		}
	}
	
	return nullptr;
}

UETItem* UETInventoryStatics::CreateItemByDefinition(UETDAItemDefinition* Definition,
                                                               UETInventoryComponentBase* OwningInventoryComponent,
                                                               int32 Amount) {
	if (Definition) {
		UETItem* NewItem = NewObject<UETItem>(OwningInventoryComponent);
		NewItem->SetAmount(Amount);
		NewItem->SetDefinition(Definition);
		NewItem->SetOwningInventoryComponent(OwningInventoryComponent);
		return NewItem;
	}
	return nullptr;
}