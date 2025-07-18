// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "ARPickUp.generated.h"

class USphereComponent;

UCLASS()
class ACTIONROGUELIKE_API AARPickUp : public AActor, public IARGameplayInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AARPickUp();

	UPROPERTY(EditAnywhere, Category="PickUp")
	float RespawnTime;

	UFUNCTION(BlueprintNativeEvent)
	void SetEnabled(bool bEnabled);

	void Interact_Implementation(APawn* InteractingPawn) override;

protected:

	UPROPERTY(ReplicatedUsing=OnRep_bIsEnabled)
	bool bIsEnabled;

	UFUNCTION()
	void OnRep_bIsEnabled();

	FTimerHandle RespawnTimer;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComp;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
	void ApplyEffect(APawn* Pawn);
};
