#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WalletComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWalletChangeSignature, int32, CurrentWallet);

enum class EStat : uint8;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOWERSURVIVOR_API UWalletComponent : public UActorComponent
{
	GENERATED_BODY()

	FTimerHandle IncomeHandle;

public:	
	UWalletComponent();

	UPROPERTY(BlueprintAssignable) FOnWalletChangeSignature OnWalletChangeDelegate;
	UPROPERTY(EditAnywhere) TSubclassOf<UUserWidget> IncomeWidget;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)		int32 Wallet			{ 0 };
	UPROPERTY(EditAnywhere,BlueprintReadOnly)		int32 CurrentIncome		{ 0 };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	float CurrentModifier	{ 0.f };

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable) void	AddToWallet				(int32 Amount);
	UFUNCTION(BlueprintCallable) void	RemoveFromWallet		(int32 Amount);
	UFUNCTION(BlueprintCallable) void	IncreaseIncome			(int32 Amount);
	UFUNCTION(BlueprintCallable) void	IncreaseIncomeModifier	(float Modifier);
	UFUNCTION(BlueprintCallable) bool	CheckWallet				(int32 Cost);
	UFUNCTION(BlueprintCallable) int32	ApplyModifier			();
	UFUNCTION()	void ApplyIncome();

	void ManageIncomeUpgrade(float Modifier, int32 FlatValue);
	void ManageSingleUse(EStat Stat, int32 Cost, int32 Reward);
};