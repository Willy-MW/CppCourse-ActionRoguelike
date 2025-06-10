// Fill out your copyright notice in the Description page of Project Settings.


#include "ARAttributeComponent.h"

UARAttributeComponent* UARAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return FromActor->FindComponentByClass<UARAttributeComponent>();
	}

	return nullptr;
}

bool UARAttributeComponent::IsActorAlive(AActor* Actor)
{
	UARAttributeComponent* AttributeComponent = GetAttributes(Actor);
	if (AttributeComponent)
	{
		return AttributeComponent->IsAlive();
	}

	return false;
}

// Sets default values for this component's properties
UARAttributeComponent::UARAttributeComponent()
{
	MaxHealth = 100.f;
	Health = MaxHealth;
}

float UARAttributeComponent::GetHealth() const
{
	return Health;
}

float UARAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}

bool UARAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float DeltaHealth)
{
	float NewHealth = FMath::Clamp(Health + DeltaHealth, 0, MaxHealth);

	if (NewHealth != Health)
	{
		Health = NewHealth;
		OnHealthChanged.Broadcast(InstigatorActor, this, Health, DeltaHealth);
	
		return true;
	}

	return false;
}

bool UARAttributeComponent::IsAlive() const
{
	return Health > 0.f;
}
