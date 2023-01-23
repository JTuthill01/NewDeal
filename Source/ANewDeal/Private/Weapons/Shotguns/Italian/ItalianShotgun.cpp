#include "Weapons/Shotguns/Italian/ItalianShotgun.h"
#include "Character/Player/PlayerCharacter.h"

AItalianShotgun::AItalianShotgun() : Length(0.F), ReloadStart(0), ReloadLoop(1), ReloadEnd(2)
{
	AWeaponBase::WeaponParser(L"AmericanShotgun");

	EjectTransform = WeaponMesh->GetSocketTransform("AmmoEject");

	EjectQuat = EjectTransform.GetRotation();

	FireTransform = WeaponMesh->GetSocketTransform("Fire_FX_Slot");

	FireQuat = FireTransform.GetRotation();
}

void AItalianShotgun::ShotgunReloadStart()
{
	Super::ShotgunReloadStart();

	if (IsValid(PlayerRef) && IsValid(WeaponAnimInstance))
	{
		Length = WeaponAnimInstance->Montage_Play(ItalianReloadArray[ReloadStart]);

		PlayerRef->GetPlayerAnimInstance()->Montage_Play(PlayerRef->GetItalianReloadMonatge()[ReloadStart]);

		GetWorldTimerManager().SetTimer(ItalianReloadHandle, this, &AItalianShotgun::ShotgunReloadLoop, Length, false);
	}
}

void AItalianShotgun::ShotgunReloadLoop()
{
	GetWorldTimerManager().ClearTimer(ItalianReloadHandle);

	Length = 0.F;

	WeaponData.CurrentMagTotal++;

	WeaponData.CurrentTotalAmmo--;

	OnWeaponReload.Broadcast(WeaponData.CurrentMagTotal, WeaponData.CurrentTotalAmmo);

	Length = WeaponAnimInstance->Montage_Play(ItalianReloadArray[ReloadLoop]);

	PlayerRef->GetPlayerAnimInstance()->Montage_Play(PlayerRef->GetItalianReloadMonatge()[ReloadLoop]);

	GetWorldTimerManager().SetTimer(ItalianReloadHandle, this, &AItalianShotgun::UpdateReloadAmmo, Length, false);
}

void AItalianShotgun::ShotgunReloadEnd()
{
	WeaponAnimInstance->Montage_Play(ItalianReloadArray[ReloadEnd]);

	PlayerRef->GetPlayerAnimInstance()->Montage_Play(PlayerRef->GetItalianReloadMonatge()[ReloadEnd]);

	GetWorldTimerManager().ClearTimer(ItalianReloadHandle);
}

void AItalianShotgun::UpdateReloadAmmo()
{
	GetWorldTimerManager().ClearTimer(ItalianReloadHandle);

	if (WeaponData.CurrentMagTotal >= WeaponData.MaxMagTotal)
		ShotgunReloadEnd();

	else
		ShotgunReloadLoop();
}
