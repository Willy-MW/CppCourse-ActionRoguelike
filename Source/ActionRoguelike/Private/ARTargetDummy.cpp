// Fill out your copyright notice in the Description page of Project Settings.


#include "ARTargetDummy.h"

#include "ARAttributeComponent.h"

// Sets default values
AARTargetDummy::AARTargetDummy()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<UARAttributeComponent>(TEXT("AttributeComp"));
}

// Called when the game starts or when spawned
void AARTargetDummy::BeginPlay()
{
	Super::BeginPlay();

	AttributeComp->OnHealthChanged.AddDynamic(this, &AARTargetDummy::OnHealthChanged);
}

void AARTargetDummy::OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComp, float NewHealth,
	float DeltaHealth)
{
	if (DeltaHealth < 0.f)
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeOfHit", GetWorld()->TimeSeconds);	
	}
}

