#include "Weapons/AK/AK47/AK47.h"

AAK47::AAK47()
{
	AWeaponBase::WeaponParser(L"AK47");

	EjectTransform = WeaponMesh->GetSocketTransform("AmmoEject");

	EjectQuat = EjectTransform.GetRotation();

	FireTransform = WeaponMesh->GetSocketTransform("Fire_FX_Slot");

	FireQuat = FireTransform.GetRotation();
}
