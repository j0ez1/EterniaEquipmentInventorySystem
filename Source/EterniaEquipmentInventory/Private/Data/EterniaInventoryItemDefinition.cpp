// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/EterniaInventoryItemDefinition.h"

UEterniaInventoryItemDefinition::UEterniaInventoryItemDefinition() {
}

FETItemType UEterniaInventoryItemDefinition::GetItemType() const {
	FETItemType* Row = ItemTypeRowHandle.GetRow<FETItemType>("");
	if (Row) {
		return *Row;
	}
	return FETItemType();
}
