#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "TowerSurvivor/Enums/EStat.h"

#include "StatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnZeroHealthSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthUpdateSignature, 
												int32, CurrentHealth, 
												int32, MaxHealth);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnApplyStatUpgradeSignature, EStat, int32);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOWERSURVIVOR_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatsComponent();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")		TMap<EStat, int32> Stats;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")	TMap<EStat, float> Modifiers;

	UPROPERTY(BlueprintAssignable) FOnZeroHealthSignature		OnZeroHealthDelegate;
	UPROPERTY(BlueprintAssignable) FOnHealthUpdateSignature		OnHealthUpdateDelegate;

	FOnApplyStatUpgradeSignature OnApplyStatUpgradeDelegate;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable) void	IncreaseStat(EStat StatToIncrease, int32 Amount, bool bUseModifier);
	UFUNCTION(BlueprintCallable) void	DecreaseStat(EStat StatToDecrease, int32 Amount);
	UFUNCTION(BlueprintCallable) void	ModifyStat(EStat StatToModify, float Modifier);
	UFUNCTION(BlueprintCallable) void	IncreaseModifier(EStat ModifierToIncrease, float Modifier);

	void ManageStatUpgrade(EStat Stat, float StatModifier, int32 FlatAmount);
	void ManageSingleUse(EStat Stat, int32 Cost, int32 Reward);
};