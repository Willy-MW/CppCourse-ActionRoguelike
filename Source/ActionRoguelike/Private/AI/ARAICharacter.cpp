// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARAICharacter.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AARAICharacter::AARAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("SensingComponent"));
}

void AARAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SensingComponent->OnSeePawn.AddDynamic(this, &AARAICharacter::OnPawnSeen);
}

void AARAICharacter::OnPawnSeen(APawn* InPawn)
{
	if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();

		BBComp->SetValueAsObject("TargetActor", InPawn);

		DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.f, true);
	}
}


