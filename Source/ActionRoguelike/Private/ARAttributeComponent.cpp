// Fill out your copyright notice in the Description page of Project Settings.


#include "ARAttributeComponent.h"

#include "ARGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("ar.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

// Sets default values for this component's properties
UARAttributeComponent::UARAttributeComponent()
{
	MaxHealth = 100.f;
	Health = MaxHealth;

	MaxRage = 100.f;
	Rage = 0.f;

	SetIsReplicatedByDefault(true);
}

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

float UARAttributeComponent::GetRage() const
{
	return Rage;
}

float UARAttributeComponent::GetMaxRage() const
{
	return MaxRage;
}

float UARAttributeComponent::GetRagePercent() const
{
	return Rage / MaxRage;
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
	const float NewHealth = FMath::Clamp(Health + DeltaHealth, 0, MaxHealth);
	const float ActualDelta = NewHealth - OldHealth;

	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;

		if (ActualDelta != 0.0f)
		{
			MulticastHealthChanged(InstigatorActor, Health, ActualDelta);	
		}

		if (ActualDelta < 0.f && Health == 0.f)
		{
			AARGameModeBase* GM = GetWorld()->GetAuthGameMode<AARGameModeBase>();

			if (GM)
			{
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}
	
	return ActualDelta != 0;
}

bool UARAttributeComponent::ApplyRageChange(float DeltaRage)
{
	if (DeltaRage == 0.0f)
	{
		return false;
	}

	const float OldRage = Rage;
	const float NewRage = FMath::Clamp(Rage + DeltaRage, 0, MaxRage);
	const float ActualDelta = NewRage - OldRage;

	if (GetOwner()->HasAuthority())
	{
		Rage = NewRage;

		if (ActualDelta != 0.0f)
		{
			MulticastRageChanged(Rage, ActualDelta);	
		}
	}
	
	return ActualDelta != 0;
}

bool UARAttributeComponent::IsAlive() const
{
	return Health > 0.f;
}

void UARAttributeComponent::MulticastRageChanged_Implementation(float NewHealth, float DeltaHealth)
{
	OnRageChanged.Broadcast(this, Rage, DeltaHealth);
}

void UARAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth,
                                                                  float DeltaHealth)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, DeltaHealth);
}

void UARAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UARAttributeComponent, Health);
	DOREPLIFETIME(UARAttributeComponent, MaxHealth);
	DOREPLIFETIME(UARAttributeComponent, Rage);
	DOREPLIFETIME(UARAttributeComponent, MaxRage);
}
