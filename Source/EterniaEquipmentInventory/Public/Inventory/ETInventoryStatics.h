// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ETInventoryComponentBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ETInventoryStatics.generated.h"

class UETInventoryComponent;
class UETInventoryEntry;
class UETInventoryItemDefinition;
/**
 * 
 */
UCLASS()
class ETERNIAEQUIPMENTINVENTORY_API UETInventoryStatics : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="Eternia|Items")
	static UETInventoryItemDefinition* FindItemDefinitionByRepresentation(UObject* WorldContextObject, UClass* ItemClass);

	UFUNCTION(BlueprintCallable, Category="Eternia|Items")
	static UETInventoryItemDefinition* FindItemDefinitionByID(UObject* WorldContextObject, FName ItemID);

	UFUNCTION(BlueprintCallable, Category="Eternia|Items")
	static UETInventoryEntry* CreateItemByDefinition(UETInventoryItemDefinition* Definition, UETInventoryComponentBase* OwningInventoryComponent, int32 Amount = 1);

private:

	static UDataTable* GetItemDataTable(UObject* WorldContextObject);
};
