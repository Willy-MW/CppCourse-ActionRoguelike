// Fill out your copyright notice in the Description page of Project Settings.


#include "ARTeleportProjectile.h"

void AARTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(EffectTimer, this, &AARTeleportProjectile::Explode, .2f);
}

void AARTeleportProjectile::Explode()
{
	Super::Explode();

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
