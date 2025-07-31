// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ETInventoryComponentBase.h"

#include "Data/ETInventoryItemDefinition.h"
#include "Inventory/ETInventoryEntry.h"
#include "Inventory/ETInventoryStatics.h"
#include "Net/UnrealNetwork.h"


class UETInventoryItemDefinition;
struct FEtItemDefinition;

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
		UETInventoryEntry* NewItem = CreateItemByDefinition(Item, this);
		TryAddItem(NewItem);
	}

	// Workaround for Inventory UI correct drawing
	OnInventoryInitialized.Broadcast();
}

UETInventoryEntry* UETInventoryComponentBase::CreateItemByDefinition(const FInventoryItem& ItemDef,
                                                                     UETInventoryComponentBase* OwningInventoryComponent) {
	FEtItemDefinition* FoundItemDef = ItemDef.Definition.GetRow<FEtItemDefinition>("");
	if (FoundItemDef) {
		UETInventoryItemDefinition* Definition = UETInventoryItemDefinition::Convert(*FoundItemDef);
		return UETInventoryStatics::CreateItemByDefinition(Definition, OwningInventoryComponent, ItemDef.Amount);
	}
	return nullptr;
}
