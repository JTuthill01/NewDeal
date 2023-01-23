#include "Projectiles/ProjectileBase.h"
#include "Character/Player/PlayerCharacter.h"
#include "Interfaces/PlayerRef/PlayerRefInterface.h"
#include "Character/HealthComponent/PlayerHealthComponent.h"
#include "Effects/ImpactEffects/ImpactEffects.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectileBase::AProjectileBase() : bIsCausingRadiusDamage(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = 10000.F;
	ProjectileMovement->MaxSpeed = 10000.F;
	ProjectileMovement->ProjectileGravityScale = 0.2F;
	ProjectileMovement->bSweepCollision = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetSphereRadius(8.F);
	Sphere->SetGenerateOverlapEvents(true);
	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	Sphere->bReturnMaterialOnMove = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMesh->SetSimulatePhysics(false);
	ProjectileMesh->SetupAttachment(Sphere);

	SetRootComponent(Sphere);
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = IPlayerRefInterface::Execute_GetPlayerRef(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	ProjectileMovement->OnProjectileStop.AddDynamic(this, &AProjectileBase::ProjectileStop);
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileBase::ProjectileStop(const FHitResult& HitResult)
{
	Destroy();

	if (bIsCausingRadiusDamage)
		ExplodeOnImpact(HitResult);

	else
	{
		DealDamage(HitResult);

		SpawnImpactFX(HitResult);
	}
}

void AProjectileBase::ExplodeOnImpact(const FHitResult& HitResult)
{
	if (!IsValid(PlayerRef))
		return;

	WeaponData.DamageRadius = PlayerRef->GetCurrentWeapons()[PlayerRef->GetCurrentPlayerWeaponName()]->GetWeaponData().DamageRadius;
	WeaponData.DamageAmount = PlayerRef->GetCurrentWeapons()[PlayerRef->GetCurrentPlayerWeaponName()]->GetWeaponData().DamageAmount;

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	TArray<AActor*> ActorsToIgnore;
	TArray<AActor*> OutActors;

	ActorsToIgnore.Add(PlayerRef->GetCurrentWeapons()[PlayerRef->GetCurrentPlayerWeaponName()]);
	ActorsToIgnore.Add(this);

	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	const bool bHasSphereOverlapped = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), HitResult.ImpactPoint, WeaponData.DamageRadius, TraceObjects, nullptr, ActorsToIgnore, OutActors);

	if (bHasSphereOverlapped)
	{
		int32 Count = OutActors.Num();

		for (int32 i = 0; i < Count; ++i)
			const bool bWasDamageApplied = UGameplayStatics::ApplyRadialDamage(GetWorld(), WeaponData.DamageAmount, HitResult.ImpactPoint, WeaponData.DamageRadius, UDamageType::StaticClass(),
				ActorsToIgnore, this, GetInstigatorController(), true, ECollisionChannel::ECC_Visibility);
	}
}

void AProjectileBase::SpawnImpactFX(const FHitResult& HitResult)
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

void AProjectileBase::DealDamage(const FHitResult& HitResult)
{
	UGameplayStatics::ApplyPointDamage(HitResult.GetActor(), PlayerRef->GetCurrentWeapons()[PlayerRef->GetCurrentPlayerWeaponName()]->GetWeaponData().DamageAmount, HitResult.ImpactPoint, HitResult, GetInstigatorController(),
		this, UDamageType::StaticClass());
}



