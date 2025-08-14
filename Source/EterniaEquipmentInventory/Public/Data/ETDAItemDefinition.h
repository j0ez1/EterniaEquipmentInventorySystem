// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ETDAItemDefinition.generated.h"

class UETDAItemType;
class UGameplayEffect;
/**
 * 
 */
UCLASS(DisplayName="[Eternia] Item Definition", BlueprintType)
class ETERNIAEQUIPMENTINVENTORY_API UETDAItemDefinition : public UDataAsset {
	GENERATED_BODY()

public:

	FORCEINLINE FName GetItemID() const { return ItemID; }

	FORCEINLINE FText GetItemName() const { return ItemName; }

	FORCEINLINE UETDAItemType* GetType() const { return ItemType.LoadSynchronous(); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE TSubclassOf<AActor> GetRepresentation() const { return Representation; }

	FORCEINLINE UStaticMesh* GetDraggableRepresentation() const { return DraggableRepresentation; }

	FORCEINLINE FText GetDescription() const { return Description; }

	FORCEINLINE int32 GetValue() const { return Value; }

	FORCEINLINE UMaterialInterface* GetImage() const { return Image; }

	FORCEINLINE UMaterialInterface* GetImageRotated() const { return ImageRotated; }

	FORCEINLINE FIntPoint GetDimensions() const { return Dimensions; }

	FORCEINLINE bool IsStackable() const { return Stackable; }

	FORCEINLINE int32 GetStackSize() const { return StackSize; }

	FORCEINLINE bool IsConsumable() const { return Consumable; }

	FORCEINLINE TSubclassOf<UGameplayEffect> GetConsumeEffect() const { return ConsumeEffect; }

	FORCEINLINE FName GetConsumeReplaceItemID() const { return ConsumeReplaceItemID; }

	FORCEINLINE TSoftClassPtr<UGameplayEffect> GetInInventoryEffect() const { return InInventoryEffect; }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName="Display Name")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UETDAItemType> ItemType;

	UPROPERTY(EditAnywhere, BlueprintGetter=GetRepresentation)
	TSubclassOf<AActor> Representation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> DraggableRepresentation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Value = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMaterialInterface* Image;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMaterialInterface* ImageRotated;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FIntPoint Dimensions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool Stackable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition=Stackable, EditConditionHides))
	int32 StackSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool Consumable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="Consumable", EditConditionHides))
	TSubclassOf<UGameplayEffect> ConsumeEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="Consumable", EditConditionHides))
	FName ConsumeReplaceItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UGameplayEffect> InInventoryEffect;
};
