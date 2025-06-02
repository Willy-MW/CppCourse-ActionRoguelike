// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ARAIController.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AARAIController : public AAIController
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBehaviorTree> BehaviorTree;

	virtual void BeginPlay() override;
	
};
