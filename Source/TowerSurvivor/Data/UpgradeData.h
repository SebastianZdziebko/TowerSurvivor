#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "TowerSurvivor/Enums/EWeaponType.h"
#include "TowerSurvivor/Enums/EStat.h"

#include "UpgradeData.generated.h"

UENUM(BlueprintType)
enum class EUpgradeType : uint8
{
	None		UMETA(DisplayName = "None"),
	Income		UMETA(DisplayName = "Income"),
	Damage		UMETA(DisplayName = "Damage"),
	Defence		UMETA(DisplayName = "Defence"),
	Single		UMETA(DisplayName = "Single Use")
};

UCLASS(BlueprintType)
class TOWERSURVIVOR_API UUpgradeData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base") EUpgradeType				Type	{ EUpgradeType::None };
	// INCOME type variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Income", meta = (EditCondition = "Type == EUpgradeType::Income", EditConditionHides)) int32		FlatIncome		{0};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Income", meta = (EditCondition = "Type == EUpgradeType::Income", EditConditionHides)) float		IncomeModifier	{0.f};

	// DAMAGE type variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (EditCondition = "Type == EUpgradeType::Damage", EditConditionHides)) EWeaponType WeaponType		{ EWeaponType::None };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage", meta = (EditCondition = "Type == EUpgradeType::Damage", EditConditionHides)) float		 DamageModifier	{ 0.f };

	// DEFENCE type variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defence", meta = (EditCondition = "Type == EUpgradeType::Defence", EditConditionHides)) EStat	StatType		{ EStat::None };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Defence", meta = (EditCondition = "Type == EUpgradeType::Defence", EditConditionHides)) int32	ChangeValue		{ 0 };

	// SINGLE USE type variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Single Use", meta = (EditCondition = "Type == EUpgradeType::Single", EditConditionHides)) EStat	SpecialCostType { EStat::None };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Single Use", meta = (EditCondition = "Type == EUpgradeType::Single", EditConditionHides)) int32	Cost			{ 0 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Single Use", meta = (EditCondition = "Type == EUpgradeType::Single", EditConditionHides)) int32	Reward			{ 0 };
};