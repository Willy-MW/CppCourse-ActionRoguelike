// Fill out your copyright notice in the Description page of Project Settings.


#include "ARTeleportProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AARTeleportProjectile::AARTeleportProjectile()
{
	
}

void AARTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->OnComponentHit.AddDynamic(this, &AARTeleportProjectile::OnHit);
	
	CollisionComp->IgnoreActorWhenMoving(GetInstigator(), true);
	GetWorld()->GetTimerManager().SetTimer(EffectTimer, this, &AARTeleportProjectile::Explode, .2f);
}

void AARTeleportProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	EffectTimer.Invalidate();
	Explode();
}

void AARTeleportProjectile::Explode()
{
	ProjectileMovementComp->StopMovementImmediately();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeEffect, GetActorLocation());

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

void AARTeleportProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
