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
	// Sets default values for this actor's properties
	AARTeleportProjectile();

protected:

	FTimerHandle EffectTimer;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> ExplodeEffect;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	void Explode();
	
	void PerformTeleport();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
