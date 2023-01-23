#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "ItalianShotgun.generated.h"

UCLASS()
class ANEWDEAL_API AItalianShotgun : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AItalianShotgun();

protected:
	virtual void ShotgunReloadStart() override;

private:

	void ShotgunReloadLoop();

	void ShotgunReloadEnd();

	void UpdateReloadAmmo();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Montages, meta = (AllowPrivateAccess = "true"))
		TArray<TObjectPtr<UAnimMontage>> ItalianReloadArray;

private:
	FTimerHandle ItalianReloadHandle;

	float Length;

	int32 ReloadStart;
	int32 ReloadLoop;
	int32 ReloadEnd;
};
