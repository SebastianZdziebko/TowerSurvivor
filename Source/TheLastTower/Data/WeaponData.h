#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "TheLastTower/Enums/EWeaponType.h"
#include "TheLastTower/Enums/ETargetType.h"
#include "TheLastTower/Enums/ERange.h"
#include "TheLastTower/Actors/Projectile.h"
#include "TheLastTower/Actors/Weapon.h"

#include "WeaponData.generated.h"

UCLASS(BlueprintType)
class THELASTTOWER_API UWeaponData : public UDataAsset
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")        int32 Damage         { 0 };
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")        float Cooldown       { 0.f };
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")        float Speed          { 0.f };
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")        ERange      Range    { ERange::None };
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")        EWeaponType Type     { EWeaponType::None };
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")        ETargetType Target   { ETargetType::None };
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")        bool        bSplash  { false };
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pointers")       TSubclassOf<AProjectile> Projectile { nullptr };
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pointers")       TObjectPtr<UTexture2D>   Icon       { nullptr };
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (EditCondition = "Target == ETargetType::Multi", EditConditionHides))
    int32 NumberOfTargets   { 1 };

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (EditCondition = "Target == ETargetType::Bounce", EditConditionHides))
    int32 NumberOfBouces    { 0 };

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (EditCondition = "Target == ETargetType::Bounce", EditConditionHides))
    float BounceRadius      { 0.f };

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (EditCondition = "bSplash", EditConditionHides))
    float SplashRadius{ 0.f };
};
