// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARPickUp.h"
#include "ARPickUp_Action.generated.h"

class UARAction;

UCLASS()
class ACTIONROGUELIKE_API AARPickUp_Action : public AARPickUp
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="PickUp")
	TSubclassOf<UARAction> GrantedAction;

protected:
	void ApplyEffect_Implementation(APawn* Pawn) override;
};
