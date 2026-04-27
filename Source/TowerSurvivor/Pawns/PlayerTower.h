#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "TowerSurvivor/Interfaces/CombatInterface.h"

#include "PlayerTower.generated.h"

UCLASS()
class TOWERSURVIVOR_API APlayerTower : public APawn, public ICombatInterface
{
	GENERATED_BODY()

	TEnumAsByte<ECollisionChannel> EnemyChannel{ ECC_GameTraceChannel2 };
	TEnumAsByte<ECollisionChannel> TowerChannel{ ECC_GameTraceChannel3 };
	TEnumAsByte<ECollisionChannel> ProjectileChannel{ ECC_GameTraceChannel4 };

public:
	APlayerTower();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	UStaticMeshComponent* MeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	class USphereComponent* CollisionComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	class UStatsComponent* StatsComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	class UWalletComponent* WalletComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	class UWeaponSystemComponent* WeaponSystemComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	class UUpgradeSystemComponent* UpgradeSystemComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)		AActor*				Shop;
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void TakeDamage_Implementation(int32 Amount) override;
};