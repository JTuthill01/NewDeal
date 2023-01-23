#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PartData.generated.h"

UENUM(BlueprintType)
enum class EStaticParts : uint8 
{
	ESP_None UMETA(DisplayName = "NONE"), ESP_Part1 UMETA(DisplayName = "First Part"), ESP_Part2 UMETA(DisplayName = "Second Part"), ESP_Part3 UMETA(DisplayName = "Third Part")
};

UCLASS()
class ANEWDEAL_API UPartData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = Parts)
	TMap<EStaticParts, class UStaticMeshComps*> AvailableParts;
};
