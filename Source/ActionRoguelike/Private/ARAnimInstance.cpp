// Fill out your copyright notice in the Description page of Project Settings.


#include "ARAnimInstance.h"

#include "ARActionComponent.h"
#include "GameplayTagContainer.h"

void UARAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* OwningActor = GetOwningActor();
	if (OwningActor)
	{
		ActionComp = Cast<UARActionComponent>(OwningActor->FindComponentByClass<UARActionComponent>());
	}
}

void UARAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");
	if (ActionComp)
	{
		bIsStunned = ActionComp->ActiveGameplayTags.HasTag(StunnedTag);
	}
}
