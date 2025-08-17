// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "Data/ETDAItemDefinition.h"
#include "UObject/Object.h"
#include "ETItem.generated.h"

class UETInventoryComponentBase;
class IAbilitySystemInterface;
class UETItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemAmountChangedDelegate, UETItem*, UpdatedItem, int32, NewAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemRotated_InventoryEntry, UETItem*, Item);

/**
 * 
 */
UCLASS(BlueprintType)
class ETERNIAEQUIPMENTINVENTORY_API UETItem : public UObject {
	GENERATED_BODY()

public:

	UETItem(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UETDAItemDefinition* GetDefinition() const { return Definition; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetAmount() const { return Amount; }

	void IncrementAmount(int32 AmountToAdd);

	void SetDefinition(UETDAItemDefinition* Definition);

	void SetAmount(int32 Am);

	UPROPERTY(BlueprintAssignable)
	FOnItemAmountChangedDelegate OnItemAmountChanged;

	UFUNCTION(BlueprintCallable)
	FIntPoint GetDimensions();

	UFUNCTION(BlueprintCallable)
	UMaterialInterface* GetImage();

	UFUNCTION(BlueprintCallable)
	void Rotate();

	UPROPERTY(BlueprintAssignable)
	FOnItemRotated_InventoryEntry OnItemRotated;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsRotated() const { return Rotated; }

	FORCEINLINE UETInventoryComponentBase* GetOwningInventoryComponent() const { return OwningInventoryComponent; }

	void SetOwningInventoryComponent(UETInventoryComponentBase* InInventoryComponent);

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsStackable() const { return Definition && Definition->IsStackable(); }

	UFUNCTION(BlueprintPure)
	bool IsSameItem(UETItem* Item) const;

	UFUNCTION(BlueprintPure)
	bool IsStackFull() const;

	UFUNCTION(BlueprintPure)
	int32 GetStackLimit() const;

	UFUNCTION(BlueprintCallable)
	void Activate(AActor* ActivatorActor);

protected:

	UPROPERTY(BlueprintReadOnly)
	UETDAItemDefinition* Definition;

	UPROPERTY(BlueprintReadOnly)
	int32 Amount;

	UPROPERTY(BlueprintReadOnly)
	bool Rotated;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UETInventoryComponentBase> OwningInventoryComponent;

	FActiveGameplayEffectHandle InInventoryEffectHandle;

	static UAbilitySystemComponent* FindAbilitySystemComponent(UETInventoryComponentBase* InventoryComponent);
};
