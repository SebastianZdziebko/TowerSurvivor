#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyMovementComponent.generated.h"

DECLARE_DELEGATE(FOnReachSinkMax);

class AEnemy;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TOWERSURVIVOR_API UEnemyMovementComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()			float	MaxSinkLevel		{ 200.f };
	UPROPERTY()			float	CurrentSinkLevel	{ 0.f };

public:
	UEnemyMovementComponent();

	FOnReachSinkMax	OnReachSinkMax;

	UPROPERTY()			float MovementSpeed	{ 0.f };

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void MoveTowardsTower	(float DeltaTime, FVector TowerLocation, AEnemy* Owner);
	void Sinking			(float DeltaTime, float Speed, AEnemy* Owner);
};