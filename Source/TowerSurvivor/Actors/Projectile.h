#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TOWERSURVIVOR_API AProjectile : public AActor
{
	GENERATED_BODY()

	TEnumAsByte<ECollisionChannel> EnemyChannel		{ ECC_GameTraceChannel2 };
	TEnumAsByte<ECollisionChannel> ProjectileChannel{ ECC_GameTraceChannel4 };
	
public:	
	AProjectile();

	UPROPERTY(BlueprintReadWrite) FVector TargetPosition = { FVector::ZeroVector };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	class AActor*				Target;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	class USphereComponent*		CollisionComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	class UNiagaraComponent*	NiagaraComp;
	UPROPERTY(EditAnywhere,	BlueprintReadOnly)	class UWeaponData*			WeaponData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")	bool	bUseArc		{ false };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")	int32	Damage		{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (EditCondition = "bUseArc == true", EditConditionHides))	float ArcOffset				{ 0.f };
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (EditCondition = "bUseArc == true", EditConditionHides)) FVector OriginalPosition	{ FVector::ZeroVector};
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (EditCondition = "bUseArc == true", EditConditionHides)) float OriginalDistance		{ 0.f };


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Bounce") int32 RemainingBouces { 0 };
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Bounce") TArray<AActor*> HittedEnemies;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OverlapTarget(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	void FindEnemyInRadius(UObject* WorldContextObject, FVector Center, float Radius);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable) void OnProjectileDestroy(FVector SpawnLocation);
	UFUNCTION(BlueprintNativeEvent)			void OnProjectileHit(AActor* OtherActor);
	UFUNCTION(BlueprintCallable)			void MoveTowardsTarget(float DeltaTime);
	UFUNCTION()								void RemoveTarget(AEnemy* DestroyedTarget);
	
	virtual void OnProjectileHit_Implementation(AActor* OtherActor);
};