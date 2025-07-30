// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ETInventoryComponentBase.h"
#include "ETInventoryComponent.generated.h"


class UETInventoryComponent;
class UEterniaItemDatabase;
class UETInventoryEntry;
class UETInventoryItemDefinition;

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

UCLASS(ClassGroup=(Eternia), meta=(BlueprintSpawnableComponent))
class ETERNIAEQUIPMENTINVENTORY_API UETInventoryComponent : public UETInventoryComponentBase {
	GENERATED_BODY()

public:

	UETInventoryComponent(const FObjectInitializer& ObjectInitializer);

	bool TryAddItemAt(UETInventoryEntry* ItemToAdd, const FInventoryTile& TopLeftTile);

	UFUNCTION(BlueprintCallable)
	TMap<UETInventoryEntry*, FInventoryTile> GetAllItems() const;

	bool GetItemTopLeftTile(UETInventoryEntry* Item, FInventoryTile& Tile) const;

	FOnMoneyCountChangedSignature OnMoneyChanged;

	UFUNCTION(BlueprintCallable)
	bool IsRoomAvailable(UETInventoryEntry* ItemToCheck, int32 TopLeftIndex) const;

	bool IsRoomAvailable(UETInventoryEntry* ItemToCheck, const FInventoryTile& TopLeftTile) const;

	bool GetItemAtIndex(int32 Index, UETInventoryEntry*& Item) const;

	bool GetItemAtTile(const FInventoryTile& Tile, UETInventoryEntry*& Item) const;

#pragma region Getters

	FORCEINLINE int32 GetMoney() const { return Money; }

	FORCEINLINE int32 GetRows() const { return Rows; }

	FORCEINLINE int32 GetColumns() const { return Columns; }

#pragma endregion

#pragma region UETInventoryComponentBase

	virtual bool TryAddItem(UETInventoryEntry* ItemToAdd) override;

	virtual bool RemoveItem(UETInventoryEntry* EntryToRemove) override;

#pragma endregion

#pragma region UActorComponent

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

#pragma endregion

protected:

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UETInventoryEntry>> Inventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Size", meta=(UIMin=1, ClampMin=1, UIMax=255, ClampMax=255))
	int32 Rows;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Size", meta=(UIMin=1, ClampMin=1, UIMax=255, ClampMax=255))
	int32 Columns;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Money;

	FInventoryTile IndexToTile(int32 Index) const;

	UFUNCTION(BlueprintCallable)
	int32 TileToIndex(const FInventoryTile& Tile) const;

	bool IsTileValid(const FInventoryTile& Tile) const;

	void AddItemAt(UETInventoryEntry* Item, int32 TopLeftIndex);

	UFUNCTION()
	void OnItemAmountChanged(UETInventoryEntry* UpdatedItem, int32 NewAmount);

	void AddItemAt(UETInventoryEntry* Item, const FInventoryTile& TopLeftTile);

#pragma region UActorComponent

	virtual void BeginPlay() override;

#pragma endregion
};
