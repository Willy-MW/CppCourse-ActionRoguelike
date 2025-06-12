// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARBTS_CheckLowHealth.h"

#include "AIController.h"
#include "ARAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void UARBTS_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	bool bIsLowHealth = false;

	if (ensure(BlackboardComponent))
	{
		AAIController* MyController = OwnerComp.GetAIOwner();

		if (ensure(MyController))
		{
			APawn* AIPawn = MyController->GetPawn();
			if (ensure(AIPawn))
			{
				UARAttributeComponent* AttributeComponent = UARAttributeComponent::GetAttributes(AIPawn);

				if (AttributeComponent)
				{
					bIsLowHealth = AttributeComponent->GetHealthPercent() < ThresholdPercent;
				}
			}
		}
	}
	BlackboardComponent->SetValueAsBool(LowHealthKey.SelectedKeyName, (bIsLowHealth));
}
