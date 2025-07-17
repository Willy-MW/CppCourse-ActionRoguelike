// Fill out your copyright notice in the Description page of Project Settings.


#include "ARItemChest.h"

#include "Net/UnrealNetwork.h"

// Sets default values
AARItemChest::AARItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComp"));
	RootComponent = BaseMeshComp;

	LidMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMeshComp"));
	LidMeshComp->SetupAttachment(BaseMeshComp);

	TargetPitch = 110.f;

	SetReplicates(true);
}

void AARItemChest::Interact_Implementation(APawn* InteractingPawn)
{
	bLidOpen = !bLidOpen;
	OnRep_LidOpened();
}

void AARItemChest::OnActorLoaded_Implementation()
{
	OnRep_LidOpened();
}

void AARItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AARItemChest, bLidOpen);
}

void AARItemChest::OnRep_LidOpened()
{
	float CurrentPitch = bLidOpen ? TargetPitch : 0;
	LidMeshComp->SetRelativeRotation(FRotator(CurrentPitch, 0.f, 0.f));
}

