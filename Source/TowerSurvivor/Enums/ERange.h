#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ERange : uint8
{
	None		UMETA(DisplayName = "None"),
	Short		UMETA(DisplayName = "Short Range"),
	Medium		UMETA(DisplayName = "Medium Range"),
	Long		UMETA(DisplayName = "Long Range"),
};

inline constexpr float RangeValues[] = { 0.f, 500.f, 1000.f, 1500.f };

inline constexpr float RangeToFloat(ERange Range) { return RangeValues[static_cast<uint8>(Range)]; }