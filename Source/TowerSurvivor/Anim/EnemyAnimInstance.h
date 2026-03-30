#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "TowerSurvivor//Enums/EEnemyState.h"

#include "EnemyAnimInstance.generated.h"

class AEnemy;

UCLASS()
class TOWERSURVIVOR_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	private:
		UFUNCTION() void UpdateAnimState(EEnemyState CurrentState);

	protected:
		UPROPERTY(BlueprintReadOnly, Category = "General")	AEnemy*		EnemyOwner;
		UPROPERTY(BlueprintReadOnly, Category = "General")	EEnemyState	EnemyState;
		UPROPERTY(BlueprintReadOnly, Category = "Movement")	float		Speed					{ 0.f };
		UPROPERTY(BlueprintReadOnly, Category = "Combat")	class UAnimMontage* AttackMontage	{ nullptr };

	public:
		virtual void NativeUpdateAnimation(float DeltaTime) override;
		virtual void NativeInitializeAnimation()			override;
};
