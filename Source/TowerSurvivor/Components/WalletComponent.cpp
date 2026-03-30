#include "WalletComponent.h"

#include "TowerSurvivor/Widgets/IncomeWidget.h"

UWalletComponent::UWalletComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UWalletComponent::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer
	(
		IncomeHandle,
		this,
		&UWalletComponent::ApplyIncome,
		5.f,
		true
	);

	PlayerController = GetWorld()->GetFirstPlayerController();
	Owner			 = GetOwner();
}

void UWalletComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UWalletComponent::AddToWallet(int32 Amount)
{
	if (Amount <= 0) return;
	else Wallet += Amount;

	OnWalletChangeDelegate.Broadcast(Wallet);
}

void UWalletComponent::RemoveFromWallet(int32 Amount)
{
	if (Amount <= 0) return;
	else Wallet -= Amount;

	OnWalletChangeDelegate.Broadcast(Wallet);
}

void UWalletComponent::IncreaseIncome(int32 Amount)
{
	if (Amount <= 0) return;
	else CurrentIncome += Amount;
}

void UWalletComponent::IncreaseIncomeModifier(float Modifier)
{
	if (Modifier <= 0.f) return;
	else CurrentModifier += Modifier;
}

bool UWalletComponent::CheckWallet(int32 Cost)
{
	if (Wallet < Cost) return false;
	else return true;
}

int32 UWalletComponent::ApplyModifier()
{
	float BaseValue		= static_cast<float>(CurrentIncome);
	float ModifiedValue = BaseValue + BaseValue * CurrentModifier;
	int32 OutValue		= FMath::RoundToInt32(ModifiedValue);

	return OutValue;
}

void UWalletComponent::ApplyIncome()
{
	if (!IncomeWidget) return;

	UIncomeWidget* NewWidget = CreateWidget<UIncomeWidget>(GetWorld(), IncomeWidget);
	
	if (!NewWidget) return;

	CreatedWidgets.Add(NewWidget);

	if (CurrentModifier > 0.f)
	{
		int32 OutIncome = ApplyModifier();
		NewWidget->CurrentIncome = OutIncome;
	}
	else NewWidget->CurrentIncome = CurrentIncome;

	NewWidget->AddToViewport();

	if (!Owner || !PlayerController) return;

	FVector		OwnerLocation = Owner->GetActorLocation();
	FVector2D	ScreenPosition;

	if (PlayerController->ProjectWorldLocationToScreen(OwnerLocation, ScreenPosition))
	{
		NewWidget->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
		NewWidget->SetPositionInViewport(ScreenPosition, true);
	}

	AddToWallet(CurrentIncome);
}