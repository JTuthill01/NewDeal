#include "Character/Player/PlayerCharacter.h"
#include "Character/Controller/PlayerCharacterController.h"
#include "Character/HealthComponent/PlayerHealthComponent.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "Enums/WeaponsEnums/WeaponEnums.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
APlayerCharacter::APlayerCharacter() : InteractableTraceTimer(0.25F), CurrentWeaponSlotEnum(EWeaponSlots::EWS_Default), bIsFirstSlotFull(false), bIsSecondSlotFull(false), bIsThirdSlotFull(false), 
	bIsFourthSlotFull(false), bHasOpenSlot(true)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;

	Arms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arms"));
	Arms->SetupAttachment(Camera);
	Arms->SetCastShadow(false);

	ShotgunShellMesh = CreateDefaultSubobject<UStaticMeshComponent>(L"Shotgun Shell");
	ShotgunShellMesh->SetupAttachment(Arms, "ShogunShellSocket");
	ShotgunShellMesh->SetCastShadow(false);
	ShotgunShellMesh->SetHiddenInGame(true);

	PlayerHealthComponent = CreateDefaultSubobject<UPlayerHealthComponent>(L"Health Component");
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerSetup();
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorldTimerManager().ClearTimer(InteractableTraceTimerHandle);

	InteractableTraceTimer = 0.25F;

	UKismetSystemLibrary::QuitGame(GetWorld(), 0, EQuitPreference::Quit, false);
}

void APlayerCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();

	// Make sure that we have a valid PlayerController.
	if (TObjectPtr<APlayerCharacterController> PC = Cast<APlayerCharacterController>(GetController()))
	{
		// Get the Enhanced Input Local Player Subsystem from the Local Player related to our Player Controller.
		if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			// PawnClientRestart can run more than once in an Actor's lifetime, so start by clearing out any leftover mappings.
			Subsystem->ClearAllMappings();

			// Add each mapping context, along with their priority values. Higher values out prioritize lower values.
			Subsystem->AddMappingContext(PlayerBaseMappingContext, BaseMappingPriority);
		}
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerCharacter::PlayerSetup()
{
	if (IsValid(Arms))
		PlayerAnimInstance = Arms->GetAnimInstance();

	if (IsValid(PlayerHealthComponent))
		PlayerHealthComponent->OnPlayerDeath.AddDynamic(this, &APlayerCharacter::PlayerDeath);

	GetWorldTimerManager().SetTimer(InteractableTraceTimerHandle, this, &APlayerCharacter::ScanForInteractables, InteractableTraceTimer, true);

	SpawnInitialWeapon();
}

void APlayerCharacter::SpawnInitialWeapon()
{
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector Location = Arms->GetComponentLocation();
	FRotator Rotation = Arms->GetComponentRotation();
	FVector Scale{ 1.F };

	FTransform LocalTransform = UKismetMathLibrary::MakeTransform(Location, Rotation, Scale);

	PlayerCurrentWeapons.WeaponSlot_01 = GetWorld()->SpawnActor<AWeaponBase>(InitialWeapon, LocalTransform);

	if (IsValid(PlayerCurrentWeapons.WeaponSlot_01))
	{
		CurrentPlayerWeapon = PlayerCurrentWeapons.WeaponSlot_01->GetWeaponData().CurrentWeaponName;

		CurrentWeaponSlotEnum = EWeaponSlots::EWS_FirstSlot;

		CurrentWeapons.Emplace(CurrentPlayerWeapon, PlayerCurrentWeapons.WeaponSlot_01);

		CurrentWeapons[CurrentPlayerWeapon]->AttachToComponent(Arms, FAttachmentTransformRules::SnapToTargetIncludingScale,
			PlayerCurrentWeapons.WeaponSlot_01->GetWeaponData().SocketName);

		bIsFirstSlotFull = true;
	}
}

void APlayerCharacter::PlayerDeath()
{
	EndPlay(EEndPlayReason::Destroyed);
}

void APlayerCharacter::ScanForInteractables()
{
}

void APlayerCharacter::InteractWithObject()
{
}

void APlayerCharacter::SpawnNewWeapon(TSubclassOf<class AWeaponBase> WeaponSub, EWeaponName CurrentName, bool& IsSuccessful)
{
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector Location = Arms->GetComponentLocation();
	FRotator Rotation = Arms->GetComponentRotation();
	FVector Scale{ 1.F };

	FTransform LocalTransform = UKismetMathLibrary::MakeTransform(Location, Rotation, Scale);

	if (bHasOpenSlot)
	{
		switch (CurrentWeaponSlotEnum)
		{
		case EWeaponSlots::EWS_Default:
			break;

		case EWeaponSlots::EWS_FirstSlot:

			if (!bIsFirstSlotFull)
			{
				PlayerCurrentWeapons.WeaponSlot_01 = GetWorld()->SpawnActor<AWeaponBase>(WeaponSub, LocalTransform, Params);

				if (IsValid(PlayerCurrentWeapons.WeaponSlot_01))
				{
					CurrentWeapons.Emplace(CurrentName, PlayerCurrentWeapons.WeaponSlot_01);

					CurrentWeapons[CurrentName]->AttachToComponent(Arms, FAttachmentTransformRules::SnapToTargetIncludingScale,
						PlayerCurrentWeapons.WeaponSlot_01->GetWeaponData().SocketName);

					bIsFirstSlotFull = true;

					CurrentWeaponSlotEnum = EWeaponSlots::EWS_FirstSlot;

					IsSuccessful = true;
				}

				else
					IsSuccessful = false;
			}

			else if (!bIsSecondSlotFull)
			{
				CurrentWeaponSlotEnum = EWeaponSlots::EWS_SecondSlot;

				SpawnNewWeapon(WeaponSub, CurrentName, IsSuccessful);
			}

			else
			{
				bHasOpenSlot = false;

				IsSuccessful = false;

				CurrentWeaponSlotEnum = EWeaponSlots::EWS_FirstSlot;
			}

			break;

		case EWeaponSlots::EWS_SecondSlot:

			if (!bIsSecondSlotFull)
			{
				PlayerCurrentWeapons.WeaponSlot_02 = GetWorld()->SpawnActor<AWeaponBase>(WeaponSub, LocalTransform, Params);

				if (IsValid(PlayerCurrentWeapons.WeaponSlot_02))
				{
					CurrentWeapons.Emplace(CurrentName, PlayerCurrentWeapons.WeaponSlot_02);

					CurrentWeapons[CurrentName]->AttachToComponent(Arms, FAttachmentTransformRules::SnapToTargetIncludingScale,
						PlayerCurrentWeapons.WeaponSlot_02->GetWeaponData().SocketName);

					bIsSecondSlotFull = true;

					CurrentWeaponSlotEnum = EWeaponSlots::EWS_SecondSlot;

					IsSuccessful = true;
				}

				else
					IsSuccessful = false;
			}

			else if (!bIsThirdSlotFull)
			{
				CurrentWeaponSlotEnum = EWeaponSlots::EWS_ThirdSlot;

				SpawnNewWeapon(WeaponSub, CurrentName, IsSuccessful);
			}

			else
			{
				bHasOpenSlot = false;

				IsSuccessful = false;

				CurrentWeaponSlotEnum = EWeaponSlots::EWS_SecondSlot;
			}

			break;

		case EWeaponSlots::EWS_ThirdSlot:

			if (!bIsThirdSlotFull)
			{
				PlayerCurrentWeapons.WeaponSlot_03 = GetWorld()->SpawnActor<AWeaponBase>(WeaponSub, LocalTransform, Params);

				if (IsValid(PlayerCurrentWeapons.WeaponSlot_03))
				{
					CurrentWeapons.Emplace(CurrentName, PlayerCurrentWeapons.WeaponSlot_03);

					CurrentWeapons[CurrentName]->AttachToComponent(Arms, FAttachmentTransformRules::SnapToTargetIncludingScale,
						PlayerCurrentWeapons.WeaponSlot_03->GetWeaponData().SocketName);

					bIsThirdSlotFull = true;

					CurrentWeaponSlotEnum = EWeaponSlots::EWS_ThirdSlot;

					IsSuccessful = true;
				}

				else
					IsSuccessful = false;
			}

			else if (!bIsFourthSlotFull)
			{
				CurrentWeaponSlotEnum = EWeaponSlots::EWS_FourthSlot;

				SpawnNewWeapon(WeaponSub, CurrentName, IsSuccessful);
			}

			else
			{
				bHasOpenSlot = false;

				IsSuccessful = false;

				CurrentWeaponSlotEnum = EWeaponSlots::EWS_ThirdSlot;
			}
			break;

		case EWeaponSlots::EWS_FourthSlot:

			if (!bIsFourthSlotFull)
			{
				PlayerCurrentWeapons.WeaponSlot_04 = GetWorld()->SpawnActor<AWeaponBase>(WeaponSub, LocalTransform, Params);

				if (IsValid(PlayerCurrentWeapons.WeaponSlot_04))
				{
					CurrentWeapons.Emplace(CurrentName, PlayerCurrentWeapons.WeaponSlot_04);

					CurrentWeapons[CurrentName]->AttachToComponent(Arms, FAttachmentTransformRules::SnapToTargetIncludingScale,
						PlayerCurrentWeapons.WeaponSlot_04->GetWeaponData().SocketName);

					bIsFourthSlotFull = true;

					CurrentWeaponSlotEnum = EWeaponSlots::EWS_FourthSlot;

					IsSuccessful = true;
				}

				else
					IsSuccessful = false;
			}

			else if (!bIsFirstSlotFull)
			{
				CurrentWeaponSlotEnum = EWeaponSlots::EWS_FirstSlot;

				SpawnNewWeapon(WeaponSub, CurrentName, IsSuccessful);
			}

			else
			{
				bHasOpenSlot = false;

				IsSuccessful = false;

				CurrentWeaponSlotEnum = EWeaponSlots::EWS_FourthSlot;
			}

			break;

		default:
			break;
		}
	}
}

void APlayerCharacter::PlayerFireWeapon()
{
	int32 LocalIndex = static_cast<int32>(CurrentPlayerWeapon);

	if (PlayerWeaponFireMontage.IsValidIndex(LocalIndex))
		PlayerAnimInstance->Montage_Play(PlayerWeaponFireMontage[LocalIndex]);

	else
		return;
}

void APlayerCharacter::PlayerReloadWeapon()
{
	int32 LocalIndex = static_cast<int32>(CurrentPlayerWeapon);

	if (PlayerWeaponReloadMontage.IsValidIndex(LocalIndex))
		PlayerAnimInstance->Montage_Play(PlayerWeaponReloadMontage[LocalIndex]);

	else
		return;
}

void APlayerCharacter::ShowShell()
{
	if (IsValid(ShotgunShellMesh))
		ShotgunShellMesh->SetHiddenInGame(false);
}

void APlayerCharacter::HideShell()
{
	if (IsValid(ShotgunShellMesh))
		ShotgunShellMesh->SetHiddenInGame(true);
}

APlayerCharacter* APlayerCharacter::GetPlayerRef_Implementation() { return this; }
