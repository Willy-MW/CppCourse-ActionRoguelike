// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPickUp_Action.h"

#include "ARAction.h"
#include "ARActionComponent.h"


void AARPickUp_Action::ApplyEffect_Implementation(APawn* Pawn)
{
	if (!ensure(GrantedAction))
	{
		return;
	}
	
	UARActionComponent* ActionComp = Pawn->FindComponentByClass<UARActionComponent>();
	if (!ActionComp)
	{
		return;
	}

	if (ActionComp->HasAction(GrantedAction))
	{
		UE_LOG(LogTemp, Display, TEXT("Action already exists!"));
		return;
	}

	ActionComp->AddAction(this, GrantedAction);
	
	Super::ApplyEffect_Implementation(Pawn);
}
