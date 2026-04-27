#include "UpgradeSystemComponent.h"

#include "TowerSurvivor/Components/WalletComponent.h"
#include "TowerSurvivor/Components/StatsComponent.h"

#include "TowerSurvivor/Pawns/PlayerTower.h"
#include "TowerSurvivor/Objects/Upgrade.h"
#include "TowerSurvivor/Data/UpgradeData.h"

#include "TowerSurvivor/Enums/EWeaponType.h"
#include "TowerSurvivor/Enums/EStat.h"

UUpgradeSystemComponent::UUpgradeSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UUpgradeSystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UUpgradeSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

int32* UUpgradeSystemComponent::CheckActiveUpgrade(TSubclassOf<UUpgrade> Upgrade)
{
	if (int32* UpgradeCount = ActiveUpgrades.Find(Upgrade))
		return UpgradeCount;
	else 
		return nullptr;
}

void UUpgradeSystemComponent::AddActiveUpgrade(TSubclassOf<UUpgrade> Upgrade)
{
	if (!IsValid(Upgrade)) return;

	int32* UpgradeCountPtr = CheckActiveUpgrade(Upgrade);

	if (UpgradeCountPtr)
		(*UpgradeCountPtr)++;
	else
	{
		ActiveUpgrades.Add(Upgrade, 1);
		SetupUpgradeTimer(Upgrade);
	}
}

void UUpgradeSystemComponent::ManageUpgrade(TSubclassOf<UUpgrade> Upgrade)
{
	if (!IsValid(Upgrade)) return;

	const UUpgrade*		DefaultUpgrade	= Upgrade->GetDefaultObject<UUpgrade>();
	const UUpgradeData*	DefaultData		= DefaultUpgrade->UpgradeData;
	const EUpgradeType	UpgradeType		= DefaultUpgrade->UpgradeData->Type;

	switch (UpgradeType)
	{
	case EUpgradeType::Income:
	{

		const float IncomeModifier	= DefaultData->Modifier;
		const int32 FlatIncome = DefaultData->FlatValue;

		OnIncomeUpgradeDelegate.Broadcast(IncomeModifier, FlatIncome);
		break;
	}
	case EUpgradeType::Damage:
	{
		const EWeaponType	DamageType		= DefaultData->WeaponType;
		const float			DamageModifier	= DefaultData->Modifier;

		OnDamageUpgradeDelegate.Broadcast(DamageType, DamageModifier);
		break;
	}
	case EUpgradeType::Stats:
	{
		const EStat StatType		= DefaultData->StatType;
		const float StatModifier	= DefaultData->Modifier;
		const int32 FlatStat		= DefaultData->FlatValue;

		OnStatUpgradeDelegate.Broadcast(StatType, StatModifier, FlatStat);
		break;
	}
	case EUpgradeType::Single:
	{
		const EStat CostType	= DefaultData->SpecialCostType;
		const int32 Cost		= DefaultData->Cost;
		const int32 Reward		= DefaultData->Reward;

		OnSingleUseUpgradeDelegate.Broadcast(CostType, Cost, Reward);
		break;
	}
	default:
		break;
	}
}

void UUpgradeSystemComponent::SetupUpgradeTimer(TSubclassOf<UUpgrade> Upgrade)
{
	if (ActiveUpgradesHandles.Contains(Upgrade)) return;

	UUpgrade* UpgradeInstance = NewObject<UUpgrade>(this, Upgrade);

	if (!IsValid(UpgradeInstance)) return;

	const float UpgradePeriod = UpgradeInstance->UpgradeData->UpgradePeriod;

	if (UpgradePeriod <= 0.f) return;

	FTimerHandle	NewHandle;
	FTimerDelegate	TimerEnd;
	TimerEnd.BindUFunction(UpgradeInstance, FName("ApplyUpgradeTick"));

	GetWorld()->GetTimerManager().SetTimer
	(
		NewHandle,
		TimerEnd,
		UpgradePeriod,
		true
	);

	ActiveUpgradesHandles.Add(Upgrade, NewHandle);
	UpgradesInstances.Add(UpgradeInstance);
}