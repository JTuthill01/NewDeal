#pragma once

#include "CoreMinimal.h"
#include "EBBullet.h"
#include "EBBulletProjectile.generated.h"

UCLASS()
class ANEWDEAL_API AEBBulletProjectile : public AEBBullet
{
	GENERATED_BODY()
	
public:
	AEBBulletProjectile();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> EBProjectileMesh;
};
