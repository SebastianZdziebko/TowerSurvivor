#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TowerSurvivor/Pawns/Enemy.h"

#include "EnemySpawner.generated.h"

class AEnemy;

UCLASS()
class TOWERSURVIVOR_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

	TEnumAsByte<ECollisionChannel> EnemyChannel{ ECC_GameTraceChannel2 };
	
public:	
	AEnemySpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")	TArray<TSubclassOf<AEnemy>> OptionsToSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")	float	SpawnRadius		{ 0.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")	float	SpawnRate		{ 0.f };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner") int32	Stage			{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swarm")	int32 SwarmSize{ 1 };

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()		FVector				GetRandomSpawnLocation();
	UFUNCTION()		TSubclassOf<AEnemy> GetRandomEnemyToSpawn();

	UFUNCTION(BlueprintCallable)	void	SpawnEnemy();
	UFUNCTION(BlueprintCallable)	void	SetupSpawnTimer();
};
