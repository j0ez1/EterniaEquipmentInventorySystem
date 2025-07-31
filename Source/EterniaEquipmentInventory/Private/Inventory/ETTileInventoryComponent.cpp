// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ETTileInventoryComponent.h"

#include "Helpers/ETLogging.h"
#include "Inventory/ETInventoryEntry.h"
#include "Data/ETInventoryItemDefinition.h"

DEFINE_LOG_CATEGORY(LogInventory);

UETTileInventoryComponent::UETTileInventoryComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Rows(1)
	, Columns(1) {
	PrimaryComponentTick.bCanEverTick = false;
}

void UETTileInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);

	Inventory.Empty();
}

bool UETTileInventoryComponent::TryAddItem(UETInventoryEntry* ItemToAdd) {
	if (!ItemToAdd) {
		EEIS_ULOG_ERROR(TEXT("Inventory entry to add is null"))
		return false;
	}

	UETInventoryItemDefinition* Definition = ItemToAdd->GetDefinition();
	if (!Definition) {
		EEIS_ULOG_ERROR(TEXT("Inventory entry definition is null"))
		return false;
	}

	if (Inventory.Find(ItemToAdd) != INDEX_NONE) {
		return true;
	}

	// Find existing stackable item
	if (ItemToAdd->IsStackable()) {
		for (const TObjectPtr<UETInventoryEntry>& ExistingItem : Items) {
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

bool UETTileInventoryComponent::TryAddItemAt(UETInventoryEntry* ItemToAdd, const FInventoryTile& TopLeftTile) {
	if (!ItemToAdd) return false;

	bool bResult = false;
	if (IsRoomAvailable(ItemToAdd, TopLeftTile)) {
		UETInventoryEntry* ItemAtTile;
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
					UETInventoryComponentBase* OwningInventoryComponent = ItemToAdd->GetOwningInventoryComponent();
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

bool UETTileInventoryComponent::RemoveItem(UETInventoryEntry* EntryToRemove) {
	bool bItemRemoved = false;
	if (EntryToRemove) {
		for (int i = 0; i < Inventory.Num(); ++i) {
			TObjectPtr<UETInventoryEntry> ExistingEntry = Inventory[i];
			if (ExistingEntry == EntryToRemove) {
				ExistingEntry->OnItemAmountChanged.RemoveDynamic(this, &UETTileInventoryComponent::OnItemAmountChanged);
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

TMap<UETInventoryEntry*, FInventoryTile> UETTileInventoryComponent::GetAllItems() const {
	TMap<UETInventoryEntry*, FInventoryTile> AllItems;
	for (int i = 0; i < Inventory.Num(); ++i) {
		UETInventoryEntry* Item = Inventory[i];
		if (Item && !AllItems.Contains(Item)) {
			AllItems.Add(Item, IndexToTile(i));
		}
	}
	return AllItems;
}

bool UETTileInventoryComponent::GetItemTopLeftTile(UETInventoryEntry* Item, FInventoryTile& Tile) const {
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

void UETTileInventoryComponent::BeginPlay() {
	Inventory.Init(nullptr, Rows * Columns);
	
	Super::BeginPlay();
}

bool UETTileInventoryComponent::IsRoomAvailable(UETInventoryEntry* ItemToCheck, int32 TopLeftIndex) const {
	return IsRoomAvailable(ItemToCheck, IndexToTile(TopLeftIndex));
}

bool UETTileInventoryComponent::IsRoomAvailable(UETInventoryEntry* ItemToCheck, const FInventoryTile& TopLeftTile) const {
	if (!ItemToCheck) {
		return false;
	}

	int32 TilesX = ItemToCheck->GetDimensions().X;
	int32 TilesY = ItemToCheck->GetDimensions().Y;
	for (int32 X = TopLeftTile.X; X < TopLeftTile.X + TilesX; X++) {
		for (int32 Y = TopLeftTile.Y; Y < TopLeftTile.Y + TilesY; Y++) {
			FInventoryTile CurrentTile(X, Y);
			if (IsTileValid(CurrentTile)) {
				UETInventoryEntry* ItemAtTile;
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

FInventoryTile UETTileInventoryComponent::IndexToTile(int32 Index) const {
	return FInventoryTile(Index % Columns, Index / Columns);
}

int32 UETTileInventoryComponent::TileToIndex(const FInventoryTile& Tile) const {
	return Tile.X + Tile.Y * Columns;
}

bool UETTileInventoryComponent::IsTileValid(const FInventoryTile& Tile) const {
	return Tile.X >= 0 && Tile.Y >= 0 && Tile.X < Columns && Tile.Y < Rows;
}

bool UETTileInventoryComponent::GetItemAtIndex(int32 Index, UETInventoryEntry*& Item) const {
	if (Inventory.IsValidIndex(Index)) {
		Item = Inventory[Index].Get();
		return true;
	}
	return false;
}

bool UETTileInventoryComponent::GetItemAtTile(const FInventoryTile& Tile, UETInventoryEntry*& Item) const {
	return GetItemAtIndex(TileToIndex(Tile), Item);
}

void UETTileInventoryComponent::AddItemAt(UETInventoryEntry* Item, int32 TopLeftIndex) {
	AddItemAt(Item, IndexToTile(TopLeftIndex));
}

void UETTileInventoryComponent::OnItemAmountChanged(UETInventoryEntry* UpdatedItem, int32 NewAmount) {
	if (NewAmount <= 0) {
		RemoveItem(UpdatedItem);
	}
}

void UETTileInventoryComponent::AddItemAt(UETInventoryEntry* Item, const FInventoryTile& TopLeftTile) {
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
		OnItemAdded.Broadcast(Item);
		Item->OnItemAmountChanged.AddUniqueDynamic(this, &UETTileInventoryComponent::OnItemAmountChanged);
	}
}
