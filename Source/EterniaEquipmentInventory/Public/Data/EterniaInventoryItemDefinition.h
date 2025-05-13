// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentTypes.h"
#include "EterniaInventoryItemDefinition.generated.h"

class UGameplayEffect;

USTRUCT()
struct FEtItemDefinition : public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName ItemID;

	UPROPERTY(EditAnywhere)
	FText ItemName;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> Representation; // TODO Remove

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> DraggableRepresentation;

	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(EditAnywhere)
	int32 Value = 0;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Image = nullptr;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* ImageRotated = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(UIMin=1, ClampMin=1))
	FIntPoint Dimensions = FIntPoint(1, 1);

	UPROPERTY(EditAnywhere, meta=(InlineEditConditionToggle))
	bool Stackable = false;

	UPROPERTY(EditAnywhere, meta=(EditCondition = "Stackable"))
	int32 StackSize = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(RequiredAssetDataTags="RowStructure=/Script/EterniaEquipmentInventory.ETItemType"))
	FDataTableRowHandle ItemType;

	UPROPERTY(EditAnywhere)
	bool Consumable = false;

	UPROPERTY(EditAnywhere, meta=(EditCondition = "Consumable"))
	TSubclassOf<UGameplayEffect> ConsumeEffect;

	UPROPERTY(EditAnywhere, meta=(EditCondition = "Consumable"))
	FName ConsumeReplaceItemID;

	/*
	 * The effect activated when item is in inventory
	 */
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UGameplayEffect> InInventoryEffect;

};

/**
 * 
 */
UCLASS(BlueprintType, EditInlineNew)
class ETERNIAEQUIPMENTINVENTORY_API UEterniaInventoryItemDefinition : public UObject {
	GENERATED_BODY()

public:

	UEterniaInventoryItemDefinition();

	FORCEINLINE FName GetItemID() const { return ItemID; }

	FORCEINLINE FText GetItemName() const { return ItemName; }

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

	FETItemType GetItemType() const;

	FORCEINLINE bool IsConsumable() const { return Consumable; }

	FORCEINLINE TSubclassOf<UGameplayEffect> GetConsumeEffect() const { return ConsumeEffect; }

	FORCEINLINE FName GetConsumeReplaceItemID() const { return ConsumeReplaceItemID; }

	FORCEINLINE TSoftClassPtr<UGameplayEffect> GetInInventoryEffect() const { return InInventoryEffect; }

	static UEterniaInventoryItemDefinition* Convert(const FEtItemDefinition& DTDef) {
		UEterniaInventoryItemDefinition* Result = NewObject<UEterniaInventoryItemDefinition>();
		Result->ItemID = DTDef.ItemID;
		Result->ItemName = DTDef.ItemName;
		Result->ItemTypeRowHandle = DTDef.ItemType;
		Result->Representation = DTDef.Representation;
		Result->DraggableRepresentation = DTDef.DraggableRepresentation;
		Result->Description = DTDef.Description;
		Result->Value = DTDef.Value;
		Result->Image = DTDef.Image;
		Result->ImageRotated = DTDef.ImageRotated;
		Result->Dimensions = DTDef.Dimensions;
		Result->Stackable = DTDef.Stackable;
		Result->StackSize = DTDef.StackSize;
		Result->Consumable = DTDef.Consumable;
		Result->ConsumeEffect = DTDef.ConsumeEffect;
		Result->ConsumeReplaceItemID = DTDef.ConsumeReplaceItemID;
		Result->InInventoryEffect = DTDef.InInventoryEffect;
		return Result;
	}

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ItemName;

	UPROPERTY()
	FDataTableRowHandle ItemTypeRowHandle;

	UPROPERTY(EditAnywhere, BlueprintGetter=GetRepresentation)
	TSubclassOf<AActor> Representation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> DraggableRepresentation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;

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

	UPROPERTY(EditAnywhere)
	int32 StackSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool Consumable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> ConsumeEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ConsumeReplaceItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UGameplayEffect> InInventoryEffect;
};
