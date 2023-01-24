#include "Weapons/SVD/SVD.h"
#include "Projectiles/ProjectileBase.h"
#include "EBBarrel.h"

ASVD::ASVD()
{
	AWeaponBase::WeaponParser(L"SVD");

	EjectTransform = WeaponMesh->GetSocketTransform("AmmoEject");

	EjectQuat = EjectTransform.GetRotation();

	FireTransform = WeaponMesh->GetSocketTransform("Fire_FX_Slot");

	FireQuat = FireTransform.GetRotation();

	bIsUsingEB = true;

	BarrelComp = CreateDefaultSubobject<UEBBarrel>(L"Barrel Component");
	BarrelComp->SetupAttachment(WeaponMesh, FName("Fire_FX_Slot"));
}

void ASVD::SpawnProjectile()
{
	Super::SpawnProjectile();

	FActorSpawnParameters Parms;
	Parms.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CurrentProjectile = GetWorld()->SpawnActor<AProjectileBase>(Projectile, FireTransform, Parms);
}

void ASVD::EBProjectile()
{
	Super::EBProjectile();

	if (IsValid(BarrelComp))
		BarrelComp->Shoot(true);
}

void ASVD::StopFire()
{
	Super::StopFire();

	if (IsValid(BarrelComp))
		BarrelComp->Shoot(false);
}
