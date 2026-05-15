#include "PlayerTower.h"

#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "TheLastTower/TheLastTower.h"
#include "TheLastTower/Components/UpgradeSystemComponent.h"
#include "TheLastTower/Components/WeaponSystemComponent.h"
#include "TheLastTower/Components/StatsComponent.h"
#include "TheLastTower/Components/WalletComponent.h"

APlayerTower::APlayerTower()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent	 =  CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// Creating mesh component
	MeshComp		 =  CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp		 -> SetupAttachment(RootComponent);

	// Creating collision component which is ignoring all trace channels
	// Except COLLISION_ENEMY and COLLISION_PROJECTILE, thes channels are overlapped
	CollisionComp	 =  CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionComp	 -> SetupAttachment(RootComponent);
	CollisionComp	 -> SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp	 -> SetCollisionObjectType(COLLISION_TOWER);
	CollisionComp	 -> SetCollisionResponseToChannel(COLLISION_ENEMY, ECR_Overlap);
	CollisionComp	 -> SetCollisionResponseToChannel(COLLISION_PROJECTILE, ECR_Overlap);

	// Creating stats component
	StatsComp		 =  CreateDefaultSubobject<UStatsComponent>(TEXT("Stats"));

	// Creating weapon system component
	WeaponSystemComp =  CreateDefaultSubobject<UWeaponSystemComponent>(TEXT("Weapon System"));
	WeaponSystemComp -> SetupAttachment(RootComponent);

	// Creating upgrade system component
	UpgradeSystemComp = CreateDefaultSubobject<UUpgradeSystemComponent>(TEXT("Upgrade System"));

	// Creating spring arm component used for camera positioning
	SpringArm		 =  CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm		 -> SetupAttachment(RootComponent);
	SpringArm		 -> bUsePawnControlRotation = false;
	SpringArm		 -> TargetArmLength = 600.f;
	SpringArm		 -> SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));

	// Creating camera component and attaching it to the spring arm component
	CameraComp		 =  CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp		 -> SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	CameraComp		 -> bUsePawnControlRotation = false;

	// Creating wallet component
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