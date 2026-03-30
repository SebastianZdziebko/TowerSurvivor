#include "SplashComponent.h"

#include "Engine/OverlapResult.h"
#include "Components/PrimitiveComponent.h"

#include "TowerSurvivor/Interfaces/CombatInterface.h"

USplashComponent::USplashComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USplashComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USplashComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool USplashComponent::SearchInSplashRadius(TArray<AActor*>& OutActors, AActor* HittedEnemy, float SplashRadius)
{
	OutActors.Empty();

	if (!IsValid(HittedEnemy)) return false;

	OutActors.AddUnique(HittedEnemy);

	if (SplashRadius <= 0.f) return true;

	FVector TargetLocation = HittedEnemy->GetActorLocation();

	TArray<FOverlapResult> OverlapResults;
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	QueryParams.AddIgnoredActor(HittedEnemy);

	const bool bHit = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		TargetLocation,
		FQuat::Identity,
		FCollisionObjectQueryParams(EnemyChannel),
		FCollisionShape::MakeSphere(SplashRadius),
		QueryParams
	);

	if (bHit)
		for (const FOverlapResult& Result : OverlapResults)
			OutActors.AddUnique(Result.GetActor());

	return true;
}

void USplashComponent::DealDamageInSplash(UPARAM(ref) TArray<AActor*>& HittedActors, int32 Damage)
{
	if (HittedActors.Num() == 0 || Damage <= 0.f) return;

	for (int32 index = HittedActors.Num() - 1; index >= 0; --index)
	{
		AActor* Enemy = HittedActors[index];

		if (!IsValid(Enemy))
		{
			HittedActors.RemoveAtSwap(index);
			continue;
		}

		ICombatInterface::Execute_TakeDamage(Enemy, Damage);

		HittedActors.RemoveAtSwap(index);
	}
}