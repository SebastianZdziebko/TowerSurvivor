#include "Weapon.h"

#include "Kismet/GameplayStatics.h"

#include "TowerSurvivor/Pawns/Enemy.h"
#include "TowerSurvivor/Actors/Projectile.h"
#include "TowerSurvivor/Data/WeaponData.h"
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

void AWeapon::FireWeapon(AActor* TargetEnemy)
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

	UGameplayStatics::FinishSpawningActor(SpawnedProjectile, SpawnTransform);
}

void		AWeapon::SetReady()				{ bIsReady = true; }
float		AWeapon::GetWeaponCooldown()	{ return WeaponData->Cooldown; }
int32		AWeapon::GetNumberOfTargets()	{ return WeaponData->NumberOfTargets; }
ERange		AWeapon::GetWeaponRange()		{ return WeaponData->Range; }