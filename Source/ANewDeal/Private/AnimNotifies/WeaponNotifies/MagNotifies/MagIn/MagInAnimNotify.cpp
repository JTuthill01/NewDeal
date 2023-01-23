#include "AnimNotifies/WeaponNotifies/MagNotifies/MagIn/MagInAnimNotify.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "Kismet/GameplayStatics.h"

void UMagInAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (IsValid(MeshComp->GetOwner()))
	{
		auto&& Current = Cast<AWeaponBase>(MeshComp->GetOwner());

		if (IsValid(Current))
			UGameplayStatics::SpawnSoundAttached(Current->GetWeaponFXData().MagInSound, MeshComp);
	}
}
