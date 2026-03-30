#include "WeaponSystemComponent.h"

#include "Components/SphereComponent.h"

#include "TowerSurvivor/Pawns/Enemy.h"
#include "TowerSurvivor/Actors/Weapon.h"
#include "TowerSurvivor/Enums/ETargetType.h"

UWeaponSystemComponent::UWeaponSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	RangeSpheres.Add(ERange::Short, CreateDefaultSubobject<USphereComponent>(TEXT("Short Range")));
	RangeSpheres.Add(ERange::Medium, CreateDefaultSubobject<USphereComponent>(TEXT("Medium Range")));
	RangeSpheres.Add(ERange::Long, CreateDefaultSubobject<USphereComponent>(TEXT("Long Range")));

	for (auto& Sphere : RangeSpheres)
	{
		Sphere.Value -> SetupAttachment(this);
		Sphere.Value -> SetVisibility(false);
		Sphere.Value -> SetSphereRadius(RangeToFloat(Sphere.Key));
		Sphere.Value -> SetCollisionObjectType(RangeChannel);
		Sphere.Value -> SetCollisionResponseToAllChannels(ECR_Ignore);
		Sphere.Value -> SetCollisionResponseToChannel(EnemyChannel, ECR_Overlap);
		Sphere.Value -> OnComponentBeginOverlap.AddDynamic(this, &UWeaponSystemComponent::OnRangeSphereBeginOverlap);	
	}
}

void UWeaponSystemComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UWeaponSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ReadyWeapons.Num() == NULL) return;

	TArray<AWeapon*> TempReadyWeapons = ReadyWeapons;

	for (AWeapon* AvailableWeapon : TempReadyWeapons)
	{
		ERange	WeaponRange		= AvailableWeapon->GetWeaponRange();
		int32   NumberOfTargets = AvailableWeapon->GetNumberOfTargets();

		if (NumberOfTargets > 1)
		{
			TArray<AActor*> TargetedEnemies = GetRandomActorsInRange(WeaponRange, NumberOfTargets);

			if (!TargetedEnemies.IsEmpty())
				FireWeaponToMultipleTargets(TargetedEnemies, AvailableWeapon);
		}
		else
		{
			AActor* TargetedEnemy = GetRandomActorInRange(WeaponRange);

			if (IsValid(TargetedEnemy))
				FireWeaponToTarget(TargetedEnemy, AvailableWeapon);
		}
	}
}

AActor* UWeaponSystemComponent::GetRandomActorInRange(ERange Range)
{
	FEnemyArray* AvailabaleEnemies = EnemiesInRanges.Find(Range);

	if (!AvailabaleEnemies) return nullptr;

	int32 EnemiesNumber = AvailabaleEnemies->Enemies.Num();

	if (EnemiesNumber == 0) return nullptr;
	
	int32	EnemyIndex			= FMath::RandRange(0, EnemiesNumber - 1);
	AActor* RandomEnemyActor	= AvailabaleEnemies->Enemies[EnemyIndex];

	return RandomEnemyActor;
}

TArray<AActor*> UWeaponSystemComponent::GetRandomActorsInRange(ERange Range, int32 NumberOfTargets)
{
	FEnemyArray* AvailabaleEnemies = EnemiesInRanges.Find(Range);

	if (!AvailabaleEnemies) return {};

	TArray<int32>	IndexPool;
	TArray<AActor*> Targets;
	const int32 EnemiesNumber = AvailabaleEnemies->Enemies.Num();

	for (int i = 0; i < EnemiesNumber; i++)
		if (AvailabaleEnemies->Enemies[i])
			IndexPool.Add(i);

	for (int i = 0; i < NumberOfTargets; i++)
	{
		if (IndexPool.IsEmpty()) break;

		const int32 PoolSize = IndexPool.Num();
		int32 AvailableIndex = FMath::RandRange(0, PoolSize - 1);
		int32 EnemyIndex	 = IndexPool[AvailableIndex];

		AActor* RandomEnemyActor = AvailabaleEnemies->Enemies[EnemyIndex];

		IndexPool.RemoveAtSwap(AvailableIndex);
		Targets.Add(RandomEnemyActor);
	}

	return Targets;
}

void UWeaponSystemComponent::RemoveEnemyFromRanges(AEnemy* Enemy)
{
	if (!Enemy) return;

	for (auto& Sphere : RangeSpheres)
	{
		const ERange Range = Sphere.Key;

		if (FEnemyArray* FoundArray = EnemiesInRanges.Find(Range))
			FoundArray->Enemies.Remove(Enemy);
	}
}

void UWeaponSystemComponent::AddWeapon(TSubclassOf<AWeapon> WeaponToAdd)
{
	if (!WeaponToAdd) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>
		(
			WeaponToAdd,
			GetOwner()->GetActorLocation(),
			GetOwner()->GetActorRotation(),
			SpawnParams
		);

	if (!SpawnedWeapon) return;

	Weapons.Add(SpawnedWeapon);
	SetWeaponReadyToFire(SpawnedWeapon);
}

void UWeaponSystemComponent::SetupWeaponCooldown(AWeapon* WeaponToSetup)
{
	FTimerHandle	WeaponTimerHandle = WeaponToSetup->WeaponCooldownHandle;
	FTimerDelegate	WeaponTimerDelegate;

	WeaponTimerDelegate.BindUObject(this, &UWeaponSystemComponent::SetWeaponReadyToFire, WeaponToSetup);
	GetWorld()->GetTimerManager().SetTimer
	(
		WeaponTimerHandle, 
		WeaponTimerDelegate, 
		WeaponToSetup->GetWeaponCooldown(), 
		false
	);
}

void UWeaponSystemComponent::SetWeaponReadyToFire(AWeapon* WeaponToSetReady)
{
	WeaponToSetReady->SetReady();
	ReadyWeapons.Add(WeaponToSetReady);
	
	FTimerHandle HandleToStop = WeaponToSetReady->WeaponCooldownHandle;

	GetWorld()->GetTimerManager().ClearTimer(HandleToStop);
}

void UWeaponSystemComponent::FireWeaponToTarget(AActor* Target, AWeapon* WeaponToFire)
{
	if (!IsValid(Target) || !IsValid(WeaponToFire)) return;

	WeaponToFire->FireWeapon(Target);
	ReadyWeapons.Remove(WeaponToFire);
	SetupWeaponCooldown(WeaponToFire);
}

void UWeaponSystemComponent::FireWeaponToMultipleTargets(TArray<AActor*> Targets, AWeapon* WeaponToFire)
{
	if (Targets.IsEmpty() || !IsValid(WeaponToFire)) return;

	const int32 NumberOfTargets = Targets.Num();

	for (AActor* Target : Targets)
		WeaponToFire->FireWeapon(Target);


	ReadyWeapons.Remove(WeaponToFire);
	SetupWeaponCooldown(WeaponToFire);
}

void UWeaponSystemComponent::OnRangeSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
													   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
													   bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->Implements<UCombatInterface>()) return;

	if(USphereComponent* Sphere = Cast<USphereComponent>(OverlappedComponent))
		if (const ERange* Found = RangeSpheres.FindKey(Sphere))
		{
			const ERange Range = *Found;

			AEnemy* Enemy = Cast<AEnemy>(OtherActor);

			EnemiesInRanges.FindOrAdd(Range).Enemies.Add(Enemy);

			Enemy->OnEnemyDeath.AddUniqueDynamic(this, &UWeaponSystemComponent::RemoveEnemyFromRanges);
		}
}