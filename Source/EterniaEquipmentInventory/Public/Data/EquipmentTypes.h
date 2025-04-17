// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EquipmentTypes.generated.h"

USTRUCT(BlueprintType)
struct FETEquipmentSlot : public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Identifier = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayName;

	bool operator==(const FETEquipmentSlot& Other) const { return Identifier == Other.Identifier; }
};

USTRUCT(BlueprintType)
struct FETItemType : public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Identifier = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName CategoryID = NAME_None;

	TArray<FETEquipmentSlot> GetValidEquipmentSlotTypes() const;

	TArray<FETEquipmentSlot> GetBlocksEquipmentSlotTypes() const;

	bool operator==(const FETItemType& Other) const { return Identifier == Other.Identifier; }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (RequiredAssetDataTags = "RowStructure=/Script/EterniaEquipmentInventory.ETEquipmentSlot"))
	TArray<FDataTableRowHandle> ValidEquipmentSlotTypes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (RequiredAssetDataTags = "RowStructure=/Script/EterniaEquipmentInventory.ETEquipmentSlot"))
	TArray<FDataTableRowHandle> BlocksEquipmentSlotTypes;

	static TArray<FETEquipmentSlot> UnpackDataTableRowArray(const TArray<FDataTableRowHandle>& Array);
};
