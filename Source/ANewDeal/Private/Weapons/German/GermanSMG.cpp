#include "Weapons/German/GermanSMG.h"

AGermanSMG::AGermanSMG()
{
	AWeaponBase::WeaponParser(L"GermanSMG");

	EjectTransform = WeaponMesh->GetSocketTransform("AmmoEject");

	EjectQuat = EjectTransform.GetRotation();

	FireTransform = WeaponMesh->GetSocketTransform("Fire_FX_Slot");

	FireQuat = FireTransform.GetRotation();
}
