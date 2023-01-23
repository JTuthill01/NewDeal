#include "Weapons/ShortStroke/ShortStrokeAR.h"

AShortStrokeAR::AShortStrokeAR() 
{ 
	AWeaponBase::WeaponParser(L"ShortStrokeAR"); 

	EjectTransform = WeaponMesh->GetSocketTransform("AmmoEject");

	EjectQuat = EjectTransform.GetRotation();

	FireTransform = WeaponMesh->GetSocketTransform("Fire_FX_Slot");

	FireQuat = FireTransform.GetRotation();
}



