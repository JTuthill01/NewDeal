#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "SVD.generated.h"

UCLASS()
class ANEWDEAL_API ASVD : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	ASVD();

protected:
	virtual void SpawnProjectile() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ProjectileMesh, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AProjectileBase> Projectile;

private:
	TObjectPtr<class AProjectileBase> CurrentProjectile;
};
