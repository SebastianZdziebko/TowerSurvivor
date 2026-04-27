#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Upgrade.generated.h"

class APlayerTower;
class UUpgradeData;

UCLASS(Blueprintable, BlueprintType)
class TOWERSURVIVOR_API UUpgrade : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Upgrade")	UUpgradeData* UpgradeData;
	UPROPERTY(BlueprintReadOnly, Category = "Upgrade")					APlayerTower* PlayerRef;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Upgrade") void ApplyUpgradeTick();
};
