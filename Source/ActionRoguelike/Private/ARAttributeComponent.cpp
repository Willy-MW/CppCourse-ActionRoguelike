// Fill out your copyright notice in the Description page of Project Settings.


#include "ARAttributeComponent.h"

#include "ARGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("ar.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

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

float UARAttributeComponent::GetHealthPercent() const
{
	return Health / MaxHealth;
}

bool UARAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -Health);
}

bool UARAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float DeltaHealth)
{
	if (!GetOwner()->CanBeDamaged() && DeltaHealth < 0.0f)
	{
		return false;
	}

	if (DeltaHealth < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		DeltaHealth *= DamageMultiplier;
	}

	const float OldHealth = Health;

	Health = FMath::Clamp(Health + DeltaHealth, 0, MaxHealth);

	const float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	if (ActualDelta < 0.f && Health == 0.f)
	{
		AARGameModeBase* GM = GetWorld()->GetAuthGameMode<AARGameModeBase>();

		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

	return ActualDelta != 0;
}

bool UARAttributeComponent::IsAlive() const
{
	return Health > 0.f;
}
