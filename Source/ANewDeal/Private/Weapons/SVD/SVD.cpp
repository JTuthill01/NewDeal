#include "Weapons/SVD/SVD.h"
#include "Projectiles/ProjectileBase.h"

ASVD::ASVD()
{
	AWeaponBase::WeaponParser(L"SVD");

	EjectTransform = WeaponMesh->GetSocketTransform("AmmoEject");

	EjectQuat = EjectTransform.GetRotation();

	FireTransform = WeaponMesh->GetSocketTransform("Fire_FX_Slot");

	FireQuat = FireTransform.GetRotation();
}

void ASVD::SpawnProjectile()
{
	Super::SpawnProjectile();

	FActorSpawnParameters Parms;
	Parms.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CurrentProjectile = GetWorld()->SpawnActor<AProjectileBase>(Projectile, FireTransform, Parms);
}
