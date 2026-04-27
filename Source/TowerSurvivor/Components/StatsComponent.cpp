#include "StatsComponent.h"

#include "Math/UnrealMathUtility.h"

#include "TowerSurvivor/Enums/EStat.h"
#include "TowerSurvivor/Components/UpgradeSystemComponent.h"

UStatsComponent::UStatsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	UUpgradeSystemComponent* OwnerUpgradeComp = GetOwner()->FindComponentByClass<UUpgradeSystemComponent>();

	if (IsValid(OwnerUpgradeComp))
	{
		OwnerUpgradeComp->OnStatUpgradeDelegate.AddUObject(this, &UStatsComponent::ManageStatUpgrade);
		OwnerUpgradeComp->OnSingleUseUpgradeDelegate.AddUObject(this, &UStatsComponent::ManageSingleUse);
	}
}

void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UStatsComponent::IncreaseStat(EStat StatToIncrease, int32 Amount, bool bUseModifier)
{
	int32 FinalAmount = Amount;

	if(bUseModifier)
		if (float* ExistingModifier = Modifiers.Find(StatToIncrease))
		{
			float BaseAmount		= static_cast<float>(Amount);
			float ModifiedAmount	= BaseAmount + BaseAmount * *(ExistingModifier);
			FinalAmount				= static_cast<int32>(ModifiedAmount);
		}

	switch (StatToIncrease)
	{
	case EStat::Health:

		Stats[EStat::Health] += FinalAmount;
		Stats[EStat::Health] = FMath::Clamp(Stats[EStat::Health], 0, Stats[EStat::MaxHealth]);

		OnHealthUpdateDelegate.Broadcast(Stats[EStat::Health], Stats[EStat::MaxHealth]);

		break;

	case EStat::MaxHealth:

		Stats[EStat::MaxHealth] += FinalAmount;
		IncreaseStat(EStat::Health, FinalAmount, false);

		break;

	default:
		Stats.FindOrAdd(StatToIncrease) += FinalAmount;
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

void UStatsComponent::IncreaseModifier(EStat ModifierToIncrease, float Modifier)
{
	if (ModifierToIncrease == EStat::None || Modifier <= 0.f) return;

	if (float* FoundModifier = Modifiers.Find(ModifierToIncrease))
		*FoundModifier += Modifier;
	else
		Modifiers.Add(ModifierToIncrease, Modifier);
}

void UStatsComponent::ManageStatUpgrade(EStat Stat, float StatModifier, int32 FlatAmount)
{
	if (Stat == EStat::None) return;

	if (FlatAmount > 0)		IncreaseStat(Stat, FlatAmount, true);
	if (StatModifier > 0)	IncreaseModifier(Stat, StatModifier);
}

void UStatsComponent::ManageSingleUse(EStat Stat, int32 Cost, int32 /*Reward*/)
{
	if (Stat != EStat::None && Cost > 0) DecreaseStat(Stat, Cost);
}