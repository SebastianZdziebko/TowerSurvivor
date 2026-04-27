#include "Enemy.h"

#include "GameFramework/FloatingPawnMovement.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

#include "TowerSurvivor/Interfaces/CombatInterface.h"
#include "TowerSurvivor/Components/StatsComponent.h"
#include "TowerSurvivor/Controllers/EnemyMovementController.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp	 =  CreateDefaultSubobject<USphereComponent>		(TEXT("Collision Comp"));
	RootComponent	 =  CollisionComp;

	MovementComp		= CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement Component"));
	AIControllerClass	= AEnemyMovementController::StaticClass();
	AutoPossessAI		= EAutoPossessAI::PlacedInWorldOrSpawned;

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh Component"));
	SkeletalMeshComp->SetupAttachment(RootComponent);
	SkeletalMeshComp->SetCastShadow(false);

	EnemyStatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("Stats Component"));

	CollisionComp	 -> SetCollisionObjectType(EnemyChannel);
	CollisionComp	 -> SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp	 -> SetGenerateOverlapEvents(true);
	CollisionComp	 -> SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp	 -> SetCollisionResponseToChannel(RangeChannel, ECR_Overlap);
	CollisionComp	 -> SetCollisionResponseToChannel(TowerChannel, ECR_Overlap);
	CollisionComp	 -> SetCollisionResponseToChannel(ProjectileChannel, ECR_Overlap);
	CollisionComp	 -> SetCollisionResponseToChannel(EnemyChannel, ECR_Block);
	CollisionComp	 -> SetNotifyRigidBodyCollision(true);

	HealthBarWidget	 =	CreateDefaultSubobject<UWidgetComponent>		(TEXT("Health Bar"));
	HealthBarWidget	 ->	SetupAttachment(RootComponent);

	EnemyStatsComp	 -> OnZeroHealthDelegate.AddDynamic(this, &AEnemy::KillEnemy);
	CollisionComp	 -> OnComponentBeginOverlap.AddDynamic(this, &AEnemy::TowerIsInRange);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerTower		= UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	TowerLocation	= PlayerTower->GetActorLocation();

	if (MovementComp && EnemyStatsComp)
	{
		MovementComp->MaxSpeed = EnemyStatsComp->Stats[EStat::Speed];
		MovementComp->Acceleration = 600.f;
		MovementComp->Deceleration = 600.f;
	}

	SetEnemyState(EEnemyState::Walking);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (EnemyState) 
	{
		case EEnemyState::Walking:
		{
			FVector Velocity = GetVelocity();

			if (!Velocity.IsNearlyZero())
			{
				Velocity.Z = 0.f;

				FRotator TargetRotation = Velocity.Rotation();
				FRotator FinalRotation  = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 10.f);

				SetActorRotation(FinalRotation);
			}

			break;
		}
		case EEnemyState::Dying:
		{
			if (AController* EnemyController = GetController())
				EnemyController->UnPossess();

			const float FallingSpeed = 40.f;

			AddActorWorldOffset(FVector(0.f, 0.f, -FallingSpeed * DeltaTime));

			FVector LineStart	= GetActorLocation();
			FVector LineEnd		= LineStart - FVector(0.f, 0.f, FallOffset);

			FHitResult Hit;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);

			bool bGroundInOffsetRange = GetWorld()->LineTraceSingleByChannel(Hit, LineStart, LineEnd, ECC_WorldStatic, Params);

			if (bGroundInOffsetRange) 
				OverlapGround(nullptr, nullptr, nullptr, 0, false, FHitResult());

			break;
		}
		case EEnemyState::Decaying:
		{
			const float FallingSpeed = 10.f;

			AddActorWorldOffset(FVector(0.f, 0.f, -FallingSpeed * DeltaTime));

			break;
		}
		default:
		{
			FVector Velocity = GetVelocity();

			if (!Velocity.IsNearlyZero())
			{
				Velocity.Z = 0.f;

				FRotator TargetRotation = Velocity.Rotation();
				FRotator FinalRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 10.f);

				SetActorRotation(FinalRotation);
			}
			break;
		}
	}
}

void AEnemy::TowerIsInRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
							bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherComp) return;

	if (OtherComp->GetCollisionObjectType() != TowerChannel &&
		OtherComp->GetCollisionObjectType() != ECC_WorldDynamic) return;

	if (AEnemyMovementController* EnemyController = Cast<AEnemyMovementController>(GetController()))
		EnemyController->StopMovement();

	GetWorldTimerManager().SetTimer
	(
		AttackHandle,
		this,
		&AEnemy::StartAttack,
		AttackCooldown,
		true,
		0.f
	);

	if(GetController())
		GetController()->UnPossess();

	SetEnemyState(EEnemyState::Waiting);
}


void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::TakeDamage_Implementation(int32 Amount)
{
	if (Amount <= 0) return;

	EnemyStatsComp->DecreaseStat(EStat::Health, Amount);
}

void AEnemy::StartAttack()
{
	SetEnemyState(EEnemyState::Attacking);
	OnEnemyAttack.Broadcast();
}

void AEnemy::HandleAttackHit()
{
	if (!PlayerTower->Implements<UCombatInterface>()) return;

	int32 Damage = EnemyStatsComp->Stats[EStat::Damage];

	ICombatInterface::Execute_TakeDamage(PlayerTower, Damage);
}

void AEnemy::RemoveFromGame()
{
	OnEnemyDestroy.Broadcast();
	GetWorldTimerManager().ClearAllTimersForObject(this);
	Destroy();
}

void AEnemy::KillEnemy()
{
	SetEnemyState(EEnemyState::Dying);

	GetWorldTimerManager().ClearTimer(AttackHandle);

	if (CollisionComp)
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	OnEnemyDeath.Broadcast(this);
}

void AEnemy::OverlapGround(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
						   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, 
						   const FHitResult& SweepResult)
{
	if (GetWorldTimerManager().IsTimerActive(DecayHandle)) return;

	FTimerDelegate DecayDelegate;
	DecayDelegate.BindUObject(this, &AEnemy::SetEnemyState, EEnemyState::Decaying);

	GetWorldTimerManager().SetTimer
	(
		DecayHandle,
		DecayDelegate,
		5.f,
		false
	);
}

void AEnemy::SetEnemyState(EEnemyState NewState) 
{ 
	if (EnemyState == NewState) return;  
	
	EnemyState = NewState;
	OnEnemyStateChange.Broadcast(NewState);
}

void AEnemy::ApplyUpgrades(const int32 CurrentStage)
{
	if (EnemyStatsComp || CurrentStage == 0) return;

	/*
	for (int32 UpgradeLevel = 0; UpgradeLevel <= CurrentStage; UpgradeLevel++)
	{
		if (!Upgrades.IsValidIndex(UpgradeLevel) || !Upgrades[UpgradeLevel].IsValidUpgrade()) continue;

		
	}
	*/
}