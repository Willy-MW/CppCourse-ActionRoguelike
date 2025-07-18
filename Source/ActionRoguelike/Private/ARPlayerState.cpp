// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPlayerState.h"

#include "ARSaveGame.h"
#include "Net/UnrealNetwork.h"

int32 AARPlayerState::GetCredits() const
{
	return Credits;
}

void AARPlayerState::AddCredits(int32 Delta)
{
	if (Delta <= 0)
	{
		return;
	}

	Credits += Delta;
	OnRep_Credits(Credits-Delta);
}

void AARPlayerState::RemoveCredits(int32 Delta)
{
	if (Delta <= 0)
	{
		return;
	}

	Credits -= Delta;
	OnRep_Credits(Credits+Delta);
}

void AARPlayerState::OnRep_Credits(int32 OldCredits)
{
	OnCreditsChanged.Broadcast(this, GetCredits(), Credits - OldCredits);
}

void AARPlayerState::LoadPlayerState_Implementation(UARSaveGame* SaveObject)
{
	if (SaveObject)
	{
		AddCredits(SaveObject->Credits);
	}
}

void AARPlayerState::SavePlayerState_Implementation(UARSaveGame* SaveObject)
{
	if (SaveObject)
	{
		Credits = SaveObject->Credits;
	}
}

void AARPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARPlayerState, Credits);
}
