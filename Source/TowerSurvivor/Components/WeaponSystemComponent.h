#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "TowerSurvivor/Interfaces/CombatInterface.h"

#include "WeaponSystemComponent.generated.h"

class AWeapon;
class AEnemy;
class USphereComponent;

enum class EWeaponType : uint8;
enum class ERange : uint8;

USTRUCT(BlueprintType)
struct FEnemyArray
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AEnemy*> Enemies;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOWERSURVIVOR_API UWeaponSystemComponent : public USceneComponent
{
	GENERATED_BODY()

	TEnumAsByte<ECollisionChannel> RangeChannel{ ECC_GameTraceChannel1 };
	TEnumAsByte<ECollisionChannel> EnemyChannel{ ECC_GameTraceChannel2 };

public:	
	UWeaponSystemComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon System") TMap<ERange, USphereComponent*>	RangeSpheres;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon System") TArray<AWeapon*>					ReadyWeapons;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon System") TArray<AWeapon*>					Weapons;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon System") TMap<ERange, FEnemyArray>		EnemiesInRanges;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon System") TMap<EWeaponType, float>			Modifiers;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()	AActor*			GetRandomActorInRange	(ERange Range);
	UFUNCTION() TArray<AActor*> GetRandomActorsInRange	(ERange Range, int32 NumberOfTargets);
	UFUNCTION()	void			RemoveEnemyFromRanges	(AEnemy* Enemy);

	UFUNCTION(BlueprintCallable) void AddWeapon(TSubclassOf<AWeapon> WeaponToAdd);
	UFUNCTION(BlueprintCallable) void IncreaseModifier(EWeaponType ModifierToIncrease, float Modifier);

	UFUNCTION()	void SetupWeaponCooldown	(AWeapon* WeaponToSetup);
	UFUNCTION()	void SetWeaponReadyToFire	(AWeapon* WeaponToSetReady);
	UFUNCTION()	void FireWeaponToTarget		(AActor* Target, AWeapon* WeaponToFire);
	UFUNCTION()	void FireWeaponToMultipleTargets(TArray<AActor*> Targets, AWeapon* WeaponToFire);

	UFUNCTION()	void OnRangeSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
											   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
											   bool bFromSweep, const FHitResult& SweepResult);
};