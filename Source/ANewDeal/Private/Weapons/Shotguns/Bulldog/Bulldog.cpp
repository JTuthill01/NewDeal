#include "Weapons/Shotguns/Bulldog/Bulldog.h"
#include "Character/Player/PlayerCharacter.h"

ABulldog::ABulldog() : Length(0.F), ReloadStart(0), ReloadLoop(1), ReloadEnd(2)
{
	AWeaponBase::WeaponParser(L"Bulldog");

	EjectTransform = WeaponMesh->GetSocketTransform("AmmoEject");

	EjectQuat = EjectTransform.GetRotation();

	FireTransform = WeaponMesh->GetSocketTransform("Fire_FX_Slot");

	FireQuat = FireTransform.GetRotation();
}

void ABulldog::ShotgunReloadStart()
{
	Super::ShotgunReloadStart();

	if (IsValid(PlayerRef) && IsValid(WeaponAnimInstance))
	{
		Length = WeaponAnimInstance->Montage_Play(BulldogReloadArray[ReloadStart]);

		PlayerRef->GetPlayerAnimInstance()->Montage_Play(PlayerRef->GetBulldogReloadMonatge()[ReloadStart]);

		GetWorldTimerManager().SetTimer(BulldogReloadHandle, this, &ABulldog::ShotgunReloadLoop, Length, false);
	}
}

void ABulldog::ShotgunReloadLoop()
{
	GetWorldTimerManager().ClearTimer(BulldogReloadHandle);

	Length = 0.F;

	WeaponData.CurrentMagTotal++;

	WeaponData.CurrentTotalAmmo--;

	OnWeaponReload.Broadcast(WeaponData.CurrentMagTotal, WeaponData.CurrentTotalAmmo);

	Length = WeaponAnimInstance->Montage_Play(BulldogReloadArray[ReloadLoop]);

	PlayerRef->GetPlayerAnimInstance()->Montage_Play(PlayerRef->GetBulldogReloadMonatge()[ReloadLoop]);

	GetWorldTimerManager().SetTimer(BulldogReloadHandle, this, &ABulldog::UpdateReloadAmmo, Length, false);
}

void ABulldog::ShotgunReloadEnd()
{
	WeaponAnimInstance->Montage_Play(BulldogReloadArray[ReloadEnd]);

	PlayerRef->GetPlayerAnimInstance()->Montage_Play(PlayerRef->GetBulldogReloadMonatge()[ReloadEnd]);

	GetWorldTimerManager().ClearTimer(BulldogReloadHandle);

	Length = 0.F;
}

void ABulldog::UpdateReloadAmmo()
{
	GetWorldTimerManager().ClearTimer(BulldogReloadHandle);

	if (WeaponData.CurrentMagTotal >= WeaponData.MaxMagTotal)
		ShotgunReloadEnd();

	else
		ShotgunReloadLoop();
}
