#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "TheLastTower/Interfaces/CombatInterface.h"
#include "TheLastTower/Enums/EEnemyState.h"
#include "TheLastTower/Enums/EStat.h"

#include "Enemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDeathSignature, AEnemy*, DeadEnemy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyStateChangeSignature, EEnemyState, NewState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyAttackSignature);

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(
	FOnEnemyDestroySignature,
	AEnemy, 
	OnEnemyDestroy
);

class UFloatingPawnMovement;
class USphereComponent;
class UStatsComponent;
class UWidgetComponent;

UCLASS()
class THELASTTOWER_API AEnemy : public APawn, public ICombatInterface
{
	GENERATED_BODY()

	FTimerHandle AttackHandle;
	FTimerHandle DecayHandle;

	AActor* PlayerTower;
	FVector TowerLocation;

public:
	AEnemy();

	// Event signature declarations
	UPROPERTY(BlueprintAssignable)	FOnEnemyAttackSignature			OnEnemyAttack;	
	UPROPERTY(BlueprintAssignable)	FOnEnemyDeathSignature			OnEnemyDeath;
	UPROPERTY(BlueprintAssignable)	FOnEnemyStateChangeSignature	OnEnemyStateChange;
	UPROPERTY()						FOnEnemyDestroySignature		OnEnemyDestroy;

	// Component declarations
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UStatsComponent*		EnemyStatsComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	UWidgetComponent*		HealthBarWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	USkeletalMeshComponent* SkeletalMeshComp;
	UPROPERTY(EditAnywhere)						USphereComponent*		CollisionComp;
	UPROPERTY(EditAnywhere)						UFloatingPawnMovement*	MovementComp;

	// Variables used to manage enemy behavior including falling under the map after death
	UPROPERTY(VisibleAnywhere,	BlueprintReadOnly,	Category = "Behavior")		EEnemyState		EnemyState		{ EEnemyState::None };
	UPROPERTY(EditAnywhere,		BlueprintReadWrite, Category = "Behavior")		float			FallOffset		{ 0.f };
	UPROPERTY(EditAnywhere,		BlueprintReadOnly,	Category = "Behavior")		float			SinkThreshold	{ 200.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")	FVector		SpawnOffset		{ FVector::ZeroVector };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")	bool		bIsSwarm		{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat") float		AttackCooldown	{ 1.f };

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void StartAttack();																// Change the enemy's state to Attacking and broadcast OnEnemyAttack event

	UFUNCTION(BlueprintCallable)	void HandleAttackHit();							// Deal damage to the player via the CombatInterface
	UFUNCTION()						void RemoveFromGame();							// Delete this enemy from the game
	UFUNCTION()						void KillEnemy();								// Manage enemy death without deleting it from game, safety clear the AttackTimer

	UFUNCTION()						void OverlapGround(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,			// Check collision with level ground, used when falling below the map
													   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,					// after the enemy's death
											   		   bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)	void TowerIsInRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,			// Check collision with COLLISION_TOWER, and start the attack sequence
														UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
														bool bFromSweep, const FHitResult& SweepResult);


	UFUNCTION(BlueprintCallable)	void SetEnemyState(EEnemyState NewState);		// Set a new state for enemy with validation and broadcast the OnEnemyStateChange event
	UFUNCTION()						void ApplyUpgrades(const int32 CurrentStage);	// Apply enemy stats upgrade based on the current game stage

	// Implementation of functions from the CombatInterface
	virtual void TakeDamage_Implementation(int32 Amount) override;
};