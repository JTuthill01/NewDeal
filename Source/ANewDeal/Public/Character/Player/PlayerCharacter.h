#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/PlayerRef/PlayerRefInterface.h"
#include "Structs/CustomLinearColors.h"
#include "Enums/WeaponsEnums/WeaponEnums.h"
#include "PlayerCharacter.generated.h"

class AWeaponBase;

UENUM(BlueprintType)
enum class EWeaponSlots : uint8
{
	EWS_Default UMETA(DisplayName = "Default"), EWS_FirstSlot UMETA(DisplayName = "Slot One"), EWS_SecondSlot UMETA(DisplayName = "Slot Two"), EWS_ThirdSlot UMETA(DisplayName = "Slot Three"),
		EWS_FourthSlot UMETA(DisplayName = "Slot Four")
};

USTRUCT(BlueprintType)
struct FCurrentWeapons
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<AWeaponBase> WeaponSlot_01 = nullptr;

	UPROPERTY()
	TObjectPtr<AWeaponBase> WeaponSlot_02 = nullptr;

	UPROPERTY()
	TObjectPtr<AWeaponBase> WeaponSlot_03 = nullptr;

	UPROPERTY()
	TObjectPtr<AWeaponBase> WeaponSlot_04 = nullptr;
};

UCLASS()
class ANEWDEAL_API APlayerCharacter : public ACharacter, public IPlayerRefInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	/** Getters C++ ONLY */
	FORCEINLINE TObjectPtr<USkeletalMeshComponent> GetPlayerArms() const { return Arms; }
	FORCEINLINE TObjectPtr<class UCameraComponent> GetPlayerCamera() const { return Camera; }
	FORCEINLINE TObjectPtr<UAnimInstance> GetPlayerAnimInstance() const { return PlayerAnimInstance; }

	FORCEINLINE FCurrentWeapons GetPlayerCurrentWeapons() const { return PlayerCurrentWeapons; }
	FORCEINLINE TMap<EWeaponName, TObjectPtr<class AWeaponBase>> GetCurrentWeapons() const { return CurrentWeapons; }

	/* Shotgun Reload Montages **/
	FORCEINLINE TArray<TObjectPtr<UAnimMontage>> GetAmericanReloadMonatge() const { return AmericanReloadMonatge; }
	FORCEINLINE TArray<TObjectPtr<UAnimMontage>> GetItalianReloadMonatge() const { return ItalianReloadMonatge; }
	FORCEINLINE TArray<TObjectPtr<UAnimMontage>> GetBulldogReloadMonatge() const { return BulldogReloadMonatge; }


	UFUNCTION(BlueprintPure)
	EWeaponName GetCurrentPlayerWeaponName() { return CurrentPlayerWeapon; }

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called on Player Death 
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void PawnClientRestart() override;

	APlayerCharacter* GetPlayerRef_Implementation() override;

public:
	void InteractWithObject();

	void SpawnNewWeapon(TSubclassOf<class AWeaponBase> WeaponSub, EWeaponName CurrentName, bool& IsSuccessful);

	void PlayerFireWeapon();

	void PlayerReloadWeapon();

	void ShowShell();

	void HideShell();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void PlayerDeath();

private:
	void PlayerSetup();

	void SpawnInitialWeapon();

	void ScanForInteractables();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	TMap<EWeaponName, TObjectPtr<class AWeaponBase>> CurrentWeapons;

protected:
	//Mapping Contexts//
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = InputMappings)
	TObjectPtr<class UInputMappingContext> PlayerBaseMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = InputMappings)
	int32 BaseMappingPriority = 0;

private:
	UPROPERTY()
	TObjectPtr<UAnimInstance> PlayerAnimInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HealthComp, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPlayerHealthComponent> PlayerHealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SK_Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Arms;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SK_Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> ShotgunShellMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Montages, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UAnimMontage>> PlayerWeaponFireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Montages, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UAnimMontage>> PlayerWeaponReloadMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Montages, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UAnimMontage>> AmericanReloadMonatge;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Montages, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UAnimMontage>> ItalianReloadMonatge;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Montages, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UAnimMontage>> BulldogReloadMonatge;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AWeaponBase> InitialWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	EWeaponName CurrentPlayerWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Colors, meta = (AllowPrivateAccess = "true"))
	FCustomLinearColors CustomHUDColors;

private:
	float InteractableTraceTimer;

	FTimerHandle InteractableTraceTimerHandle;

	EWeaponSlots CurrentWeaponSlotEnum;

	FCurrentWeapons PlayerCurrentWeapons;

	bool bIsFirstSlotFull;
	bool bIsSecondSlotFull;
	bool bIsThirdSlotFull;
	bool bIsFourthSlotFull;
	bool bHasOpenSlot;
	bool bIsSuccessful;
};
