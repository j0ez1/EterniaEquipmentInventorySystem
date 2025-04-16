// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/EquipmentBlueprintLibrary.h"

EEquipType UEquipmentBlueprintLibrary::GetEquipType(const EEquipmentSlotType& SlotType) {
	switch (SlotType) {
		case EEST_RightHand: {
			return EET_Primary;
		}
		case EEST_LeftHand: {
			return EET_Aux;
		}
		default: return EET_MAX;
	}
}

EEquipmentSlotType UEquipmentBlueprintLibrary::GetSlotType(const EEquipType& EquipType) {
	switch (EquipType) {
		case EET_Primary:
			return EEST_RightHand;
		case EET_Aux:
			return EEST_LeftHand;
		case EET_TwoHanded:
			return EEST_RightHand; // FIXME
		case EET_Consumable:
			return EEST_Consumable;
		default:
			return EEST_MAX;
	}
}

TArray<TEnumAsByte<EEquipmentSlotType>> UEquipmentBlueprintLibrary::GetOccupiedSlotTypes(const EEquipType& EquipType) {
	TArray<TEnumAsByte<EEquipmentSlotType>> Result;
	switch (EquipType) {
		case EET_Primary:
			Result.Add(EEST_RightHand);
			break;
		case EET_Aux:
			Result.Add(EEST_LeftHand);
			break;
		case EET_TwoHanded:
			Result.Add(EEST_RightHand);
			Result.Add(EEST_LeftHand);
			break;
		case EET_Consumable:
			Result.Add(EEST_Consumable);
			break;
	}
	return Result;
}
