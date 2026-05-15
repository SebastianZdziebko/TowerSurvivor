#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IncomeWidget.generated.h"

UCLASS()
class THELASTTOWER_API UIncomeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int32 CurrentIncome;
};