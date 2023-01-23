#include "Weapons/HandCannon/HandCannon.h"

AHandCannon::AHandCannon()
{
	AWeaponBase::WeaponParser(L"HandCannon");

	EjectTransform = WeaponMesh->GetSocketTransform("AmmoEject");

	EjectQuat = EjectTransform.GetRotation();

	FireTransform = WeaponMesh->GetSocketTransform("Fire_FX_Slot");

	FireQuat = FireTransform.GetRotation();
}
