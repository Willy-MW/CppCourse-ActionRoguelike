// Fill out your copyright notice in the Description page of Project Settings.


#include "ARTeleportProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

AARTeleportProjectile::AARTeleportProjectile()
{
	ProjectileMovementComp->InitialSpeed = 4000.0f;
}

void AARTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, FName("Explode"), false);
	
	GetWorld()->GetTimerManager().SetTimer(EffectTimer, TimerDel, .2f, false);
}

void AARTeleportProjectile::Explode(bool bDestroy)
{
	Super::Explode(false);

	ProjectileParticleComp->Deactivate();

	GetWorld()->GetTimerManager().SetTimer(EffectTimer, this, &AARTeleportProjectile::PerformTeleport, .2f);
}

void AARTeleportProjectile::PerformTeleport()
{
	FVector TeleportLocation = GetActorLocation();
	TeleportLocation.Z += 20.f;

	FRotator TeleportRotation = GetInstigator()->GetActorRotation();

	GetInstigator()->TeleportTo(TeleportLocation, TeleportRotation);

	Destroy();
}
