// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ETInventoryComponent.generated.h"


class UETInventoryComponent;
class UEterniaItemDatabase;
class UEterniaInventoryEntry;
class UEterniaInventoryWeaponDefinition;
class UEterniaInventoryItemDefinition;

// This class does not need to be modified.
UINTERFACE()
class ETERNIAEQUIPMENTINVENTORY_API UInventoryInterface : public UInterface {
	GENERATED_BODY()
};

/**
 * 
 */
class ETERNIAEQUIPMENTINVENTORY_API IInventoryInterface {
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Inventory")
	UETInventoryComponent* GetInventoryComponent() const;
};


DECLARE_LOG_CATEGORY_EXTERN(LogInventory, Log, All);

USTRUCT(BlueprintType)
struct FInventoryTile {
	GENERATED_BODY()

	FInventoryTile() = default;

	FInventoryTile(int32 TileX, int32 TileY): X(TileX), Y(TileY) {};

	UPROPERTY(BlueprintReadWrite)
	int32 X;

	UPROPERTY(BlueprintReadWrite)
	int32 Y;

	FIntPoint ToIntPoint() const {
		return FIntPoint(X, Y); 
	}
};

USTRUCT()
struct FInventoryItem {
	GENERATED_BODY()

	FInventoryItem() = default;

	UPROPERTY(EditAnywhere)
	FDataTableRowHandle Definition = FDataTableRowHandle();

	UPROPERTY(EditAnywhere, meta=(UIMin=1, ClampMin=1))
	int32 Amount = 1;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAddedDelegate, UEterniaInventoryEntry*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemUpdatedDelegate, UEterniaInventoryEntry*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemRemovedDelegate, UEterniaInventoryEntry*, Item);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMoneyCountChangedSignature, float);
DECLARE_MULTICAST_DELEGATE(FOnInventoryInitialized);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ETERNIAEQUIPMENTINVENTORY_API UETInventoryComponent : public UActorComponent {
	GENERATED_BODY()

public:

	UETInventoryComponent(const FObjectInitializer& ObjectInitializer);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable)
	bool TryAddItem(UEterniaInventoryEntry* ItemToAdd);

	bool TryAddItemAt(UEterniaInventoryEntry* ItemToAdd, const FInventoryTile& TopLeftTile);

	UFUNCTION(BlueprintCallable)
	bool RemoveItem(UEterniaInventoryEntry* EntryToRemove);

	UFUNCTION(BlueprintCallable)
	TMap<UEterniaInventoryEntry*, FInventoryTile> GetAllItems();

	// Entry to remove must exist in inventory
	void SwapItems(UEterniaInventoryEntry* EntryToRemove, UEterniaInventoryEntry* EntryToAdd);

	bool GetItemTopLeftTile(UEterniaInventoryEntry* Item, FInventoryTile& Tile);

	FOnItemAddedDelegate OnItemAdded;

	FOnItemUpdatedDelegate OnItemUpdated;

	FOnItemRemovedDelegate OnItemRemoved;

	FOnMoneyCountChangedSignature OnMoneyChanged;

	FOnInventoryInitialized OnInventoryInitialized;

	FORCEINLINE int32 GetMoney() const { return Money; }

	FORCEINLINE int32 GetRows() const { return Rows; }

	FORCEINLINE int32 GetColumns() const { return Columns; }

	UFUNCTION(BlueprintCallable)
	bool IsRoomAvailable(UEterniaInventoryEntry* ItemToCheck, int32 TopLeftIndex) const;

	bool IsRoomAvailable(UEterniaInventoryEntry* ItemToCheck, const FInventoryTile& TopLeftTile) const;

	static UEterniaInventoryEntry* CreateItemByDefinition(const FInventoryItem& ItemDef, UETInventoryComponent* OwningInventoryComponent);

	bool GetItemAtIndex(int32 Index, UEterniaInventoryEntry*& Item) const;

	bool GetItemAtTile(const FInventoryTile& Tile, UEterniaInventoryEntry*& Item) const;

protected:

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UEterniaInventoryEntry>> Inventory;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UEterniaInventoryEntry>> Items;

	// Items to be added to inventory on BeginPlay
	UPROPERTY(EditAnywhere)
	TArray<FInventoryItem> StartItems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Size", meta=(UIMin=1, ClampMin=1, UIMax=255, ClampMax=255))
	int32 Rows;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Size", meta=(UIMin=1, ClampMin=1, UIMax=255, ClampMax=255))
	int32 Columns;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Money;

	bool bIsDirty;

	virtual void BeginPlay() override;

	FInventoryTile IndexToTile(int32 Index) const;

	UFUNCTION(BlueprintCallable)
	int32 TileToIndex(const FInventoryTile& Tile) const;

	bool IsTileValid(const FInventoryTile& Tile) const;

	void AddItemAt(UEterniaInventoryEntry* Item, int32 TopLeftIndex);

	UFUNCTION()
	void OnItemAmountChanged(UEterniaInventoryEntry* UpdatedItem, int32 NewAmount);

	void AddItemAt(UEterniaInventoryEntry* Item, const FInventoryTile& TopLeftTile);

	void InitInventory();
};