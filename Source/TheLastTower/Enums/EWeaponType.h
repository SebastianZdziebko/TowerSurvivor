#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8 
{
	None		UMETA(DisplayName = "None"),
	Fire		UMETA(DisplayName = "Fire Weapon"),
	Energy		UMETA(DisplayName = "Magic Weapon")
};