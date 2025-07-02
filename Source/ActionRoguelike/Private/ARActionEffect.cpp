// Fill out your copyright notice in the Description page of Project Settings.


#include "ARActionEffect.h"

#include "ARActionComponent.h"

UARActionEffect::UARActionEffect()
{
	bAutoStart = true;
	Duration = 0.f;
	Period = 0.f;
}

void UARActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (Duration > 0.f)
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, "StopAction", Instigator);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, TimerDelegate, Duration, false);
	}

	if (Period > 0.f)
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, TimerDelegate, Period, true);
	}
}

void UARActionEffect::StopAction_Implementation(AActor* Instigator)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}
	
	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);

	UARActionComponent* Comp = GetOwningComponent();
	if (Comp)
	{
		Comp->RemoveAction(this);
	}
}

void UARActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
	
}