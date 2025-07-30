// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/EquipmentTypes.h"

TArray<FETEquipmentSlotType> FETItemType::GetValidEquipmentSlotTypes() const {
	return UnpackDataTableRowArray(ValidEquipmentSlotTypes);
}

TArray<FETEquipmentSlotType> FETItemType::GetBlocksEquipmentSlotTypes() const {
	return UnpackDataTableRowArray(BlocksEquipmentSlotTypes);
}

TArray<FETEquipmentSlotType> FETItemType::UnpackDataTableRowArray(const TArray<FDataTableRowHandle>& Array) {
	TArray<FETEquipmentSlotType> ResultArray;
	for (FDataTableRowHandle RowHandle : Array) {
		FETEquipmentSlotType* SlotType = RowHandle.GetRow<FETEquipmentSlotType>("");
		if (SlotType) {
			ResultArray.AddUnique(*SlotType);
		}
	}
	return ResultArray;
}
