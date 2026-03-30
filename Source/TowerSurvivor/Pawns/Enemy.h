#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "TowerSurvivor/Interfaces/CombatInterface.h"
#include "TowerSurvivor/Enums/EEnemyState.h"
#include "TowerSurvivor/Enums/EStat.h"

#include "Enemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDeathSignature, AEnemy*, DeadEnemy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyStateChangeSignature, EEnemyState, NewState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyAttackSignature);

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(
	FOnEnemyDestroySignature,
	AEnemy, 
	OnEnemyDestroy
);

USTRUCT(BlueprintType)
struct FEnemyUpgradeData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)	bool	IsLinear;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float	Value;

	FEnemyUpgradeData()
	{
		IsLinear	= false;
		Value		= 0.f;
	}

	bool IsValidUpgrade() const
	{
		if (Value != 0.f) return true;
		else return false;
	}
};

USTRUCT(BlueprintType)
struct FEnemyUpgradeDataArray
{
	GENERATED_BODY()


};

UCLASS()
class TOWERSURVIVOR_API AEnemy : public APawn, public ICombatInterface
{
	GENERATED_BODY()

	TEnumAsByte<ECollisionChannel> RangeChannel		{ ECC_GameTraceChannel1 };
	TEnumAsByte<ECollisionChannel> EnemyChannel		{ ECC_GameTraceChannel2 };
	TEnumAsByte<ECollisionChannel> TowerChannel		{ ECC_GameTraceChannel3 };
	TEnumAsByte<ECollisionChannel> ProjectileChannel{ ECC_GameTraceChannel4 };

	FTimerHandle AttackHandle;
	FTimerHandle DecayHandle;

	AActor* PlayerTower;
	FVector TowerLocation;

public:
	AEnemy();

	UPROPERTY(BlueprintAssignable)	FOnEnemyAttackSignature			OnEnemyAttack;	
	UPROPERTY(BlueprintAssignable)	FOnEnemyDeathSignature			OnEnemyDeath;
	UPROPERTY(BlueprintAssignable)	FOnEnemyStateChangeSignature	OnEnemyStateChange;
	FOnEnemyDestroySignature		OnEnemyDestroy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStatsComponent*				EnemyStatsComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UEnemyMovementComponent*		MovementComp;

	UPROPERTY(EditAnywhere)
	class USphereComponent*				CollisionComp;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent*				SkeletalMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWidgetComponent*				HealthBarWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Behavior") EEnemyState EnemyState { EEnemyState::None };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")	 float		 FallOffset	{ 0.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")	FVector		SpawnOffset		{ FVector::ZeroVector };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")	bool		bIsSwarm		{ false };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") float	AttackCooldown	{ 1.f };

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upgrades") TMap<EStat, TArray<FEnemyUpgradeData>> Upgrades;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void TowerIsInRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void TakeDamage_Implementation(int32 Amount) override;

	void StartAttack();

	UFUNCTION(BlueprintCallable)	void HandleAttackHit();
	UFUNCTION()						void RemoveFromGame();
	UFUNCTION()						void KillEnemy();
	UFUNCTION()						void OverlapGround(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
													   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
											   		   bool bFromSweep, const FHitResult& SweepResult);


	UFUNCTION(BlueprintCallable)	void SetEnemyState(EEnemyState NewState);
	UFUNCTION()						void ApplyUpgrades(const int32 CurrentStage);
};