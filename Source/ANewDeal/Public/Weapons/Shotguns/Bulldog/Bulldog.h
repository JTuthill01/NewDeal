#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "Bulldog.generated.h"

UCLASS()
class ANEWDEAL_API ABulldog : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	ABulldog();

protected:
	virtual void ShotgunReloadStart() override;

private:

	void ShotgunReloadLoop();

	void ShotgunReloadEnd();

	void UpdateReloadAmmo();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Montages, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UAnimMontage>> BulldogReloadArray;

private:
	FTimerHandle BulldogReloadHandle;

	float Length;

	int32 ReloadStart;
	int32 ReloadLoop;
	int32 ReloadEnd;
};
