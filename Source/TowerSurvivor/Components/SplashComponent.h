#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SplashComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOWERSURVIVOR_API USplashComponent : public UActorComponent
{
	GENERATED_BODY()

	TEnumAsByte<ECollisionChannel> EnemyChannel{ ECC_GameTraceChannel2 };

public:	
	USplashComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool SearchInSplashRadius(TArray<AActor*>& OutActors, AActor* HittedEnemy, float SplashRadius);

	UFUNCTION(BlueprintCallable)
	void DealDamageInSplash(UPARAM(ref) TArray<AActor*>& HittedActors, int32 Damage);
};