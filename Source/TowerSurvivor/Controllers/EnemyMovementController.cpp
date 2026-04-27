#include "EnemyMovementController.h"

#include "Kismet/GameplayStatics.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "AITypes.h"

AEnemyMovementController::AEnemyMovementController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent());

	if (CrowdComp)
	{
		CrowdComp->SetCrowdSeparation(true);
		CrowdComp->SetCrowdSeparationWeight(10.f);
		CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);
	}
}

void AEnemyMovementController::CommandMoveToTarget(FVector Destination)
{
	MoveToLocation
	(
		Destination,
		100.f,
		true,
		true,
		true,
		true,
		0,
		true
	);
}

void AEnemyMovementController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	APawn* PlayerTower = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (PlayerTower)
		CommandMoveToTarget(PlayerTower->GetActorLocation());
}
