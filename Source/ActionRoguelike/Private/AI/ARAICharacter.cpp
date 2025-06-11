// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARAICharacter.h"

#include "AIController.h"
#include "ARAttributeComponent.h"
#include "ARWorldUserWidget.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AARAICharacter::AARAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("SensingComponent"));
	AttributeComp = CreateDefaultSubobject<UARAttributeComponent>(TEXT("AttributeComp"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AARAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SensingComponent->OnSeePawn.AddDynamic(this, &AARAICharacter::OnPawnSeen);
	
	AttributeComp->OnHealthChanged.AddDynamic(this, &AARAICharacter::OnHealthChanged);
}

void AARAICharacter::SetTarget(AActor* Target)
{
	if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", Target);;
		DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.f, true);
	}
}

void AARAICharacter::OnPawnSeen(APawn* InPawn)
{
	SetTarget(InPawn);
}

void AARAICharacter::OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComp, float NewHealth,
	float DeltaHealth)
{
	if (DeltaHealth < 0.f)
	{
		if (InstigatorActor != this)
		{
			SetTarget(InstigatorActor);
		}

		if (ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<UARWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}

		GetMesh()->SetScalarParameterValueOnMaterials("TimeOfHit", GetWorld()->TimeSeconds);	
		
		if (NewHealth <= 0.f)
		{
			if (AAIController* AIC = Cast<AAIController>(GetController()))
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}

			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");
			
			SetLifeSpan(10.f);
		}
	}
}


