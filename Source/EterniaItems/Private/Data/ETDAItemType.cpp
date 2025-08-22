// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ETDAItemType.h"

TArray<TSoftObjectPtr<UETDAEquipmentSlotType>> UETDAItemType::GetValidEquipmentSlotTypes() const {
	return ValidEquipmentSlotTypes;
}

TArray<TSoftObjectPtr<UETDAEquipmentSlotType>> UETDAItemType::GetBlocksEquipmentSlotTypes() const {
	return BlocksEquipmentSlotTypes;
}
