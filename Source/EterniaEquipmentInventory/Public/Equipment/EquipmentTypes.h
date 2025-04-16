// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EquipmentTypes.generated.h"

UENUM(BlueprintType)
enum EEquipmentSlotType {
	EEST_RightHand UMETA(DisplayName="Right Hand"),
	EEST_LeftHand UMETA(DisplayName="Left Hand"),
	EEST_Torso UMETA(DisplayName="Torso"),
	EEST_Consumable UMETA(DisplayName="Consumable"),
	EEST_MAX UMETA(DisplayName="Invalid")
};
