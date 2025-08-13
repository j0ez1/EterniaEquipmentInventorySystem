// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ETDAEquipmentSlotType.generated.h"

/**
 * 
 */
UCLASS(DisplayName="[Eternia] Equipment Slot Type")
class ETERNIAEQUIPMENTINVENTORY_API UETDAEquipmentSlotType : public UDataAsset {
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	FGuid GetIdentifier() const { return Identifier; }

	UFUNCTION(BlueprintCallable)
	FText GetDisplayName() const { return DisplayName; }

	bool operator==(const UETDAEquipmentSlotType& Other) const { return Identifier == Other.Identifier; }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGuid Identifier = FGuid::NewGuid();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayName;
};
