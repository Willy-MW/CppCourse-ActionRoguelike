// Fill out your copyright notice in the Description page of Project Settings.


#include "ARHealthPotion.h"

#include "ARAttributeComponent.h"
#include "ARGameplayFunctionLibrary.h"
#include "ARPlayerState.h"

#define LOCTEXT_NAMESPACE "InteractableActors"

AARHealthPotion::AARHealthPotion()
{
	PrimaryActorTick.bCanEverTick = true;

	HealingAmount = 20.f;
	CreditCost = 20;
}

FText AARHealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	UARAttributeComponent* AttributeComp = UARAttributeComponent::GetAttributes(InstigatorPawn);
	if (AttributeComp && AttributeComp->IsFullHealth())
	{
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full health.");
	}

	return FText::Format(LOCTEXT("HealthPotion_InteractMessage", "Cost {0} Credits. Restores health to maximum."), CreditCost);
}

void AARHealthPotion::ApplyEffect_Implementation(APawn* Pawn)
{
	AARPlayerState* PS = Pawn->GetPlayerState<AARPlayerState>();

	if (!PS)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerState is invalid AARPlayerState class."));
		return;
	}

	if (PS->GetCredits() < CreditCost)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.0f, FColor::Yellow, "Insufficient credits");
		return;
	}
	
	UARAttributeComponent* AttributeComp = Pawn->FindComponentByClass<UARAttributeComponent>();
	if (AttributeComp && AttributeComp->GetHealth() < AttributeComp->GetMaxHealth())
	{
		PS->RemoveCredits(CreditCost);
		
		AttributeComp->ApplyHealthChange(this, HealingAmount);

		Super::ApplyEffect_Implementation(Pawn);
	}
}

#undef LOCTEXT_NAMESPACE