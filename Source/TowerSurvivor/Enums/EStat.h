#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EStat : uint8
{
	None			UMETA(DisplayName = "None"),
	Health			UMETA(DisplayName = "Health"),
	MaxHealth		UMETA(DisplayName = "MaxHealth"),
	RegHealth		UMETA(DisplayName = "Regeneration"),
	Speed			UMETA(DisplayName = "Speed"),
	Damage			UMETA(DisplayName = "Damage")
};