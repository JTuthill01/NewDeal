#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class ANEWDEAL_API IInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnInteractableFound();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnInteractWithObject();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnClearViewport();
};
