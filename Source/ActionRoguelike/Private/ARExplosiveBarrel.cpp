// Fill out your copyright notice in the Description page of Project Settings.


#include "ARExplosiveBarrel.h"

#include "ARMagicProjectile.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AARExplosiveBarrel::AARExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetSimulatePhysics(true);

	ForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("ForceComp"));
	ForceComp->SetupAttachment(MeshComp);
	ForceComp->ImpulseStrength = 2000.f;
	ForceComp->Radius = 700.f;
	ForceComp->bImpulseVelChange = true;
	
}

void AARExplosiveBarrel::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->GetClass()->IsChildOf(AARMagicProjectile::StaticClass()))
	{
		Explode();
	}
}

// Called when the game starts or when spawned
void AARExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	MeshComp->OnComponentHit.AddDynamic(this, &AARExplosiveBarrel::OnHit);
}

void AARExplosiveBarrel::Explode()
{
	ForceComp->FireImpulse();
}

// Called every frame
void AARExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

