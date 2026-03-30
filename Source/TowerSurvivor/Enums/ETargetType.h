#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ETargetType : uint8
{
	None		UMETA(DisplayName = "None"),
	Single		UMETA(DisplayName = "Single Target"),
	Multi		UMETA(DisplayName = "Multi Target"),
	Bounce		UMETA(DisplayName = "Bounce Target")
};