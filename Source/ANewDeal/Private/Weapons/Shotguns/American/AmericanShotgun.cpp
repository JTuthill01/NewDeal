#include "Weapons/Shotguns/American/AmericanShotgun.h"
#include "Character/Player/PlayerCharacter.h"

AAmericanShotgun::AAmericanShotgun() : Length(0.F), ReloadStart(0), ReloadLoop(1), ReloadEnd(2)
{
	AWeaponBase::WeaponParser(L"AmericanShotgun");

	EjectTransform = WeaponMesh->GetSocketTransform("AmmoEject");

	EjectQuat = EjectTransform.GetRotation();

	FireTransform = WeaponMesh->GetSocketTransform("Fire_FX_Slot");

	FireQuat = FireTransform.GetRotation();
}

void AAmericanShotgun::ShotgunReloadStart()
{
	Super::ShotgunReloadStart();

	if (IsValid(PlayerRef) && IsValid(WeaponAnimInstance))
	{
		Length = WeaponAnimInstance->Montage_Play(AmericanReloadArray[ReloadStart]);

		PlayerRef->GetPlayerAnimInstance()->Montage_Play(PlayerRef->GetAmericanReloadMonatge()[ReloadStart]);
	
		GetWorldTimerManager().SetTimer(AmericanReloadHandle, this, &AAmericanShotgun::ShotgunReloadLoop, Length, false);
	}
}

void AAmericanShotgun::BeginPlay()
{
	Super::BeginPlay();
}

void AAmericanShotgun::ShotgunReloadLoop()
{
	GetWorldTimerManager().ClearTimer(AmericanReloadHandle);

	Length = 0.F;

	WeaponData.CurrentMagTotal++;

	WeaponData.CurrentTotalAmmo--;

	OnWeaponReload.Broadcast(WeaponData.CurrentMagTotal, WeaponData.CurrentTotalAmmo);

	Length = WeaponAnimInstance->Montage_Play(AmericanReloadArray[ReloadLoop]);

	PlayerRef->GetPlayerAnimInstance()->Montage_Play(PlayerRef->GetAmericanReloadMonatge()[ReloadLoop]);

	GetWorldTimerManager().SetTimer(AmericanReloadHandle, this, &AAmericanShotgun::UpdateReloadAmmo, Length, false);
}

void AAmericanShotgun::ShotgunReloadEnd()
{
	WeaponAnimInstance->Montage_Play(AmericanReloadArray[ReloadEnd]);

	PlayerRef->GetPlayerAnimInstance()->Montage_Play(PlayerRef->GetAmericanReloadMonatge()[ReloadEnd]);

	GetWorldTimerManager().ClearTimer(AmericanReloadHandle);

	Length = 0.F;
}

void AAmericanShotgun::UpdateReloadAmmo()
{
	GetWorldTimerManager().ClearTimer(AmericanReloadHandle);

	if (WeaponData.CurrentMagTotal >= WeaponData.MaxMagTotal)
		ShotgunReloadEnd();

	else
		ShotgunReloadLoop();
}

