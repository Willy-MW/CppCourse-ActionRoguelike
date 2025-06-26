// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPickUp.h"
#include "ARCoin.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARCoin : public AARPickUp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AARCoin();

	UPROPERTY(EditAnywhere);
	int32 CreditsGain;

protected:
	void ApplyEffect_Implementation(APawn* Pawn) override;
};
