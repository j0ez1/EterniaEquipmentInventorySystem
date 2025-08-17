// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ETInventoryComponentBase.h"
#include "ETTileInventoryComponent.generated.h"


class UETTileInventoryComponent;
class UEterniaItemDatabase;
class UETItem;

USTRUCT(BlueprintType)
struct FInventoryTile {
	GENERATED_BODY()

	FInventoryTile() = default;

	FInventoryTile(int32 TileX, int32 TileY)
		: X(TileX)
		, Y(TileY) {};

	UPROPERTY(BlueprintReadWrite)
	int32 X;

	UPROPERTY(BlueprintReadWrite)
	int32 Y;

	FIntPoint ToIntPoint() const {
		return FIntPoint(X, Y);
	}
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMoneyCountChangedSignature, float);

UCLASS(ClassGroup=(Eternia), DisplayName="[Eternia] Tile Inventory Component", meta=(BlueprintSpawnableComponent))
class ETERNIAEQUIPMENTINVENTORY_API UETTileInventoryComponent : public UETInventoryComponentBase {
	GENERATED_BODY()

public:

	UETTileInventoryComponent(const FObjectInitializer& ObjectInitializer);

	bool TryAddItemAt(UETItem* ItemToAdd, const FInventoryTile& TopLeftTile);

	UFUNCTION(BlueprintCallable)
	TMap<UETItem*, FInventoryTile> GetAllItems() const;

	bool GetItemTopLeftTile(UETItem* Item, FInventoryTile& Tile) const;

	UFUNCTION(BlueprintCallable)
	bool IsRoomAvailable(UETItem* ItemToCheck, int32 TopLeftIndex) const;

	bool IsRoomAvailable(UETItem* ItemToCheck, const FInventoryTile& TopLeftTile) const;

	bool GetItemAtIndex(int32 Index, UETItem*& Item) const;

	bool GetItemAtTile(const FInventoryTile& Tile, UETItem*& Item) const;

	FOnMoneyCountChangedSignature OnMoneyChanged;

#pragma region Getters

	FORCEINLINE int32 GetRows() const { return Rows; }

	FORCEINLINE int32 GetColumns() const { return Columns; }

#pragma endregion

#pragma region UETInventoryComponentBase

	virtual bool TryAddItem(UETItem* ItemToAdd) override;

	virtual bool RemoveItem(UETItem* EntryToRemove) override;

#pragma endregion

#pragma region UActorComponent

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

#pragma endregion

protected:

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, SaveGame)
	TArray<TObjectPtr<UETItem>> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, SaveGame, Category="Size", meta=(UIMin=1, ClampMin=1, UIMax=255, ClampMax=255))
	int32 Rows;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, SaveGame, Category="Size", meta=(UIMin=1, ClampMin=1, UIMax=255, ClampMax=255))
	int32 Columns;

	FInventoryTile IndexToTile(int32 Index) const;

	UFUNCTION(BlueprintCallable)
	int32 TileToIndex(const FInventoryTile& Tile) const;

	bool IsTileValid(const FInventoryTile& Tile) const;

	void AddItemAt(UETItem* Item, int32 TopLeftIndex);

	UFUNCTION()
	void OnItemAmountChanged(UETItem* UpdatedItem, int32 NewAmount);

	void AddItemAt(UETItem* Item, const FInventoryTile& TopLeftTile);

#pragma region UActorComponent

	virtual void BeginPlay() override;

#pragma endregion
};
