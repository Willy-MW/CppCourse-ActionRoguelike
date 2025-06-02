// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ARBTService_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UARBTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector AttackRangeKey;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
