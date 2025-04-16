// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ETInventoryStatics.generated.h"

class UETInventoryComponent;
class UEterniaInventoryEntry;
class UEterniaInventoryItemDefinition;
/**
 * 
 */
UCLASS()
class ETERNIAEQUIPMENTINVENTORY_API UETInventoryStatics : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="Eternia|Items")
	static UEterniaInventoryItemDefinition* FindItemDefinitionByRepresentation(UObject* WorldContextObject, UClass* ItemClass);

	UFUNCTION(BlueprintCallable, Category="Eternia|Items")
	static UEterniaInventoryItemDefinition* FindItemDefinitionByID(UObject* WorldContextObject, FName ItemID);

	UFUNCTION(BlueprintCallable, Category="Eternia|Items")
	static UEterniaInventoryEntry* CreateItemByDefinition(UEterniaInventoryItemDefinition* Definition, UETInventoryComponent* OwningInventoryComponent, int32 Amount = 1);

private:

	static UDataTable* GetItemDataTable(UObject* WorldContextObject);
};
