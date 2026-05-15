#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "TheLastTower/Interfaces/CombatInterface.h"

#include "PlayerTower.generated.h"

class USphereComponent;
class UStatsComponent;
class UWalletComponent;
class UWeaponSystemComponent;
class UUpgradeSystemComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class THELASTTOWER_API APlayerTower : public APawn, public ICombatInterface
{
	GENERATED_BODY()

public:
	APlayerTower();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	UStaticMeshComponent*		MeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	USphereComponent*			CollisionComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	UStatsComponent*			StatsComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	UWalletComponent*			WalletComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	UWeaponSystemComponent*		WeaponSystemComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	UUpgradeSystemComponent*	UpgradeSystemComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	USpringArmComponent*		SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	UCameraComponent*			CameraComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)		AActor*						Shop;
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Implementation of functions from CombatInterface
	virtual void TakeDamage_Implementation(int32 Amount) override;
};