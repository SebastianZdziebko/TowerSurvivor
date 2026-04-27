#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UpgradeSystemComponent.generated.h"

class UUpgrade;

enum class EWeaponType : uint8;
enum class EStat : uint8;

DECLARE_MULTICAST_DELEGATE_ThreeParams(OnSingleUseUpgradeSignature, EStat, int32, int32);
DECLARE_MULTICAST_DELEGATE_ThreeParams(OnStatUpgradeSignature, EStat, float, int32);
DECLARE_MULTICAST_DELEGATE_TwoParams(OnDamageUpgradeSignature, EWeaponType, float);
DECLARE_MULTICAST_DELEGATE_TwoParams(OnIncomeUpgradeSignature, float, int32);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TOWERSURVIVOR_API UUpgradeSystemComponent : public UActorComponent
{
	GENERATED_BODY()

	TMap<TSubclassOf<UUpgrade>, int32>			ActiveUpgrades;
	TMap<TSubclassOf<UUpgrade>, FTimerHandle>	ActiveUpgradesHandles;

public:
	UUpgradeSystemComponent();

	UPROPERTY() TArray<UUpgrade*> UpgradesInstances;

	OnDamageUpgradeSignature	OnDamageUpgradeDelegate;
	OnStatUpgradeSignature		OnStatUpgradeDelegate;
	OnIncomeUpgradeSignature	OnIncomeUpgradeDelegate;
	OnSingleUseUpgradeSignature OnSingleUseUpgradeDelegate;

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Upgrade Component") void	AddActiveUpgrade(TSubclassOf<UUpgrade> Upgrade);
	UFUNCTION(BlueprintCallable, Category = "Upgrade Component") void	ManageUpgrade(TSubclassOf<UUpgrade> Upgrade);
	UFUNCTION(BlueprintCallable, Category = "Upgrade Component") void	SetupUpgradeTimer(TSubclassOf<UUpgrade> Upgrade);

	int32* CheckActiveUpgrade(TSubclassOf<UUpgrade> Upgrade);
};