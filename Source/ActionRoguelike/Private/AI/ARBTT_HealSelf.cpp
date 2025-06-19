// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARBTT_HealSelf.h"

#include "ARAttributeComponent.h"
#include "AIController.h"

EBTNodeResult::Type UARBTT_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();

	if (ensure(MyController))
	{
		APawn* AIPawn = MyController->GetPawn();
		if (!AIPawn)
		{
			return EBTNodeResult::Failed;
		}

		UARAttributeComponent* AttributeComponent = UARAttributeComponent::GetAttributes(AIPawn);

		if (!AttributeComponent)
		{
			return EBTNodeResult::Failed;
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Healing self"));
		AttributeComponent->ApplyHealthChange(AIPawn, AttributeComponent->GetMaxHealth() - AttributeComponent->GetHealth());
		
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
