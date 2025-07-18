// Fill out your copyright notice in the Description page of Project Settings.


#include "ARPickUp.h"

#include "Net/UnrealNetwork.h"


// Sets default values
AARPickUp::AARPickUp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = MeshComp;
	
	RespawnTime = 10.f;

	SetReplicates(true);
}

void AARPickUp::SetEnabled_Implementation(bool bEnabled)
{
	if (HasAuthority())
	{
		bIsEnabled = bEnabled;
	}
	
	MeshComp->SetCollisionEnabled(bEnabled ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
	MeshComp->SetVisibility(bEnabled);
}

void AARPickUp::Interact_Implementation(APawn* InteractingPawn)
{
	ApplyEffect(InteractingPawn);
}

void AARPickUp::OnRep_bIsEnabled()
{
	SetEnabled(bIsEnabled);
}

// Called when the game starts or when spawned
void AARPickUp::BeginPlay()
{
	Super::BeginPlay();
}

void AARPickUp::ApplyEffect_Implementation(APawn* Pawn)
{
	SetEnabled(false);
	
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, "SetEnabled", true);
	
	GetWorldTimerManager().SetTimer(RespawnTimer, TimerDelegate, RespawnTime, false);
}

void AARPickUp::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARPickUp, bIsEnabled);
}

