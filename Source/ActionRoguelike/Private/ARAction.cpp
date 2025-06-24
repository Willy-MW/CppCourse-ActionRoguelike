// Fill out your copyright notice in the Description page of Project Settings.


#include "ARAction.h"

#include "ARActionComponent.h"

void UARAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));

	UARActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}

void UARAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));

	ensureAlways(bIsRunning);

	UARActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

UWorld* UARAction::GetWorld() const
{
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp)
	{
		return Comp->GetWorld();
	}

	return nullptr;
}

bool UARAction::IsRunning() const
{
	return bIsRunning;
}

bool UARAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}
	
	UARActionComponent* Comp = GetOwningComponent();

	if (Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}

	return true;
}

UARActionComponent* UARAction::GetOwningComponent() const
{
	return Cast<UARActionComponent>(GetOuter());
}
