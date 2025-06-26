// Fill out your copyright notice in the Description page of Project Settings.


#include "ARCoin.h"

#include "ARPlayerState.h"


AARCoin::AARCoin()
{
	CreditsGain = 10;
}

void AARCoin::ApplyEffect_Implementation(APawn* Pawn)
{
	AARPlayerState* PS = Cast<AARPlayerState>(Pawn->GetPlayerState());

	if (!PS)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerState is invalid AARPlayerState class."));
		return;
	}

	PS->AddCredits(CreditsGain);
	
	Super::ApplyEffect_Implementation(Pawn);
}

