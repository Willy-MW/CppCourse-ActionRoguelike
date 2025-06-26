// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPlayerState.h"

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

	OnCreditsChanged.Broadcast(this, GetCredits(), Delta);
}

void AARPlayerState::RemoveCredits(int32 Delta)
{
	if (Delta <= 0)
	{
		return;
	}

	Credits -= Delta;

	OnCreditsChanged.Broadcast(this, GetCredits(), Delta);
}
