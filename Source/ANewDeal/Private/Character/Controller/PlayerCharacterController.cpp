#include "Character/Controller/PlayerCharacterController.h"
#include "Character/Player/PlayerCharacter.h"
#include "Weapons/WeaponBase/WeaponBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

APlayerCharacterController::APlayerCharacterController()
{
}

void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = IPlayerRefInterface::Execute_GetPlayerRef(GetCharacter());
}

void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (TObjectPtr<UEnhancedInputComponent> PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (JumpAction)
		{
			PlayerEnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacterController::Jump);
			PlayerEnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacterController::StopJump);
		}

		if (WeaponFireAction)
		{
			PlayerEnhancedInputComponent->BindAction(WeaponFireAction, ETriggerEvent::Started, this, &APlayerCharacterController::WeaponFire);
			PlayerEnhancedInputComponent->BindAction(WeaponFireAction, ETriggerEvent::Completed, this, &APlayerCharacterController::StopFiringWeapon);
		}

		if (WeaponReloadAction)
			PlayerEnhancedInputComponent->BindAction(WeaponReloadAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::WeaponReload);

		if (MovementAction)
			PlayerEnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::Move);

		if (LookAction)
			PlayerEnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::Look);

		if (InteractAction)
			PlayerEnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::Interact);
	}
}

void APlayerCharacterController::Move(const FInputActionValue& Value)
{
	if (Value.GetMagnitude() != 0.F)
	{
		GetPawn()->AddMovementInput(GetPawn()->GetActorForwardVector(), Value[1]);
		GetPawn()->AddMovementInput(GetPawn()->GetActorRightVector(), Value[0]);
	}
}

void APlayerCharacterController::Look(const FInputActionValue& Value)
{
	AddPitchInput(Value[1] * -1);
	AddYawInput(Value[0]);
}

void APlayerCharacterController::Interact()
{
}

void APlayerCharacterController::Jump() { GetCharacter()->Jump(); }

void APlayerCharacterController::StopJump() { GetCharacter()->StopJumping(); }

void APlayerCharacterController::WeaponFire()
{
	if (!IsValid(PlayerRef))
		return;

	const EWeaponName& LocalWeaponName = PlayerRef->GetCurrentPlayerWeaponName();

	const EWeaponFireType& LocalFireType = PlayerRef->GetCurrentWeapons()[LocalWeaponName]->GetWeaponData().FireType;

	const TObjectPtr<UAnimInstance>& LocalInstance = PlayerRef->GetCurrentWeapons()[LocalWeaponName]->GetWeaponAnimInstance();

	const TObjectPtr<UAnimMontage>& LocalMontage = PlayerRef->GetCurrentWeapons()[LocalWeaponName]->GetWeaponFireMontage();

	const TObjectPtr<UAnimMontage>& LocalReloadMontage = PlayerRef->GetCurrentWeapons()[LocalWeaponName]->GetWeaponReloadMontage();

	if (PlayerRef->GetCurrentWeapons()[LocalWeaponName]->MagHasAmmo() && !PlayerRef->GetCurrentWeapons()[LocalWeaponName]->IsWeaponFiringOrReloading(LocalInstance, LocalMontage) &&
			!PlayerRef->GetCurrentWeapons()[LocalWeaponName]->IsWeaponFiringOrReloading(LocalInstance, LocalReloadMontage))
				PlayerRef->GetCurrentWeapons()[LocalWeaponName]->WeaponFire(LocalFireType, LocalInstance, LocalMontage);
}

void APlayerCharacterController::StopFiringWeapon()
{
	const EWeaponName& LocalWeaponName = PlayerRef->GetCurrentPlayerWeaponName();

	PlayerRef->GetCurrentWeapons()[LocalWeaponName]->StopFire();
}

void APlayerCharacterController::WeaponReload()
{
	if (!IsValid(PlayerRef))
		return;

	const EWeaponName& LocalWeaponName = PlayerRef->GetCurrentPlayerWeaponName();

	const EWeaponFireType& LocalFireType = PlayerRef->GetCurrentWeapons()[LocalWeaponName]->GetWeaponData().FireType;

	const TObjectPtr<UAnimInstance>& LocalInstance = PlayerRef->GetCurrentWeapons()[LocalWeaponName]->GetWeaponAnimInstance();

	const TObjectPtr<UAnimMontage>& LocalMontage = PlayerRef->GetCurrentWeapons()[LocalWeaponName]->GetWeaponFireMontage();

	const TObjectPtr<UAnimMontage>& LocalReloadMontage = PlayerRef->GetCurrentWeapons()[LocalWeaponName]->GetWeaponReloadMontage();

	if (PlayerRef->GetCurrentWeapons()[LocalWeaponName]->HasAmmoForReload() && !PlayerRef->GetCurrentWeapons()[LocalWeaponName]->IsMagFull())
	{
		if (!PlayerRef->GetCurrentWeapons()[LocalWeaponName]->IsWeaponFiringOrReloading(LocalInstance, LocalMontage) && 
				!PlayerRef->GetCurrentWeapons()[LocalWeaponName]->IsWeaponFiringOrReloading(LocalInstance, LocalReloadMontage))
		{
			PlayerRef->GetCurrentWeapons()[LocalWeaponName]->WeaponReload(LocalFireType, LocalInstance, LocalReloadMontage);
		}
	}			
}


