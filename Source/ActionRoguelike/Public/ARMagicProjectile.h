// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARAction.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "ARMagicProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class ACTIONROGUELIKE_API AARMagicProjectile : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<USphereComponent> CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UParticleSystemComponent> ProjectileParticleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UAudioComponent> AudioComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visuals")
	TObjectPtr<UParticleSystem> ImpactEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visuals")
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visuals|CameraShake")
	TSubclassOf<UCameraShakeBase> CameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visuals|CameraShake")
	float CameraShakeInnerRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visuals|CameraShake")
	float CameraShakeOuterRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	TSubclassOf<UARAction> BurningActionClass;

public:
	UFUNCTION(BlueprintNativeEvent)
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* Actor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Sets default values for this actor's properties
	AARMagicProjectile();

protected:

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FGameplayTag ParryTag;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	virtual void Explode(bool bDestroy = true);

	virtual void BeginPlay() override;
};
