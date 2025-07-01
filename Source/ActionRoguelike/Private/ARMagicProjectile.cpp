// Fill out your copyright notice in the Description page of Project Settings.


#include "ARMagicProjectile.h"

#include "ARActionComponent.h"
#include "ARGameplayFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AARMagicProjectile::AARMagicProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetCollisionProfileName("Projectile");
	RootComponent = CollisionComp;

	ProjectileParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ProjectileParticleComp"));
	ProjectileParticleComp->SetupAttachment(CollisionComp);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	ProjectileMovementComp->InitialSpeed = 2000.f;
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->bInitialVelocityInLocalSpace = true;
	ProjectileMovementComp->ProjectileGravityScale = 0.f;

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(CollisionComp);

	Damage = 20.f;

	SetReplicates(true);
}

void AARMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AARMagicProjectile::OnActorOverlap);
}

void AARMagicProjectile::Explode(bool bDestroy)
{
	ProjectileMovementComp->StopMovementImmediately();
	AudioComp->Stop();

	if (ImpactSound) UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, GetActorLocation());
	if (ImpactEffect) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, GetActorLocation());
	if (CameraShake) UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShake, GetActorLocation(), CameraShakeInnerRadius, CameraShakeOuterRadius);
	
	if (bDestroy) Destroy();
}

void AARMagicProjectile::OnActorOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* Actor,
                                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                       bool bFromSweep, const FHitResult& SweepResult)
{
	if (Actor && Actor != GetInstigator())
	{
		UARActionComponent* ActionComp = Actor->FindComponentByClass<UARActionComponent>();
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			ProjectileMovementComp->Velocity = -ProjectileMovementComp->Velocity;

			SetInstigator(Cast<APawn>(Actor));
			return;
		}
		
		UARGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), Actor, Damage, SweepResult);

		Explode();

		if (ActionComp)
		{
			ActionComp->AddAction(GetInstigator(), BurningActionClass);
		}
	}
}

void AARMagicProjectile::BeginPlay()
{
	Super::BeginPlay();

	AudioComp->Play();
}

