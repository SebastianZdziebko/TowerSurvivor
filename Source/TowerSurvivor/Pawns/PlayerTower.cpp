#include "PlayerTower.h"

#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "TowerSurvivor/Components/UpgradeSystemComponent.h"
#include "TowerSurvivor/Components/WeaponSystemComponent.h"
#include "TowerSurvivor/Components/StatsComponent.h"
#include "TowerSurvivor/Components/WalletComponent.h"

APlayerTower::APlayerTower()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent	 =  CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	MeshComp		 =  CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp		 -> SetupAttachment(RootComponent);

	CollisionComp	 =  CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionComp	 -> SetupAttachment(RootComponent);
	CollisionComp	 -> SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp	 -> SetCollisionObjectType(TowerChannel);
	CollisionComp	 -> SetCollisionResponseToChannel(EnemyChannel, ECR_Overlap);
	CollisionComp	 -> SetCollisionResponseToChannel(ProjectileChannel, ECR_Overlap);


	StatsComp		 =  CreateDefaultSubobject<UStatsComponent>(TEXT("Stats"));

	WeaponSystemComp =  CreateDefaultSubobject<UWeaponSystemComponent>(TEXT("Weapon System"));
	WeaponSystemComp -> SetupAttachment(RootComponent);

	UpgradeSystemComp = CreateDefaultSubobject<UUpgradeSystemComponent>(TEXT("Upgrade System"));

	SpringArm		 =  CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm		 -> SetupAttachment(RootComponent);
	SpringArm		 -> bUsePawnControlRotation = false;
	SpringArm		 -> TargetArmLength = 600.f;
	SpringArm		 -> SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));

	CameraComp		 =  CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp		 -> SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	CameraComp		 -> bUsePawnControlRotation = false;

	WalletComp		 = CreateDefaultSubobject<UWalletComponent>(TEXT("Wallet"));
}

void APlayerTower::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerTower::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerTower::TakeDamage_Implementation(int32 Amount)
{
	StatsComp->DecreaseStat(EStat::Health, Amount);
}