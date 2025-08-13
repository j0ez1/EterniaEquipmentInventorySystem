// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ETInventoryComponentBase.h"

#include "Inventory/ETInventoryEntry.h"
#include "Inventory/ETInventoryStatics.h"
#include "Net/UnrealNetwork.h"


UETInventoryComponentBase::UETInventoryComponentBase(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer) {
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UETInventoryComponentBase::SwapItems(UETInventoryEntry* EntryToRemove, UETInventoryEntry* EntryToAdd) {
	if (EntryToRemove && EntryToRemove->GetOwningInventoryComponent() && TryAddItem(EntryToAdd)) {
		EntryToRemove->GetOwningInventoryComponent()->RemoveItem(EntryToRemove);
	}
}

void UETInventoryComponentBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UETInventoryComponentBase, Items, COND_None, REPNOTIFY_Always);
}

void UETInventoryComponentBase::BeginPlay() {
	Super::BeginPlay();

	InitInventory();
}

void UETInventoryComponentBase::InitInventory() {
	for (FInventoryItem Item : StartItems) {
		UETInventoryEntry* NewItem = UETInventoryStatics::CreateItemByDefinition(Item.Definition.LoadSynchronous(), this, Item.Amount);
		TryAddItem(NewItem);
	}

	// Workaround for Inventory UI correct drawing
	OnInventoryInitialized.Broadcast();
}