// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARAICharacter.generated.h"

class UARWorldUserWidget;
class UARAttributeComponent;
class UPawnSensingComponent;

UCLASS()
class ACTIONROGUELIKE_API AARAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AARAICharacter();

protected:

	UPROPERTY()
	UARWorldUserWidget* ActiveHealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UARAttributeComponent> AttributeComp;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPawnSensingComponent> SensingComponent;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PlayerSpottedWidgetClass;

	virtual void PostInitializeComponents() override;

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastOnPlayerSpotted();
	
	void SetTarget(AActor* Target);
	
	UFUNCTION()
	void OnPawnSeen(APawn* InPawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComp, float NewHealth, float DeltaHealth);

};
