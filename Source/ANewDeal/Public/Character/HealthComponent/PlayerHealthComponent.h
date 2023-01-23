#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthAndArmorUpdate, float, ArmorUpdate, float, HealthUpdate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANEWDEAL_API UPlayerHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerHealthComponent();

public:
	FORCEINLINE bool IsPlayerAlive() { return CurrentHealth > 0.F; }
	FORCEINLINE bool HasFullHealth() { return CurrentHealth >= MaxHealth; }
	FORCEINLINE bool HasFullArmor() { return CurrentArmor >= MaxArmor; }

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
	void ChangeHealthAndArmor(float ChangeAmount);

	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health)
	float MaxArmor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health)
	float CurrentArmor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health)
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health)
	float CurrentHealth;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerDeath OnPlayerDeath;

	UPROPERTY(BlueprintAssignable)
	FOnHealthAndArmorUpdate OnPlayerHealthAndArmorUpdate;
};
