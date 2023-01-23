#include "Weapons/WeaponBase/WeaponBase.h"
#include "Character/Player/PlayerCharacter.h"
#include "Effects/ImpactEffects/ImpactEffects.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "JsonUtilities.h"

// Sets default values
AWeaponBase::AWeaponBase() : ShotgunPellets(6), Range(4'500), SpreadAngle(8.89F), bHasHitOccured(false), bCanShotgunFireOrReload(false), EjectQuat(FQuat(0.F)), FireQuat(FQuat(0.F)), 
	IconFilePath("")
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Root"));
	SetRootComponent(WeaponRoot);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->SetCastShadow(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->bReturnMaterialOnMove = true;
	WeaponMesh->SetupAttachment(WeaponRoot);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = IPlayerRefInterface::Execute_GetPlayerRef(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (IsValid(WeaponMesh))
		WeaponAnimInstance = WeaponMesh->GetAnimInstance();
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponBase::WeaponFire(EWeaponFireType CurrentType, TObjectPtr<UAnimInstance>InInstance, TObjectPtr<UAnimMontage> InMontage)
{
	EjectTransform = WeaponMesh->GetSocketTransform("AmmoEject");

	EjectQuat = EjectTransform.GetRotation();

	FireTransform = WeaponMesh->GetSocketTransform("Fire_FX_Slot");

	FireQuat = FireTransform.GetRotation();

	WeaponData.CurrentMagTotal--;

	OnWeaponFire.Broadcast(WeaponData.CurrentMagTotal);

	if (WeaponData.CurrentMagTotal < 0)
		WeaponData.CurrentMagTotal = 0;

	switch (CurrentType)
	{
	case EWeaponFireType::EWFT_None:
		break;

	case EWeaponFireType::EWFT_Hitscan:

		if (IsValid(InInstance))
		{
			InInstance->Montage_Play(InMontage);
			PlayerRef->PlayerFireWeapon();

			UGameplayStatics::SpawnSoundAttached(WeaponFXData.FireSound, WeaponMesh);

			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WeaponFXData.AmmoEject, EjectTransform.GetTranslation(), EjectQuat.Rotator());
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WeaponFXData.FireFX, FireTransform.GetTranslation(), FireQuat.Rotator());

		}

		BulletTrace();

		break;

	case EWeaponFireType::EWFT_Projectile:

		if (IsValid(InInstance))
		{
			InInstance->Montage_Play(InMontage);
			PlayerRef->PlayerFireWeapon();

			UGameplayStatics::SpawnSoundAttached(WeaponFXData.FireSound, WeaponMesh);

			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WeaponFXData.AmmoEject, EjectTransform.GetTranslation(), EjectQuat.Rotator());
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WeaponFXData.FireFX, FireTransform.GetTranslation(), FireQuat.Rotator());

		}

		SpawnProjectile();

		break;

	case EWeaponFireType::EWFT_SpreadScan:

		if (IsValid(InInstance))
		{
			InInstance->Montage_Play(InMontage);
			PlayerRef->PlayerFireWeapon();

			UGameplayStatics::SpawnSoundAttached(WeaponFXData.FireSound, WeaponMesh);

			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WeaponFXData.AmmoEject, EjectTransform.GetTranslation(), EjectQuat.Rotator());
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WeaponFXData.FireFX, FireTransform.GetTranslation(), FireQuat.Rotator());

		}

		ShotgunFireMulti(ShotgunPellets);

		break;

	default:
		break;
	}
}

void AWeaponBase::WeaponReload(EWeaponFireType CurrentType, TObjectPtr<UAnimInstance>InInstance, TObjectPtr<UAnimMontage> InMontage)
{
	if (!IsValid(PlayerRef))
		return;

	switch (CurrentType)
	{
	case EWeaponFireType::EWFT_None:
		break;

	case EWeaponFireType::EWFT_Hitscan:

		WeaponData.CurrentTotalAmmo -= WeaponData.MaxMagTotal - WeaponData.CurrentMagTotal;

		WeaponData.CurrentMagTotal = UKismetMathLibrary::Min(WeaponData.MaxMagTotal, WeaponData.CurrentTotalAmmo);

		WeaponData.CurrentMagTotal = FMath::Clamp(WeaponData.CurrentMagTotal, 0, WeaponData.MaxMagTotal);

		WeaponData.CurrentTotalAmmo = FMath::Clamp(WeaponData.CurrentTotalAmmo, 0, WeaponData.MaxTotalAmmo);

		if (IsValid(InInstance))
		{
			InInstance->Montage_Play(InMontage);

			PlayerRef->PlayerReloadWeapon();

			OnWeaponReload.Broadcast(WeaponData.CurrentMagTotal, WeaponData.CurrentTotalAmmo);
		}

		break;

	case EWeaponFireType::EWFT_Projectile:

		WeaponData.CurrentTotalAmmo -= WeaponData.MaxMagTotal - WeaponData.CurrentMagTotal;

		WeaponData.CurrentMagTotal = UKismetMathLibrary::Min(WeaponData.MaxMagTotal, WeaponData.CurrentTotalAmmo);

		WeaponData.CurrentMagTotal = FMath::Clamp(WeaponData.CurrentMagTotal, 0, WeaponData.MaxMagTotal);

		WeaponData.CurrentTotalAmmo = FMath::Clamp(WeaponData.CurrentTotalAmmo, 0, WeaponData.MaxTotalAmmo);

		if (IsValid(InInstance))
		{
			InInstance->Montage_Play(InMontage);

			PlayerRef->PlayerReloadWeapon();

			OnWeaponReload.Broadcast(WeaponData.CurrentMagTotal, WeaponData.CurrentTotalAmmo);
		}

		break;

	case EWeaponFireType::EWFT_SpreadScan:

		ShotgunReloadStart();

		break;

	default:
		break;
	}
}

void AWeaponBase::CreateImpactFX(FHitResult HitResult)
{
	if (TObjectPtr<UImpactEffects> PhysMat = Cast<UImpactEffects>(HitResult.PhysMaterial))
	{
		if (TObjectPtr<USoundBase> ImpactSound = PhysMat->LineTraceImpactEffect.ImpactSound)
		{
			FVector Location = HitResult.Location;

			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Location);
		}

		if (TObjectPtr<UNiagaraSystem> HitFX = Cast<UNiagaraSystem>(PhysMat->LineTraceImpactEffect.ImpactEffect))
		{
			FRotator Rotation = UKismetMathLibrary::MakeRotFromX(HitResult.Normal);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitFX, HitResult.Location, Rotation);
		}

		else if (TObjectPtr<UParticleSystem> ParticleFX = Cast<UParticleSystem>(PhysMat->LineTraceImpactEffect.ImpactEffect))
		{
			FRotator Rotation = UKismetMathLibrary::MakeRotFromX(HitResult.Normal);
			FVector Location = HitResult.Location;

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleFX, Location, Rotation);
		}

		if (TObjectPtr<UMaterialInstance> ImpactDecal = PhysMat->LineTraceImpactEffect.ImpactDecal)
		{
			if (TObjectPtr<USceneComponent> HitComponent = HitResult.GetComponent())
			{
				FRotator Rotation = UKismetMathLibrary::MakeRotFromX(HitResult.Normal);

				Rotation.Pitch += 180.0f;

				FVector DecalSize = PhysMat->LineTraceImpactEffect.DecalSize;

				float DecalLifetime = PhysMat->LineTraceImpactEffect.DecalLifeTime;

				UGameplayStatics::SpawnDecalAttached(ImpactDecal, DecalSize, HitComponent, NAME_None,
					HitResult.Location, Rotation, EAttachLocation::KeepWorldPosition, DecalLifetime);
			}
		}
	}
}

void AWeaponBase::BulletTrace()
{
	if (!IsValid(PlayerRef))
		return;

	FHitResult HitResult;

	FVector Start{ PlayerRef->GetPlayerCamera()->GetComponentLocation() };
	FVector Rotation{ PlayerRef->GetPlayerCamera()->GetComponentRotation().Vector() };
	FVector End{ Start + Rotation * 10'000.F };
	FVector Scale = FVector(1.F);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(PlayerRef);
	ActorsToIgnore.Add(this);

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

	const bool bHasBeenHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), Start, End, TraceObjects, true, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

	if (bHasBeenHit)
	{
		CreateImpactFX(HitResult);

		UGameplayStatics::ApplyPointDamage(HitResult.GetActor(), 20.F, HitResult.ImpactPoint, HitResult, nullptr, this, UDamageType::StaticClass());
	}
}

void AWeaponBase::WeaponFire()
{
	
}

void AWeaponBase::ShotgunFireMulti(int32 InShotgunPelletCount)
{
	if (!IsValid(PlayerRef))
		return;

	for (int32 i = 0; i < InShotgunPelletCount; ++i)
	{
		FVector Start{ WeaponMesh->GetSocketLocation("Fire_FX_Slot") };
		FVector Middle{ (UKismetMathLibrary::RandomUnitVectorInConeInDegrees(PlayerRef->GetPlayerCamera()->GetForwardVector(), SpreadAngle)) * Range };
		FVector End{ Start + Middle };

		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(PlayerRef);
		ActorsToIgnore.Add(this);

		TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
		TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
		TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

		TArray<FHitResult> OutResult;

		bHasHitOccured = UKismetSystemLibrary::LineTraceMultiForObjects(GetWorld(), Start, End, TraceObjects, true, ActorsToIgnore, EDrawDebugTrace::None, OutResult, true);

		if (bHasHitOccured)
		{
			for (auto& Result : OutResult)
			{
				CreateImpactFX(Result);

				UGameplayStatics::ApplyPointDamage(Result.GetActor(), 10.F, Result.ImpactPoint, Result, nullptr, this, UDamageType::StaticClass());
			}
		}
	}
}

void AWeaponBase::WeaponParser(FString InObjectID)
{
	FWeaponData LocalWeaponData;

	/* Creates a string ref to wherever the json file(s) are */
	const FString JsonFilePath = FPaths::ProjectContentDir() + "/_Main/JSON/WeaponData/WeaponStats.json";
	FString JsonString; /* Json converted to FString */

	FFileHelper::LoadFileToString(JsonString, *JsonFilePath); /* Remember to dereference file path */

	/* Create a json object to store the information from the json string */
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();

	/* The json reader is used to deserialize the json object later on */
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		/* Gets whatever "object" from the json file you choose */
		TSharedPtr<FJsonObject> DataObject = JsonObject->GetObjectField(InObjectID);

		WeaponData.CurrentMagTotal = DataObject->GetIntegerField("CurrentMagTotal");

		WeaponData.MaxMagTotal = DataObject->GetIntegerField("MaxMagTotal");

		WeaponData.CurrentTotalAmmo = DataObject->GetIntegerField("CurrentTotalAmmo");

		WeaponData.MaxTotalAmmo = DataObject->GetIntegerField("MaxTotalAmmo");

		WeaponData.LowAmmo = DataObject->GetIntegerField("LowAmmo");

		WeaponData.CrosshairIndex = DataObject->GetIntegerField("CrosshairIndex");

		WeaponData.DamageAmount = DataObject->GetNumberField("DamageAmount");

		WeaponData.CriciticalHitChance = DataObject->GetNumberField("CriciticalHitChance");

		WeaponData.DamageRadius = DataObject->GetNumberField("DamageRadius");

		WeaponData.SocketName = FName(*DataObject->GetStringField("SocketName"));

		IconFilePath = DataObject->GetStringField("Icon");

		WeaponData.CurrentWeaponName = static_cast<EWeaponName>(DataObject->GetIntegerField("WeaponName"));

		WeaponData.FireType = static_cast<EWeaponFireType>(DataObject->GetIntegerField("FireType"));

		WeaponData.WeaponType = static_cast<EWeaponType>(DataObject->GetIntegerField("WeaponType"));
	}
}

bool AWeaponBase::MagHasAmmo() { return WeaponData.CurrentMagTotal > 0; }

bool AWeaponBase::HasAmmoForReload() { return WeaponData.CurrentTotalAmmo > 0; }

bool AWeaponBase::IsMagFull() { return WeaponData.CurrentMagTotal >= WeaponData.MaxMagTotal; }

bool AWeaponBase::CanFireOrReload() { return !WeaponAnimInstance->Montage_IsPlaying(WeaponFireMontage) && !WeaponAnimInstance->Montage_IsPlaying(WeaponReloadMontage); }

bool AWeaponBase::IsAmmoFull() { return WeaponData.CurrentTotalAmmo >= WeaponData.MaxTotalAmmo; }

bool AWeaponBase::IsWeaponFiringOrReloading(TObjectPtr<UAnimInstance>InInstance, TObjectPtr<UAnimMontage> InMontage) { return InInstance->Montage_IsPlaying(InMontage); }

void AWeaponBase::StopFire() {}

void AWeaponBase::ShotgunReloadStart() {}

void AWeaponBase::SpawnProjectile() {}
