#include "Projectile.h"

#include "Engine/OverlapResult.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "TowerSurvivor/Pawns/Enemy.h"
#include "TowerSurvivor/Interfaces/CombatInterface.h"
#include "TowerSurvivor/Data/WeaponData.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	CollisionComp =  CreateDefaultSubobject<USphereComponent>(TEXT("Collision Component"));
	CollisionComp -> SetupAttachment(RootComponent);
	CollisionComp -> SetCollisionObjectType(ProjectileChannel);
	CollisionComp -> SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionComp -> SetGenerateOverlapEvents(true);
	CollisionComp -> SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp -> SetCollisionResponseToChannel(EnemyChannel, ECR_Overlap);

	NiagaraComp	  =  CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Component"));
	NiagaraComp	  -> SetupAttachment(RootComponent);
	NiagaraComp	  -> SetPaused(true);

	CollisionComp -> OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OverlapTarget);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if(WeaponData)
		RemainingBouces  = WeaponData->NumberOfBouces;

	if (AEnemy* TargetEnemy = Cast<AEnemy>(Target))
		TargetEnemy->OnEnemyDeath.AddDynamic(this, &AProjectile::RemoveTarget);

	if (!Target) return; 
	
	TargetPosition = Target->GetActorLocation();
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::OverlapTarget(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
								UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
								bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || !IsValid(OtherComp)) return;

	if (!OtherActor->Implements<UCombatInterface>()) return;

	if (OtherActor == Target && !HittedEnemies.Contains(OtherActor))
		OnProjectileHit(OtherActor);
}

void AProjectile::FindEnemyInRadius(UObject* WorldContextObject, FVector Center, float Radius)
{
	if (!WorldContextObject) return;

	UWorld* WorldContext = WorldContextObject->GetWorld();

	if (!WorldContext) return;

	AEnemy* PotentialTarget = nullptr;
	TArray<FOverlapResult>	OverlapResults;

	FCollisionShape			BounceCollision;
	BounceCollision.SetSphere(Radius);

	FCollisionQueryParams		QueryParams;
	QueryParams.bTraceComplex = false;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(EnemyChannel);

	const bool bHit = WorldContext->OverlapMultiByObjectType(
		OverlapResults,
		Center,
		FQuat::Identity,
		ObjectQueryParams,
		BounceCollision,
		QueryParams
	);

	if (!bHit)
	{
		OnProjectileDestroy(Center);
		return;
	}

	float MinDistance = FLT_MAX;

	for (const FOverlapResult& Result : OverlapResults)
	{
		AEnemy* OverlappedActor = Cast<AEnemy>(Result.GetActor());
		bool bHittedByProjectile = HittedEnemies.Contains(OverlappedActor);

		if (!bHittedByProjectile)
		{
			const FVector	OverlappedActorLocation = OverlappedActor->GetActorLocation();
			const float		Distance = FVector::DistSquared(Center, OverlappedActorLocation);

			if (Distance > MinDistance) continue;

			MinDistance = Distance;
			PotentialTarget = OverlappedActor;
		}
	}

	if (!PotentialTarget) OnProjectileDestroy(Center);
	else Target = PotentialTarget;
}

void AProjectile::RemoveTarget(AEnemy* DestroyedTarget)
{
	Target	= nullptr;
	NiagaraComp->Deactivate();
}

void AProjectile::OnProjectileHit_Implementation(AActor* OtherActor)
{
	if (OtherActor->IsA<AEnemy>())
		HittedEnemies.Add(OtherActor);

	ICombatInterface::Execute_TakeDamage(OtherActor, Damage);
}

void AProjectile::MoveTowardsTarget(float DeltaTime)
{
	const FVector CurrentPosition	= GetActorLocation();

	FVector AimPosition	 = TargetPosition;
	AimPosition.Z		 = CurrentPosition.Z;

	const FVector Direction	= (AimPosition - CurrentPosition).GetSafeNormal();

	float	Speed			= WeaponData->Speed;
	FVector NewPosition		= CurrentPosition + Direction * Speed * DeltaTime;

	if (bUseArc)
	{
		const float TotalDist		= FVector::Dist(OriginalPosition, TargetPosition);
		const float CurrentDist		= FVector::Dist(OriginalPosition, NewPosition);
		const float Alpha			= FMath::Clamp(CurrentDist / TotalDist, 0.f, 1.f);

		const float LerpZ = FMath::Lerp(OriginalPosition.Z, TargetPosition.Z, Alpha);

		const float ArcZ = ArcOffset * 4.0f * Alpha * (1.0f - Alpha);

		NewPosition.Z = LerpZ + ArcZ;
	}

	SetActorLocation(NewPosition);

	FVector Velocity = (NewPosition - CurrentPosition).GetSafeNormal();

	SetActorRotation(Velocity.Rotation());
}