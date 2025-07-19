// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPlayerController.h"

#include "Blueprint/UserWidget.h"

void AARPlayerController::TogglePauseMenu()
{
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;

		SetShowMouseCursor(false);
		SetInputMode(FInputModeGameOnly());
		return;
	}
	
	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if (PauseMenuInstance)
	{
		PauseMenuInstance->AddToViewport(100);

		SetShowMouseCursor(true);
		SetInputMode(FInputModeUIOnly());
	}
}

void AARPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &AARPlayerController::TogglePauseMenu);
}

void AARPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();

	BlueprintBeginPlayingState();
}
