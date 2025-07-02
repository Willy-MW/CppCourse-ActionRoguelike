// Fill out your copyright notice in the Description page of Project Settings.


#include "ARActionEffect_Thorns.h"

#include "ARActionComponent.h"
#include "ARAttributeComponent.h"
#include "ARGameplayFunctionLibrary.h"

UARActionEffect_Thorns::UARActionEffect_Thorns()
{
	DeflectionPercent = 0.5;
}

void UARActionEffect_Thorns::DeflectDamage(AActor* DamageInstigator, float DamageReceived)
{
	int DeflectedDamage = FMath::RoundToInt(DamageReceived * DeflectionPercent);
	
	UARGameplayFunctionLibrary::ApplyDamage(GetOwningComponent()->GetOwner(), DamageInstigator, DeflectedDamage);
}

void UARActionEffect_Thorns::OnOwnerHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComp,
                                                  float NewHealth, float DeltaHealth)
{
	if (DeltaHealth > 0.f || InstigatorActor == OwningComp->GetOwner())
	{
		return;
	}

	DeflectDamage(InstigatorActor, -DeltaHealth);
}

void UARActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	UARAttributeComponent* AttributeComp = UARAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());

	if (!AttributeComp)
	{
		return;
	}

	AttributeComp->OnHealthChanged.AddDynamic(this, &UARActionEffect_Thorns::OnOwnerHealthChanged);

	Super::StartAction_Implementation(Instigator);
}

void UARActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	UARAttributeComponent* AttributeComp = UARAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());

	if (!ensure(AttributeComp))
	{
		return;
	}

	AttributeComp->OnHealthChanged.RemoveDynamic(this, &UARActionEffect_Thorns::OnOwnerHealthChanged);
	
	Super::StopAction_Implementation(Instigator);
}
