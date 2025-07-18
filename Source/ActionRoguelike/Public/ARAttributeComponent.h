// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, UARAttributeComponent*, OwningComp, float, NewHealth, float, DeltaHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRageChanged, UARAttributeComponent*, OwningComp, float, NewRage, float, DeltaRage);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UARAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static UARAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName="IsAlive"))
	static bool IsActorAlive(AActor* Actor);

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
	
	// Sets default values for this component's properties
	UARAttributeComponent();

	float GetHealth() const;

	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealthPercent() const;

	UPROPERTY(BlueprintAssignable)
	FOnRageChanged OnRageChanged;

	float GetRage() const;

	float GetMaxRage() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetRagePercent() const;

	bool Kill(AActor* InstigatorActor);
	
	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float DeltaHealth);

	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool ApplyRageChange(float DeltaRage);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsAlive() const;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category="Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category="Attributes")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category="Attributes")
	float Rage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category="Attributes")
	float MaxRage;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float DeltaHealth);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRageChanged(float NewHealth, float DeltaHealth);
};
