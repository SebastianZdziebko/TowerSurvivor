#include "EnemyMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

#include "TowerSurvivor/Pawns/Enemy.h"
#include "TowerSurvivor/Enums/EEnemyState.h"
#include "TowerSurvivor/Components/StatsComponent.h"

UEnemyMovementComponent::UEnemyMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnemyMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEnemyMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UEnemyMovementComponent::MoveTowardsTower(float DeltaTime, FVector TowerLocation, AEnemy* Owner)
{
	/* Move character to the new direction, based on speed stat from Pawn's Stats Component */
	const FVector CurrentLocation = Owner->GetActorLocation();

	FVector Direction	= TowerLocation - CurrentLocation;
	Direction.Z			= 0.f;
	Direction			= Direction.GetSafeNormal();

	FVector SeparationForce = FVector::ZeroVector;

	TArray<AActor*> OverlappingEnemies;
	Owner->GetOverlappingActors(OverlappingEnemies, AEnemy::StaticClass());

	for(AActor* OtherEnemy : OverlappingEnemies)
		if (OtherEnemy && OtherEnemy != Owner)
		{
			FVector PushDirection	= CurrentLocation - OtherEnemy->GetActorLocation();
			float	Distance		= PushDirection.Length();

			if (Distance > 0.1f) SeparationForce += (PushDirection.GetSafeNormal() / Distance);
		}

	float SeparationWeight = 50.f;
	SeparationForce *= SeparationWeight;

	FVector FinalDirection	= Direction + SeparationForce;
	FinalDirection.Z		= 0.f;
	FinalDirection			= FinalDirection.GetSafeNormal();

	FVector NewLocation = CurrentLocation + FinalDirection * MovementSpeed * DeltaTime;

	Owner->SetActorLocation(NewLocation, true);

	/* Rotate Pawn to the movement direction */
	FRotator TargetRotation		= FinalDirection.Rotation();
	FRotator CurrentRotation	= Owner->GetActorRotation();
	TargetRotation.Pitch	= 0.f;
	TargetRotation.Roll		= 0.f;

	float RotationSpeed = 10.f;
	FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);

	Owner->SetActorRotation(SmoothRotation);
}

void UEnemyMovementComponent::Sinking(float DeltaTime, float Speed, AEnemy* Owner)
{
	if (!Owner) return;

	const FVector	CurrentLocation = Owner->GetActorLocation();
	const float		FallSpeed = Speed * DeltaTime;
	const FVector	NewLocation = CurrentLocation - FVector{ 0.f,
															 0.f,
															 FallSpeed };

	CurrentSinkLevel += FallSpeed;
	Owner->SetActorLocation(NewLocation, true);

	if (CurrentSinkLevel >= MaxSinkLevel) OnReachSinkMax.Execute();
}