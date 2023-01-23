#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AsyncTestActor.generated.h"

UCLASS()
class ANEWDEAL_API AAsyncTestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAsyncTestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
