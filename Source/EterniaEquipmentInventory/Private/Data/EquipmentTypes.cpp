// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/EquipmentTypes.h"

TArray<FETEquipmentSlot> FETItemType::GetValidEquipmentSlotTypes() const {
	return UnpackDataTableRowArray(ValidEquipmentSlotTypes);
}

TArray<FETEquipmentSlot> FETItemType::GetBlocksEquipmentSlotTypes() const {
	return UnpackDataTableRowArray(BlocksEquipmentSlotTypes);
}

TArray<FETEquipmentSlot> FETItemType::UnpackDataTableRowArray(const TArray<FDataTableRowHandle>& Array) {
	TArray<FETEquipmentSlot> ResultArray;
	for (FDataTableRowHandle RowHandle : Array) {
		FETEquipmentSlot* SlotType = RowHandle.GetRow<FETEquipmentSlot>("");
		if (SlotType) {
			ResultArray.AddUnique(*SlotType);
		}
	}
	return ResultArray;
}
