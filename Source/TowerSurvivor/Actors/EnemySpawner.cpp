#include "EnemySpawner.h"

#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector AEnemySpawner::GetRandomSpawnLocation()
{
	if (FMath::IsNearlyZero(SpawnRadius, 10.f)) return FVector::ZeroVector;

	const float Angle = FMath::RandRange(0.f, 2.f * UE_PI);

	FVector SpawnerLocation = this->GetActorLocation();

	const float X = SpawnerLocation.X + SpawnRadius * FMath::Cos(Angle);
	const float Y = SpawnerLocation.Y + SpawnRadius * FMath::Sin(Angle);

	return FVector(X, Y, SpawnerLocation.Z);
}

TSubclassOf<AEnemy> AEnemySpawner::GetRandomEnemyToSpawn()
{
	if (OptionsToSpawn.IsEmpty()) return {};

	int32				RandomIndex = FMath::RandHelper(OptionsToSpawn.Num());
	TSubclassOf<AEnemy> RandomEnemy = OptionsToSpawn[RandomIndex];

	return RandomEnemy;
}

void AEnemySpawner::SpawnEnemy()
{
	TSubclassOf<AEnemy> EnemyBP	 = GetRandomEnemyToSpawn();
	AEnemy*				Enemy	 = Cast<AEnemy>(EnemyBP->GetDefaultObject());

	if (!IsValid(EnemyBP) || !Enemy) return;


	const float		CollisionRadius		= Enemy->CollisionComp->GetUnscaledSphereRadius();
	const float		CollisionDiameter	= CollisionRadius * 2;
	const FVector	SpawnLocation		= GetRandomSpawnLocation();
	const FVector	SpawnOffset			= Enemy->SpawnOffset;
	const FRotator	SpawnRotation		= FRotator::ZeroRotator;
	FCollisionShape CollisionShape		= FCollisionShape::MakeSphere(CollisionRadius);
	FActorSpawnParameters SpawnParam;

	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(EnemyChannel);

	if(Enemy->bIsSwarm)
		for (int i = 0; i < SwarmSize; i++)
		{
			const int32 MaxAttempts		= 5;
			int32		Attempts		= 0;
			bool		BFoundValidSpot	= false;
			FVector		FinalLocation;

			while (Attempts < MaxAttempts)
			{
				FVector SwarmOffset = FVector::ZeroVector;
				if (i != 0)
				{
					SwarmOffset		= FMath::VRand() * CollisionDiameter * 5;
					SwarmOffset.Z	= 0.f;
				}

				FVector CandidateLocation = SpawnLocation + SwarmOffset;

				bool bHasCollision = GetWorld()->OverlapAnyTestByObjectType(
					CandidateLocation,
					FQuat::Identity,
					ObjectParams,
					CollisionShape
				);

				if (!bHasCollision)
				{
					FinalLocation = CandidateLocation;
					BFoundValidSpot = true;
					break;
				}

				Attempts++;
			}

			if (BFoundValidSpot)
			{
				AEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyBP, FinalLocation + SpawnOffset, FRotator::ZeroRotator, SpawnParam);

				SpawnedEnemy->ApplyUpgrades(Stage);
			}
		}
	else 
		GetWorld()->SpawnActor<AEnemy>(EnemyBP, SpawnLocation + SpawnOffset, FRotator::ZeroRotator, SpawnParam);
}

void AEnemySpawner::SetupSpawnTimer()
{
	if (FMath::IsNearlyZero(SpawnRate, 1.f)) return;

	FTimerHandle	SpawnerHandle;

	GetWorld()->GetTimerManager().SetTimer
	(
		SpawnerHandle,
		this,
		&AEnemySpawner::SpawnEnemy,
		SpawnRate,
		true
	);
}