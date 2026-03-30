#include "Enemy.h"


#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

#include "TowerSurvivor/Interfaces/CombatInterface.h"
#include "TowerSurvivor/Components/StatsComponent.h"
#include "TowerSurvivor/Components/EnemyMovementComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent	 =  CreateDefaultSubobject<USceneComponent>			(TEXT("Root Component"));
	MovementComp	 =	CreateDefaultSubobject<UEnemyMovementComponent>	(TEXT("Movement Component"));

	SkeletalMeshComp =  CreateDefaultSubobject<USkeletalMeshComponent>	(TEXT("Skeletal Mesh Component"));
	SkeletalMeshComp ->	SetupAttachment(RootComponent);
	SkeletalMeshComp -> SetCastShadow(false);

	EnemyStatsComp	 =  CreateDefaultSubobject<UStatsComponent>			(TEXT("Stats Component"));

	CollisionComp	 =  CreateDefaultSubobject<USphereComponent>		(TEXT("Collision Comp"));
	CollisionComp	 -> SetupAttachment(RootComponent);
	CollisionComp	 -> SetCollisionObjectType(EnemyChannel);
	CollisionComp	 -> SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp	 -> SetGenerateOverlapEvents(true);
	CollisionComp	 -> SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp	 -> SetCollisionResponseToChannel(RangeChannel, ECR_Overlap);
	CollisionComp	 -> SetCollisionResponseToChannel(TowerChannel, ECR_Overlap);
	CollisionComp	 -> SetCollisionResponseToChannel(ProjectileChannel, ECR_Overlap);
	CollisionComp	 -> SetCollisionResponseToChannel(EnemyChannel, ECR_Overlap);
	CollisionComp	 -> SetNotifyRigidBodyCollision(true);

	HealthBarWidget	 =	CreateDefaultSubobject<UWidgetComponent>		(TEXT("Health Bar"));
	HealthBarWidget	 ->	SetupAttachment(RootComponent);

	EnemyStatsComp	 -> OnZeroHealthDelegate.AddDynamic(this, &AEnemy::KillEnemy);
	CollisionComp	 -> OnComponentBeginOverlap.AddDynamic(this, &AEnemy::TowerIsInRange);
	MovementComp	 -> OnReachSinkMax.BindUObject(this, &AEnemy::RemoveFromGame);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	SetEnemyState(EEnemyState::Walking);
	PlayerTower		= UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	TowerLocation	= PlayerTower->GetActorLocation();

	if(MovementComp && EnemyStatsComp)
		MovementComp->MovementSpeed = EnemyStatsComp->Stats[EStat::Speed];
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (EnemyState) 
	{
		case EEnemyState::Walking:
		{
			if (!IsValid(MovementComp)) return;
			MovementComp->MoveTowardsTower(DeltaTime, TowerLocation, this);
			break;
		}
		case EEnemyState::Dying:
		{
			const float FallingSpeed = 40.f;

			FVector LineStart	= GetActorLocation();
			FVector LineEnd		= LineStart - FVector(0.f, 0.f, FallOffset);

			FHitResult Hit;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);

			bool bGroundInOffsetRange = GetWorld()->LineTraceSingleByChannel(Hit, LineStart, LineEnd, ECC_WorldStatic, Params);

			if (!bGroundInOffsetRange) MovementComp->Sinking(DeltaTime, FallingSpeed, this);
			else OverlapGround(nullptr, nullptr, nullptr, 0, false, FHitResult());

			break;
		}
		case EEnemyState::Decaying:
		{
			const float FallingSpeed = 10.f;

			MovementComp->Sinking(DeltaTime, FallingSpeed, this);
			break;
		}
		default:
			break;
	}
}

void AEnemy::TowerIsInRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
							bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetCollisionObjectType() != TowerChannel) return;

	GetWorldTimerManager().SetTimer
	(
		AttackHandle,
		this,
		&AEnemy::StartAttack,
		AttackCooldown,
		true,
		0.f
	);

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

	for (int32 UpgradeLevel = 0; UpgradeLevel <= CurrentStage; UpgradeLevel++)
	{
		if (!Upgrades.IsValidIndex(UpgradeLevel) || !Upgrades[UpgradeLevel].IsValidUpgrade()) continue;

		
	}
}