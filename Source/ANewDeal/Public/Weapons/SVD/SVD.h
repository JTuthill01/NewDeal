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

	virtual void EBProjectile() override;

	virtual void StopFire() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ProjectileMesh, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AProjectileBase> Projectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = EB, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UEBBarrel> BarrelComp;

private:
	TObjectPtr<class AProjectileBase> CurrentProjectile;
};
