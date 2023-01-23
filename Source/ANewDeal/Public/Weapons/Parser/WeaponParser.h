#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponParser.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANEWDEAL_API UWeaponParser : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponParser();

	FORCEINLINE void SetObjectData(FString InObjectString) { ObjectString = InObjectString; }

public:
	void WeaponParser();
		
private:
	FString ObjectString;
};
