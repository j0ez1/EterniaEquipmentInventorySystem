// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ETInventoryComponentBase.generated.h"


class UETDAItemDefinition;
class UETItem;

USTRUCT()
struct FInventoryItem {
	GENERATED_BODY()

	FInventoryItem() = default;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UETDAItemDefinition> Definition;

	UPROPERTY(EditAnywhere, meta=(UIMin=1, ClampMin=1))
	int32 Amount = 1;
};

DECLARE_LOG_CATEGORY_EXTERN(LogInventory, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAddedDelegate, UETItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemUpdatedDelegate, UETItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemRemovedDelegate, UETItem*, Item);
DECLARE_MULTICAST_DELEGATE(FOnInventoryInitialized);

UCLASS(Abstract, ClassGroup=(Eternia), DisplayName="[Eternia] Inventory Component Base", meta=(BlueprintSpawnableComponent))
class ETERNIAITEMS_API UETInventoryComponentBase : public UActorComponent {
	GENERATED_BODY()

public:

	UETInventoryComponentBase(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	virtual bool TryAddItem(UETItem* ItemToAdd) { return false; }

	UFUNCTION(BlueprintCallable)
	virtual bool RemoveItem(UETItem* EntryToRemove) { return false; }

	// Entry to remove must exist in inventory
	void SwapItems(UETItem* EntryToRemove, UETItem* EntryToAdd);

	UPROPERTY(BlueprintAssignable)
	FOnItemAddedDelegate OnItemAdded;

	UPROPERTY(BlueprintAssignable)
	FOnItemUpdatedDelegate OnItemUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnItemRemovedDelegate OnItemRemoved;

	FOnInventoryInitialized OnInventoryInitialized;

#pragma region UActorComponent

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#pragma endregion // UActorComponent

protected:

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, SaveGame, Replicated)
	TArray<TObjectPtr<UETItem>> Items;

	// Items to be added to inventory on BeginPlay
	UPROPERTY(EditAnywhere)
	TArray<FInventoryItem> StartItems;

	virtual void BeginPlay() override;

	void InitInventory();

};
