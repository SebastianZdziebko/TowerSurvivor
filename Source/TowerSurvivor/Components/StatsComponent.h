#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "TowerSurvivor/Enums/EStat.h"

#include "StatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnZeroHealthSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthUpdateSignature, 
												int32, CurrentHealth, 
												int32, MaxHealth);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOWERSURVIVOR_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatsComponent();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats") TMap<EStat, int32> Stats;

	UPROPERTY(BlueprintAssignable) FOnZeroHealthSignature		OnZeroHealthDelegate;
	UPROPERTY(BlueprintAssignable) FOnHealthUpdateSignature		OnHealthUpdateDelegate;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable) void IncreaseStat(EStat StatToIncrease, int32 Amount);
	UFUNCTION(BlueprintCallable) void DecreaseStat(EStat StatToDecrease, int32 Amount);
	UFUNCTION(BlueprintCallable) void ModifyStat(EStat StatToModify, float Modifier);
};