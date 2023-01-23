#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "ImpactEffects.generated.h"

USTRUCT(BlueprintType)
struct FImpactEffect
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UFXSystemAsset> ImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UMaterialInstance> ImpactDecal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class USoundBase> ImpactSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector DecalSize { 2.5F, 2.5F, 2.5F };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DecalLifeTime = 8.F;
};

UCLASS()
class ANEWDEAL_API UImpactEffects : public UPhysicalMaterial
{
	GENERATED_BODY()
	
public:
	UImpactEffects();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FImpactEffect LineTraceImpactEffect;
};
