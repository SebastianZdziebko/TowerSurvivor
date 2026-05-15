#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "TheLastTower/Interfaces/CombatInterface.h"

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
class THELASTTOWER_API UWeaponSystemComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UWeaponSystemComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon System") TMap<ERange, USphereComponent*>	RangeSpheres;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon System") TArray<AWeapon*>					ReadyWeapons;					// Holds only ready to fire weapons
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon System") TArray<AWeapon*>					Weapons;						// Holds every weapon bought by the player
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon System") TMap<ERange, FEnemyArray>		EnemiesInRanges;				// Holds enemy pointers in different arrays base on its current range from tower
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon System") TMap<EWeaponType, float>			Modifiers;						// Holds modifier to weapons damage base on its type

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