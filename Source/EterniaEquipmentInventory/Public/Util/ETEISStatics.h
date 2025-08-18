// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ETEISStatics.generated.h"

/**
 * 
 */
UCLASS()
class ETERNIAITEMS_API UETEISStatics : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	template<typename T>
	static void GetObjectsOfClass(TArray<T*>& OutArray);

};

template<typename T>
void UETEISStatics::GetObjectsOfClass(TArray<T*>& OutArray)
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AssetData;
	AssetRegistryModule.Get().GetAssetsByClass(FTopLevelAssetPath(T::StaticClass()->GetPathName()), AssetData);
	for (int i = 0; i < AssetData.Num(); i++) {
		T* Object = Cast<T>(AssetData[i].GetAsset());
		OutArray.Add(Object);
	}
}
