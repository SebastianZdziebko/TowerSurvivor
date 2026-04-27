#include "Weapon.h"

#include "Kismet/GameplayStatics.h"

#include "TowerSurvivor/Pawns/Enemy.h"
#include "TowerSurvivor/Actors/Projectile.h"
#include "TowerSurvivor/Data/WeaponData.h"

//#include "TowerSurvivor/Enums/EWeaponType.h"
#include "TowerSurvivor/Enums/ERange.h"

AWeapon::AWeapon()					
{ 
	PrimaryActorTick.bCanEverTick			= true;
	PrimaryActorTick.bStartWithTickEnabled	= true;
}

void AWeapon::BeginPlay()			
{ 
	Super::BeginPlay(); 
}

void AWeapon::Tick(float DeltaTime)	
{ 
	Super::Tick(DeltaTime); 
}

void AWeapon::FireWeapon(AActor* TargetEnemy, float DamageModifier)
{
	if (!IsValid(TargetEnemy)) return;

	SpawnTransform = GetActorTransform();
	SpawnTransform.AddToTranslation(FVector(0.f, 0.f, 50.f));

	AProjectile* SpawnedProjectile = GetWorld()->SpawnActorDeferred<AProjectile>
	(
		WeaponData->Projectile,
		SpawnTransform, 
		this,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	if (!SpawnedProjectile) return;

	SpawnedProjectile->WeaponData	= WeaponData;
	SpawnedProjectile->Target		= Cast<AEnemy>(TargetEnemy);
	SpawnedProjectile->Damage		= ApplyModifier(WeaponData->Damage, DamageModifier);

	UGameplayStatics::FinishSpawningActor(SpawnedProjectile, SpawnTransform);
}

int32 AWeapon::ApplyModifier(int32 BaseValue, float Modifier)
{
	if (Modifier <= 0.f) return BaseValue;

	float DamageValue		= static_cast<float>(BaseValue);
	float ModifiedDamage	= DamageValue + DamageValue * Modifier;
	int32 FinalDamage		= static_cast<int32>(ModifiedDamage);

	return FinalDamage;
}

void		AWeapon::SetReady()				{ bIsReady = true; }
float		AWeapon::GetWeaponCooldown()	{ return WeaponData->Cooldown; }
int32		AWeapon::GetNumberOfTargets()	{ return WeaponData->NumberOfTargets; }
ERange		AWeapon::GetWeaponRange()		{ return WeaponData->Range; }
EWeaponType AWeapon::GetWeaponType()		{ return WeaponData->Type; }