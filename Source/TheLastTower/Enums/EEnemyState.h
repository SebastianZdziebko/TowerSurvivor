#pragma once

#include "CoreMinimal.h"
#include "EEnemyState.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	None		UMETA(DisplayName = "None"),
	Walking		UMETA(DisplayName = "Walking"),
	Attacking	UMETA(DisplayName = "Attacking"),
	Waiting		UMETA(DisplayName = "Waiting"),
	Dying		UMETA(DisplayName = "Dying"),
	Decaying	UMETA(DisplayName = "Decaying")
};