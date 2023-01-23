#include "Weapons/Belgian/BelgianAR.h"

ABelgianAR::ABelgianAR()
{
	AWeaponBase::WeaponParser(L"BelgianAR");

	EjectTransform = WeaponMesh->GetSocketTransform("AmmoEject");

	EjectQuat = EjectTransform.GetRotation();

	FireTransform = WeaponMesh->GetSocketTransform("Fire_FX_Slot");

	FireQuat = FireTransform.GetRotation();
}

