#include "EBProjectile/EBBulletProjectile.h"

AEBBulletProjectile::AEBBulletProjectile()
{
	EBProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EB Projectile Mesh"));
	EBProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EBProjectileMesh->SetSimulatePhysics(false);
}
