#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums/WeaponsEnums/WeaponEnums.h"
#include "WeaponBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateCurrentTotalAmmo, int32, TotalAmmoCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponFire, int32, Ammo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponReload, int32, CurrentMagAmount, int32, CurrentTotalAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShotgunReload, int32, CurrentAmount, int32, CurrentTotal);

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 CurrentMagTotal = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MaxMagTotal = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 CurrentTotalAmmo = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MaxTotalAmmo = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 LowAmmo = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 CrosshairIndex = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	double DamageAmount = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	double CriciticalHitChance = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	double DamageRadius = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName = FName(NAME_None);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<class UTexture2D> Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EWeaponName CurrentWeaponName = EWeaponName::EWN_NONE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EWeaponFireType FireType = EWeaponFireType::EWFT_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EWeaponType WeaponType = EWeaponType::EWT_None;
};

USTRUCT(BlueprintType)
struct FWeaponFXData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USoundBase> RackSlideSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USoundBase> MagOutSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USoundBase> MagInSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USoundBase> FireSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UNiagaraSystem> AmmoEject = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UNiagaraSystem> FireFX = nullptr;
};

UCLASS()
class ANEWDEAL_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	/** Getters C++ ONLY */
	FORCEINLINE FWeaponData GetWeaponData() const { return WeaponData; }

	FORCEINLINE FWeaponFXData GetWeaponFXData() const { return WeaponFXData; }

	FORCEINLINE TObjectPtr<UAnimInstance> GetWeaponAnimInstance() const { return WeaponAnimInstance; }

	FORCEINLINE TObjectPtr<UAnimMontage> GetWeaponReloadMontage() const { return WeaponReloadMontage; }

	FORCEINLINE TObjectPtr<UAnimMontage> GetWeaponFireMontage() const { return WeaponFireMontage; }

	FORCEINLINE FTransform GetFireTransform() const { return FireTransform; }

	/** BP Getters ONLY */
	UFUNCTION(BlueprintCallable)
	FWeaponData GetCurrentWeaponData() const { return WeaponData; }

public:
	void WeaponFire(EWeaponFireType CurrentType, TObjectPtr<UAnimInstance>InInstance, TObjectPtr<UAnimMontage> InMontage);

	void WeaponReload(EWeaponFireType CurrentType, TObjectPtr<UAnimInstance>InInstance, TObjectPtr<UAnimMontage> InMontage);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void StopFire();

	void WeaponFire();

	bool MagHasAmmo();

	bool HasAmmoForReload();

	bool IsMagFull();

	bool CanFireOrReload();

	bool IsAmmoFull();

	bool IsWeaponFiringOrReloading(TObjectPtr<UAnimInstance>InInstance, TObjectPtr<UAnimMontage> InMontage);

protected:
	void WeaponParser(FString InObjectID);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EBProjectile();

	virtual void ShotgunReloadStart();

	virtual void SpawnProjectile();

private:
	void CreateImpactFX(FHitResult HitResult);

	void ShotgunFireMulti(int32 InShotgunPelletCount);

	void BulletTrace();

public:
	UPROPERTY(BlueprintAssignable)
	FOnWeaponFire OnWeaponFire;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponReload OnWeaponReload;

protected:
	UPROPERTY()
	TObjectPtr<class APlayerCharacter> PlayerRef;

	UPROPERTY()
	TObjectPtr<UAnimInstance> WeaponAnimInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WeaponRoot)
	TObjectPtr<class USceneComponent> WeaponRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WeaponMesh)
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<UAnimMontage> WeaponFireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<UAnimMontage> WeaponReloadMontage;

	/* BP Only */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Structs)
	FWeaponFXData WeaponFXData;

#pragma region Shotgun Data

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ShotgunBalistics)
	int32 ShotgunPellets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ShotgunBalistics)
	int32 Range;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ShotgunBalistics)
	float SpreadAngle;

#pragma endregion

protected:
	FWeaponData WeaponData;

	bool bHasHitOccured;
	bool bCanShotgunFireOrReload;
	bool bIsUsingEB;

	FQuat EjectQuat;
	FTransform EjectTransform;

	FQuat FireQuat;
	FTransform FireTransform;

	FString ObjectID;
	FString IconFilePath;
};
