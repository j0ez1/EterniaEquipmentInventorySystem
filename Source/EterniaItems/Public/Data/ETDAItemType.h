// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ETDAItemType.generated.h"

class UETDAEquipmentSlotType;
/**
 * 
 */
UCLASS(DisplayName="[Eternia] Item Type", BlueprintType)
class ETERNIAITEMS_API UETDAItemType : public UDataAsset {
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	FORCEINLINE TArray<TSoftObjectPtr<UETDAEquipmentSlotType>> GetValidEquipmentSlotTypes() const { return ValidEquipmentSlotTypes; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE TArray<TSoftObjectPtr<UETDAEquipmentSlotType>> GetBlocksEquipmentSlotTypes() const { return BlocksEquipmentSlotTypes; }

	bool operator==(const UETDAItemType& Other) const { return Identifier == Other.Identifier; }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Identifier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName CategoryID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName="Valid Equipment Slot Types")
	TArray<TSoftObjectPtr<UETDAEquipmentSlotType>> ValidEquipmentSlotTypes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName="Blocks Equipment Slot Types")
	TArray<TSoftObjectPtr<UETDAEquipmentSlotType>> BlocksEquipmentSlotTypes;

};
