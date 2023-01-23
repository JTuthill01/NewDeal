#include "Weapons/SKS/SKS.h"

ASKS::ASKS()
{
	AWeaponBase::WeaponParser(L"SKS");

	EjectTransform = WeaponMesh->GetSocketTransform("AmmoEject");

	EjectQuat = EjectTransform.GetRotation();

	FireTransform = WeaponMesh->GetSocketTransform("Fire_FX_Slot");

	FireQuat = FireTransform.GetRotation();
}
