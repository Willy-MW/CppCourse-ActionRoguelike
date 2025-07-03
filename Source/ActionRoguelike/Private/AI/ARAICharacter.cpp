// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/ARAICharacter.h"

#include "AIController.h"
#include "ARAttributeComponent.h"
#include "ARWorldUserWidget.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AARAICharacter::AARAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("SensingComponent"));
	AttributeComp = CreateDefaultSubobject<UARAttributeComponent>(TEXT("AttributeComp"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
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

		UObject* PreviousTarget = AIC->GetBlackboardComponent()->GetValueAsObject("TargetActor");
		if (PreviousTarget != Target)
		{
			AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", Target);

			UARWorldUserWidget* PlayerSpottedWidget = CreateWidget<UARWorldUserWidget>(GetWorld(), PlayerSpottedWidgetClass);
			if (PlayerSpottedWidget)
			{
				PlayerSpottedWidget->AttachedActor = this;
				PlayerSpottedWidget->AddToViewport();
			}
		}
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

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
			
			SetLifeSpan(10.f);
		}
	}
}


