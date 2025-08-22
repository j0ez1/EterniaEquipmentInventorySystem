// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ETDAItemDefinition.h"

UETDAItemType* UETDAItemDefinition::GetType() const {
	return ItemType.LoadSynchronous();
}
