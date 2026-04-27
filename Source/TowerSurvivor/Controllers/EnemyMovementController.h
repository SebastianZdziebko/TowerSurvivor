#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyMovementController.generated.h"

UCLASS()
class TOWERSURVIVOR_API AEnemyMovementController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyMovementController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UFUNCTION(BlueprintCallable) void CommandMoveToTarget(FVector Destination);

protected:
	virtual void OnPossess(APawn* InPawn) override;
};
