// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARMagicProjectile.h"
#include "ARTeleportProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARTeleportProjectile : public AARMagicProjectile
{
	GENERATED_BODY()

public:
	AARTeleportProjectile();
	
protected:

	FTimerHandle EffectTimer;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Explode(bool bDestroy = true) override;
	
	void PerformTeleport();
	
};
