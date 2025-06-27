// Fill out your copyright notice in the Description page of Project Settings.


#include "ARActionComponent.h"

#include "ARAction.h"


UARActionComponent::UARActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UARActionComponent::AddAction(AActor* Instigator, TSubclassOf<UARAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UARAction* NewAction = NewObject<UARAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void UARActionComponent::RemoveAction(UARAction* Action)
{
	if (!ensure(Action && !Action->IsRunning()))
	{
		return;
	}
	
	Actions.Remove(Action);
}

bool UARActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UARAction* Action : Actions)
	{
		if (!(Action && Action->ActionName == ActionName))
		{
			continue;
		}

		if (!Action->CanStart(Instigator))
		{
			continue;
		}
		
		Action->StartAction(Instigator);
		return true;
	}

	return false;
}

bool UARActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (UARAction* Action : Actions)
	{
		if (!(Action && Action->ActionName == ActionName))
		{
			continue;
		}

		if (!Action->IsRunning())
		{
			continue;
		}

		Action->StopAction(Instigator);
		return true;
	}

	return false;
}


void UARActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (auto ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
}


// Called every frame
void UARActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString DebugMsg = GetNameSafe(GetOwner()) + ": " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
}

