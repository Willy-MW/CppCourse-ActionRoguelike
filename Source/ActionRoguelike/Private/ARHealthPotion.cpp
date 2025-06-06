// Fill out your copyright notice in the Description page of Project Settings.


#include "ARHealthPotion.h"

#include "ARAttributeComponent.h"


// Sets default values
AARHealthPotion::AARHealthPotion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealingAmount = 20.f;
}

void AARHealthPotion::ApplyEffect_Implementation(APawn* Pawn)
{
	UARAttributeComponent* AttributeComp = Cast<UARAttributeComponent>(Pawn->GetComponentByClass(UARAttributeComponent::StaticClass()));
	if (AttributeComp && (AttributeComp->GetHealth() < AttributeComp->GetMaxHealth()))
	{
		AttributeComp->ApplyHealthChange(HealingAmount);

		Super::ApplyEffect_Implementation(Pawn);
	}
}

