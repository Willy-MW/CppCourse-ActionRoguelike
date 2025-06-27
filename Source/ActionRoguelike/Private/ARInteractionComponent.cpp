// Fill out your copyright notice in the Description page of Project Settings.


#include "ARInteractionComponent.h"

#include "ARGameplayInterface.h"
#include "ARWorldUserWidget.h"
#include "Blueprint/UserWidget.h"

static TAutoConsoleVariable<bool> CVarDrawDebug(TEXT("ar.DrawDebug"), false, TEXT("Enable drawing debug objects"), ECVF_Cheat);

UARInteractionComponent::UARInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TraceRadius = 30.f;
	TraceDistance = 1000.f;
	CollisionChannel = ECC_WorldDynamic;
	
}

void UARInteractionComponent::PrimaryInteract()
{
	if (FocusedActor == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "No FocusedActor to interact");
		return;
	}
	
	APawn* InteractingPawn = Cast<APawn>(GetOwner());
		
	IARGameplayInterface::Execute_Interact(FocusedActor, InteractingPawn);
}

void UARInteractionComponent::FindBestInteractable()
{
	bool bDrawDebug = CVarDrawDebug.GetValueOnGameThread();
	
	TArray<FHitResult> HitResults;
	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* InteractingActor = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	InteractingActor->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + EyeRotation.Vector() * TraceDistance;

	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);
	
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(HitResults, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor HitColor = bBlockingHit ? FColor::Green : FColor::Red;

	FocusedActor = nullptr;
	
	for (FHitResult& HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor();

		if (HitActor)
		{
			if (bDrawDebug)
			{
				DrawDebugSphere(GetWorld(), HitResult.Location, TraceRadius, 10, HitColor, false, 2.f);
			}
			
			if (HitActor->Implements<UARGameplayInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}

	if (FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<UARWorldUserWidget>(GetWorld(), DefaultWidgetClass);	
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;

			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();	
			}
		}
	} else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}

	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, HitColor, false, 2.f, 0, 2.f);	
	}
}

void UARInteractionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FindBestInteractable();
}

