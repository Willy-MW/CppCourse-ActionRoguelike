// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARActionEffect.h"
#include "ARAttributeComponent.h"
#include "ARActionEffect_Thorns.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARActionEffect_Thorns : public UARActionEffect
{
	GENERATED_BODY()

public:
	UARActionEffect_Thorns();

	void DeflectDamage(AActor* DamageInstigator, float DamageReceived);
	
	UFUNCTION()
	void OnOwnerHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComp, float NewHealth, float DeltaHealth);
	
	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	float DeflectionPercent;
};
