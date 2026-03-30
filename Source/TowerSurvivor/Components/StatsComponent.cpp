#include "StatsComponent.h"

#include "Math/UnrealMathUtility.h"

#include "TowerSurvivor/Enums/EStat.h"

UStatsComponent::UStatsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UStatsComponent::IncreaseStat(EStat StatToIncrease, int32 Amount)
{
	switch (StatToIncrease)
	{
	case EStat::Health:

		Stats[EStat::Health] += Amount;
		Stats[EStat::Health] = FMath::Clamp(Stats[EStat::Health], 0, Stats[EStat::MaxHealth]);

		OnHealthUpdateDelegate.Broadcast(Stats[EStat::Health], Stats[EStat::MaxHealth]);

		break;

	case EStat::MaxHealth:

		Stats[EStat::MaxHealth] += Amount;
		IncreaseStat(EStat::Health, Amount);

		break;

	default:
		Stats[StatToIncrease] += Amount;
		break;
	}
}

void UStatsComponent::DecreaseStat(EStat StatToDecrease, int32 Amount)
{
	switch (StatToDecrease)
	{
	case EStat::Health:

		Stats[EStat::Health] -= Amount;
		Stats[EStat::Health] = FMath::Clamp(Stats[EStat::Health], 0, Stats[EStat::MaxHealth]);

		if (Stats[EStat::Health] == 0)
		{
			if (IsValid(this) && OnZeroHealthDelegate.IsBound())
				OnZeroHealthDelegate.Broadcast();
		}

		OnHealthUpdateDelegate.Broadcast(Stats[EStat::Health], Stats[EStat::MaxHealth]);

		break;

	case EStat::MaxHealth:

		Stats[EStat::Health] -= Amount;
		Stats[EStat::MaxHealth] -= Amount;
		break;

	default:
		Stats[StatToDecrease] -= Amount;
		break;
	}
}

void UStatsComponent::ModifyStat(EStat StatToModify, float Modifier)
{
	switch (StatToModify)
	{
	case EStat::Health:

		Stats[EStat::Health] *= Modifier;
		Stats[EStat::Health] = FMath::Clamp(Stats[EStat::Health], 0, Stats[EStat::MaxHealth]);

		OnHealthUpdateDelegate.Broadcast(Stats[EStat::Health], Stats[EStat::MaxHealth]);

		break;

	case EStat::MaxHealth:

		Stats[EStat::MaxHealth] *= Modifier;
		ModifyStat(EStat::Health, Modifier);

		break;

	default:
		Stats[StatToModify] *= Modifier;
		break;
	}
}