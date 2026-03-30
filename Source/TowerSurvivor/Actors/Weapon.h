#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TowerSurvivor/Interfaces/CombatInterface.h"
#include "TowerSurvivor/Enums/ERange.h"
#include "TowerSurvivor/Enums/ETargetType.h"

#include "Weapon.generated.h"

class UWeaponData;

UCLASS()
class TOWERSURVIVOR_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)	bool bIsReady{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)	UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	UWeaponData*		  WeaponData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon") FTransform SpawnTransform { FTransform::Identity };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon") FTimerHandle WeaponCooldownHandle;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void FireWeapon(AActor* Target);

	void		SetReady();
	float		GetWeaponCooldown();
	int32		GetNumberOfTargets();
	ERange		GetWeaponRange();
};