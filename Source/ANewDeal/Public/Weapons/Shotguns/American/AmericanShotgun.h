#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "AmericanShotgun.generated.h"

UCLASS()
class ANEWDEAL_API AAmericanShotgun : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AAmericanShotgun();

protected:
	virtual void ShotgunReloadStart() override;

	virtual void BeginPlay() override;

private:

	void ShotgunReloadLoop();

	void ShotgunReloadEnd();

	void UpdateReloadAmmo();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Montages, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UAnimMontage>> AmericanReloadArray;

private:
	FTimerHandle AmericanReloadHandle;

	float Length;

	int32 ReloadStart;
	int32 ReloadLoop;
	int32 ReloadEnd;
};
