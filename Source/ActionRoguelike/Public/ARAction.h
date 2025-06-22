// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ARAction.generated.h"


UCLASS(Blueprintable)
class ACTIONROGUELIKE_API UARAction : public UObject
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditDefaultsOnly, Category="Action")
	FName ActionName;

	UFUNCTION(BlueprintNativeEvent, Category="Action")
	void StartAction(AActor* Instigator);
	
	UFUNCTION(BlueprintNativeEvent, Category="Action")
	void StopAction(AActor* Instigator);
};
