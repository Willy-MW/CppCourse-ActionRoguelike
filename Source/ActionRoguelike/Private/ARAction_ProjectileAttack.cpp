// Fill out your copyright notice in the Description page of Project Settings.


#include "ARAction_ProjectileAttack.h"

#include "ARCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UARAction_ProjectileAttack::UARAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";
	AttackAnimDelay = 0.2f;
}

void UARAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (!Character)
	{
		return;
	}

	Character->PlayAnimMontage(AttackAnim);
	
	FTimerHandle TimerHandle_AttackDelay;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, "AttackDelay_Elapsed", Character);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, TimerDelegate, AttackAnimDelay, false);
}

void UARAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	AARCharacter* Character = Cast<AARCharacter>(InstigatorCharacter);
	if (!Character)
	{
		StopAction(InstigatorCharacter);
		return;
	}
	
	FVector SpawnLocation = Character->GetMesh()->GetSocketLocation(HandSocketName);
	FVector TargetLocation = Character->PerformLineTraceFromCamera();
	FTransform SpawnTransform = FTransform(UKismetMathLibrary::FindLookAtRotation(SpawnLocation, TargetLocation), SpawnLocation);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = Character;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParams);
	
	if (CastingEffect) UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName);

	StopAction(InstigatorCharacter);
}