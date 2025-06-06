// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPickUp.h"
#include "ARHealthPotion.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AARHealthPotion : public AARPickUp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AARHealthPotion();

	UPROPERTY(EditAnywhere)
	float HealingAmount;
	
protected:
	void ApplyEffect_Implementation(APawn* Pawn) override;
};
