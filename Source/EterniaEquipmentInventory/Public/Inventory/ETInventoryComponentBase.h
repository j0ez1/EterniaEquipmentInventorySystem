// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ETInventoryComponentBase.generated.h"


class UETInventoryEntry;

USTRUCT()
struct FInventoryItem {
	GENERATED_BODY()

	FInventoryItem() = default;

	UPROPERTY(EditAnywhere)
	FDataTableRowHandle Definition = FDataTableRowHandle();

	UPROPERTY(EditAnywhere, meta=(UIMin=1, ClampMin=1))
	int32 Amount = 1;
};

DECLARE_LOG_CATEGORY_EXTERN(LogInventory, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAddedDelegate, UETInventoryEntry*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemUpdatedDelegate, UETInventoryEntry*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemRemovedDelegate, UETInventoryEntry*, Item);
DECLARE_MULTICAST_DELEGATE(FOnInventoryInitialized);

UCLASS(Abstract, ClassGroup=(Eternia), meta=(BlueprintSpawnableComponent))
class ETERNIAEQUIPMENTINVENTORY_API UETInventoryComponentBase : public UActorComponent {
	GENERATED_BODY()

public:

	UETInventoryComponentBase(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	virtual bool TryAddItem(UETInventoryEntry* ItemToAdd) { return false; }

	UFUNCTION(BlueprintCallable)
	virtual bool RemoveItem(UETInventoryEntry* EntryToRemove) { return false; }

	// Entry to remove must exist in inventory
	void SwapItems(UETInventoryEntry* EntryToRemove, UETInventoryEntry* EntryToAdd);

	UPROPERTY(BlueprintAssignable)
	FOnItemAddedDelegate OnItemAdded;

	UPROPERTY(BlueprintAssignable)
	FOnItemUpdatedDelegate OnItemUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnItemRemovedDelegate OnItemRemoved;

	FOnInventoryInitialized OnInventoryInitialized;

protected:

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UETInventoryEntry>> Items;

	// Items to be added to inventory on BeginPlay
	UPROPERTY(EditAnywhere)
	TArray<FInventoryItem> StartItems;

	virtual void BeginPlay() override;

	void InitInventory();

	static UETInventoryEntry* CreateItemByDefinition(const FInventoryItem& ItemDef, UETInventoryComponentBase* OwningInventoryComponent);

};
