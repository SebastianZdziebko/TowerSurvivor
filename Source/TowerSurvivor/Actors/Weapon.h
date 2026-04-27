#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TowerSurvivor/Interfaces/CombatInterface.h"

#include "Weapon.generated.h"

class UWeaponData;

enum class ERange : uint8;
enum class ETargetType : uint8;
enum class EWeaponType : uint8;

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

	void	FireWeapon(AActor* Target, float DamageModifier);
	int32	ApplyModifier(int32 BaseValue, float Modifier);

	void		SetReady();
	float		GetWeaponCooldown();
	int32		GetNumberOfTargets();
	ERange		GetWeaponRange();
	EWeaponType GetWeaponType();
};