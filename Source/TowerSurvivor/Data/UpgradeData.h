#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "TowerSurvivor/Enums/EWeaponType.h"
#include "TowerSurvivor/Enums/EStat.h"

#include "UpgradeData.generated.h"

class UUpgrade;

UENUM(BlueprintType)
enum class EUpgradeType : uint8
{
	None		UMETA(DisplayName = "None"),
	Income		UMETA(DisplayName = "Income"),
	Damage		UMETA(DisplayName = "Damage"),
	Stats		UMETA(DisplayName = "Stats"),
	Single		UMETA(DisplayName = "Single Use")
};

UCLASS(BlueprintType)
class TOWERSURVIVOR_API UUpgradeData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base")	EUpgradeType			Type	{ EUpgradeType::None };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base", meta = (EditCondition = "Type != EUpgradeType::Single", EditConditionHides))	int32			FlatValue		{ 0 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base", meta = (EditCondition = "Type != EUpgradeType::Single", EditConditionHides))	float			Modifier		{ 0.f };

	// Timer
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base", meta = (EditCondition = "Type != EUpgradeType::Single", EditConditionHides))				bool	bUseTimer		{ false };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base", meta = (EditCondition = "Type != EUpgradeType::Single && bUseTimer", EditConditionHides))	float	UpgradePeriod	{ 0.f };

	// DAMAGE variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (EditCondition = "Type == EUpgradeType::Damage", EditConditionHides))
	EWeaponType WeaponType	{ EWeaponType::None };

	// STATS variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta = (EditCondition = "Type == EUpgradeType::Stats", EditConditionHides))
	EStat	StatType		{ EStat::None };

	// SINGLE USE type variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Single Use", meta = (EditCondition = "Type == EUpgradeType::Single", EditConditionHides)) EStat	SpecialCostType { EStat::None };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Single Use", meta = (EditCondition = "Type == EUpgradeType::Single", EditConditionHides)) int32	Cost			{ 0 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Single Use", meta = (EditCondition = "Type == EUpgradeType::Single", EditConditionHides)) int32	Reward			{ 0 };
};