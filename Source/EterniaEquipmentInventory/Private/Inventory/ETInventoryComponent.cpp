// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ETInventoryComponent.h"

#include "Helpers/ETLogging.h"
#include "Inventory/EterniaInventoryEntry.h"
#include "Data/EterniaInventoryItemDefinition.h"
#include "Inventory/ETInventoryStatics.h"

DEFINE_LOG_CATEGORY(LogInventory);

UETInventoryComponent::UETInventoryComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Rows(1)
	, Columns(1)
	, Money(0)
	, bIsDirty(false) {
	PrimaryComponentTick.bCanEverTick = false;
}

void UETInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);

	Inventory.Empty();
}

bool UETInventoryComponent::TryAddItem(UEterniaInventoryEntry* ItemToAdd) {
	if (!ItemToAdd) {
		EEIS_ULOG_ERROR(TEXT("Inventory entry to add is null"))
		return false;
	}

	UEterniaInventoryItemDefinition* Definition = ItemToAdd->GetDefinition();
	if (!Definition) {
		EEIS_ULOG_ERROR(TEXT("Inventory entry definition is null"))
		return false;
	}

	if (Inventory.Find(ItemToAdd) != INDEX_NONE) {
		return true;
	}

	// Find existing stackable item
	if (ItemToAdd->IsStackable()) {
		for (const TObjectPtr<UEterniaInventoryEntry>& ExistingItem : Items) {
			if (ExistingItem && ExistingItem->IsSameItem(ItemToAdd)) {
				if (ExistingItem->GetAmount() < Definition->GetStackSize()) {
					ExistingItem->IncrementAmount(ItemToAdd->GetAmount());
					OnItemAdded.Broadcast(ExistingItem);
					return true;
				}
			}
		}
	}

	for (int i = 0; i < Inventory.Num(); i++) {
		if (IsRoomAvailable(ItemToAdd, i)) {
			AddItemAt(ItemToAdd, i);
			return true;
		}
	}
	ItemToAdd->Rotate();
	for (int i = 0; i < Inventory.Num(); i++) {
		if (IsRoomAvailable(ItemToAdd, i)) {
			AddItemAt(ItemToAdd, i);
			return true;
		}
	}
	return false;
}

bool UETInventoryComponent::TryAddItemAt(UEterniaInventoryEntry* ItemToAdd, const FInventoryTile& TopLeftTile) {
	if (!ItemToAdd) return false;

	bool bResult = false;
	if (IsRoomAvailable(ItemToAdd, TopLeftTile)) {
		UEterniaInventoryEntry* ItemAtTile;
		if (GetItemAtTile(TopLeftTile, ItemAtTile) && ItemAtTile) {
			if (ItemAtTile != ItemToAdd) {
				// Merging with the same item
				int32 CurrentStackLimit = ItemAtTile->GetStackLimit();
				if (ItemToAdd->GetAmount() <= CurrentStackLimit) {
					// Full merge
					ItemAtTile->SetAmount(ItemAtTile->GetAmount() + ItemToAdd->GetAmount());
					ItemToAdd->GetOwningInventoryComponent()->RemoveItem(ItemToAdd);
				} else {
					// Partial merge
					ItemAtTile->SetAmount(ItemAtTile->GetAmount() + CurrentStackLimit);
					ItemToAdd->SetAmount(ItemToAdd->GetAmount() - CurrentStackLimit);
					UETInventoryComponent* OwningInventoryComponent = ItemToAdd->GetOwningInventoryComponent();
					if (OwningInventoryComponent != this) {
						if (TryAddItem(ItemToAdd)) {
							OwningInventoryComponent->RemoveItem(ItemToAdd);
						}
					}
				}
				bResult = true;
			}
		} else {
			// Moving to free slots
			AddItemAt(ItemToAdd, TopLeftTile);
			bResult = true;
		}
	} else if (ItemToAdd->GetOwningInventoryComponent() != this) {
		// Item has moved from another inventory
		bResult = TryAddItem(ItemToAdd);
		if (bResult) {
			ItemToAdd->GetOwningInventoryComponent()->RemoveItem(ItemToAdd);
		}
	}
	return bResult;
}

bool UETInventoryComponent::RemoveItem(UEterniaInventoryEntry* EntryToRemove) {
	bool bItemRemoved = false;
	if (EntryToRemove) {
		for (int i = 0; i < Inventory.Num(); ++i) {
			TObjectPtr<UEterniaInventoryEntry> ExistingEntry = Inventory[i];
			if (ExistingEntry == EntryToRemove) {
				ExistingEntry->OnItemAmountChanged.RemoveDynamic(this, &UETInventoryComponent::OnItemAmountChanged);
				Inventory[i] = nullptr;
				Items.Remove(ExistingEntry);
				bItemRemoved = true;
			}
		}
		if (bItemRemoved) {
			OnItemRemoved.Broadcast(EntryToRemove);
		}
	}
	return bItemRemoved;
}

TMap<UEterniaInventoryEntry*, FInventoryTile> UETInventoryComponent::GetAllItems() {
	TMap<UEterniaInventoryEntry*, FInventoryTile> AllItems;
	for (int i = 0; i < Inventory.Num(); ++i) {
		UEterniaInventoryEntry* Item = Inventory[i];
		if (Item) {
			if (!AllItems.Contains(Item)) {
				AllItems.Add(Item, IndexToTile(i));
			}
		}
	}
	return AllItems;
}

void UETInventoryComponent::SwapItems(UEterniaInventoryEntry* EntryToRemove, UEterniaInventoryEntry* EntryToAdd) {
	if (EntryToRemove && EntryToRemove->GetOwningInventoryComponent() && TryAddItem(EntryToAdd)) {
		EntryToRemove->GetOwningInventoryComponent()->RemoveItem(EntryToRemove);
	}
}

void UETInventoryComponent::InitInventory() {
	Inventory.Init(nullptr, Rows * Columns);
	for (FInventoryItem Item : StartItems) {
		UEterniaInventoryEntry* NewItem = CreateItemByDefinition(Item, this);
		TryAddItem(NewItem);
	}

	// Workaround for Inventory UI correct drawing
	OnInventoryInitialized.Broadcast();
}

bool UETInventoryComponent::GetItemTopLeftTile(UEterniaInventoryEntry* Item, FInventoryTile& Tile) {
	for (int i = 0; i < Rows; ++i) {
		for (int j = 0; j < Columns; ++j) {
			if (Inventory[i * Columns + j] == Item) {
				Tile = FInventoryTile(j, i);
				return true;
			}
		}
	}
	return false;
}

void UETInventoryComponent::BeginPlay() {
	Super::BeginPlay();

	InitInventory();
}

bool UETInventoryComponent::IsRoomAvailable(UEterniaInventoryEntry* ItemToCheck, int32 TopLeftIndex) const {
	return IsRoomAvailable(ItemToCheck, IndexToTile(TopLeftIndex));
}

bool UETInventoryComponent::IsRoomAvailable(UEterniaInventoryEntry* ItemToCheck, const FInventoryTile& TopLeftTile) const {
	if (!ItemToCheck) {
		return false;
	}

	int32 TilesX = ItemToCheck->GetDimensions().X;
	int32 TilesY = ItemToCheck->GetDimensions().Y;
	for (int32 X = TopLeftTile.X; X < TopLeftTile.X + TilesX; X++) {
		for (int32 Y = TopLeftTile.Y; Y < TopLeftTile.Y + TilesY; Y++) {
			FInventoryTile CurrentTile(X, Y);
			if (IsTileValid(CurrentTile)) {
				UEterniaInventoryEntry* ItemAtTile;
				if (GetItemAtIndex(TileToIndex(CurrentTile), ItemAtTile)) {
					if (ItemAtTile) {
						bool bIsSameStackableItemWithAvailSpace = ItemAtTile->IsSameItem(ItemToCheck) && !ItemAtTile->IsStackFull();
						if (ItemAtTile != ItemToCheck && !bIsSameStackableItemWithAvailSpace) {
							return false;
						}
					}
				} else {
					return false;
				}
			} else {
				return false;
			}
		}
	}
	return true;
}

FInventoryTile UETInventoryComponent::IndexToTile(int32 Index) const {
	return FInventoryTile(Index % Columns, Index / Columns);
}

int32 UETInventoryComponent::TileToIndex(const FInventoryTile& Tile) const {
	return Tile.X + Tile.Y * Columns;
}

bool UETInventoryComponent::IsTileValid(const FInventoryTile& Tile) const {
	return Tile.X >= 0 && Tile.Y >= 0 && Tile.X < Columns && Tile.Y < Rows;
}

bool UETInventoryComponent::GetItemAtIndex(int32 Index, UEterniaInventoryEntry*& Item) const {
	if (Inventory.IsValidIndex(Index)) {
		Item = Inventory[Index].Get();
		return true;
	}
	return false;
}

bool UETInventoryComponent::GetItemAtTile(const FInventoryTile& Tile, UEterniaInventoryEntry*& Item) const {
	return GetItemAtIndex(TileToIndex(Tile), Item);
}

UEterniaInventoryEntry* UETInventoryComponent::CreateItemByDefinition(const FInventoryItem& ItemDef, UETInventoryComponent* OwningInventoryComponent) {
	FEtItemDefinition* FoundItemDef = ItemDef.Definition.GetRow<FEtItemDefinition>("");
	if (FoundItemDef) {
		UEterniaInventoryItemDefinition* Definition = UEterniaInventoryItemDefinition::Convert(*FoundItemDef);
		return UETInventoryStatics::CreateItemByDefinition(Definition, OwningInventoryComponent, ItemDef.Amount);
	}
	return nullptr;
}

void UETInventoryComponent::AddItemAt(UEterniaInventoryEntry* Item, int32 TopLeftIndex) {
	AddItemAt(Item, IndexToTile(TopLeftIndex));
}

void UETInventoryComponent::OnItemAmountChanged(UEterniaInventoryEntry* UpdatedItem, int32 NewAmount) {
	if (NewAmount <= 0) {
		RemoveItem(UpdatedItem);
	}
}

void UETInventoryComponent::AddItemAt(UEterniaInventoryEntry* Item, const FInventoryTile& TopLeftTile) {
	if (Item) {
		Items.Add(Item);
		Item->SetOwningInventoryComponent(this);
		int32 TilesX = Item->GetDimensions().X;
		int32 TilesY = Item->GetDimensions().Y;
		for (int32 X = TopLeftTile.X; X < TopLeftTile.X + TilesX; X++) {
			for (int32 Y = TopLeftTile.Y; Y < TopLeftTile.Y + TilesY; Y++) {
				FInventoryTile CurrentTile(X, Y);
				if (IsTileValid(CurrentTile)) {
					int32 Index = TileToIndex(CurrentTile);
					Inventory[Index] = Item;
				}
			}
		}
		bIsDirty = true;
		OnItemAdded.Broadcast(Item);
		Item->OnItemAmountChanged.AddUniqueDynamic(this, &UETInventoryComponent::OnItemAmountChanged);
	}
}
