// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ETCombinationBlueprintLibrary.generated.h"

class UEterniaInventoryEntry;

USTRUCT(BlueprintType)
struct FCombinationResult {
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ResultId;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName RemainderId;
};

USTRUCT()
struct FItemCombinationTableRow : public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName CombinableId;

	UPROPERTY(EditAnywhere)
	TMap<FName, FCombinationResult> CombinationResults;
};

UCLASS(NotBlueprintable)
class ETERNIAEQUIPMENTINVENTORY_API UETCombinationBlueprintLibrary : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static bool CanCombineItems(UObject* WorldContextObject, UEterniaInventoryEntry* InitiatorItem, UEterniaInventoryEntry* ItemToCombineWith, FCombinationResult& CombinationResult);

	UFUNCTION(BlueprintCallable)
	static bool CanCombine(UObject* WorldContextObject, FName CombinatorId, FName CombineableId, FCombinationResult& CombinationResult);

	UFUNCTION(BlueprintCallable)
	static UDataTable* GetItemCombinationDataTable(UObject* WorldContextObject);

};
