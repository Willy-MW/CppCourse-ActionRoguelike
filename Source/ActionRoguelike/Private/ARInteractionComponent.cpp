// Fill out your copyright notice in the Description page of Project Settings.


#include "ARInteractionComponent.h"

#include "ARGameplayInterface.h"

static TAutoConsoleVariable<bool> CVarDrawDebug(TEXT("ar.DrawDebug"), false, TEXT("Enable drawing debug objects"), ECVF_Cheat);

// Sets default values for this component's properties
UARInteractionComponent::UARInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UARInteractionComponent::PrimaryInteract()
{
	bool bDrawDebug = CVarDrawDebug.GetValueOnGameThread();
	
	TArray<FHitResult> HitResults;
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* InteractingActor = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	InteractingActor->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + EyeRotation.Vector() * 1000.f;

	float radius = 30.f;
	FCollisionShape Shape;
	Shape.SetSphere(radius);
	
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(HitResults, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor HitColor = bBlockingHit ? FColor::Green : FColor::Red;
	
	for (FHitResult& HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor();

		if (HitActor)
		{
			if (bDrawDebug)
			{
				DrawDebugSphere(GetWorld(), HitResult.Location, radius, 10, HitColor, false, 2.f);
			}
			
			if (HitActor->Implements<UARGameplayInterface>())
			{
				APawn* InteractingPawn = Cast<APawn>(InteractingActor);
		
				IARGameplayInterface::Execute_Interact(HitActor, InteractingPawn);
				break;
			}
		}
	}

	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, HitColor, false, 2.f, 0, 2.f);	
	}
}


// Called when the game starts
void UARInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UARInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

