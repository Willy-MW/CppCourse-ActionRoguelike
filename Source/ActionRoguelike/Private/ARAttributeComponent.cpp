// Fill out your copyright notice in the Description page of Project Settings.


#include "ARAttributeComponent.h"

// Sets default values for this component's properties
UARAttributeComponent::UARAttributeComponent()
{
	Health = 100.f;
}

bool UARAttributeComponent::ApplyHealthChange(float DeltaHealth)
{
	Health += DeltaHealth;

	return true;
}
