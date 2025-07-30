// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ETInventoryItemDefinition.h"

UETInventoryItemDefinition::UETInventoryItemDefinition() {
}

FETItemType UETInventoryItemDefinition::GetItemType() const {
	FETItemType* Row = ItemTypeRowHandle.GetRow<FETItemType>("");
	if (Row) {
		return *Row;
	}
	return FETItemType();
}
