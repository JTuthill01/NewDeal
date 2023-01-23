#include "Weapons/XM82/XM82.h"

AXM82::AXM82()
{
	AWeaponBase::WeaponParser(L"XM82");

	EjectTransform = WeaponMesh->GetSocketTransform("AmmoEject");

	EjectQuat = EjectTransform.GetRotation();

	FireTransform = WeaponMesh->GetSocketTransform("Fire_FX_Slot");

	FireQuat = FireTransform.GetRotation();
}
