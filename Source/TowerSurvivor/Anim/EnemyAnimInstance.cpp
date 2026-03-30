#include "TowerSurvivor/Anim/EnemyAnimInstance.h"
#include "TowerSurvivor/Components/StatsComponent.h"
#include "TowerSurvivor/Pawns/Enemy.h"

void UEnemyAnimInstance::UpdateAnimState(EEnemyState CurrentState) { EnemyState = CurrentState; }

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if (!EnemyOwner) 
	{
		EnemyOwner = Cast<AEnemy>(TryGetPawnOwner());
		return;
	}

	Speed = EnemyOwner->EnemyStatsComp->Stats[EStat::Speed];
}

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	APawn* PawnOwner = TryGetPawnOwner();

	if (!PawnOwner) return;

	EnemyOwner = Cast<AEnemy>(PawnOwner);

	if(IsValid(EnemyOwner)) EnemyOwner->OnEnemyStateChange.AddDynamic(this, &UEnemyAnimInstance::UpdateAnimState);
}