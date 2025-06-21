// Fill out your copyright notice in the Description page of Project Settings.


#include "ARGameModeBase.h"

#include "ARAttributeComponent.h"
#include "ARCharacter.h"
#include "EngineUtils.h"
#include "AI/ARAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("ar.SpawnBots"), true, TEXT("Enable spawning of bots via timer"), ECVF_Cheat);

void AARGameModeBase::OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
                                      EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS query Failed!"))
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}

void AARGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot disabled by CVarSpawnBots"));
		return;
	}
	
	int32 NrOfAliveBots = 0;

	for (TActorIterator<AARAICharacter> It(GetWorld()); It; ++It)
	{
		AARAICharacter* Bot = *It;

		UARAttributeComponent* AttributeComponent = UARAttributeComponent::GetAttributes(Bot);
		if (AttributeComponent && AttributeComponent->IsAlive())
		{
			NrOfAliveBots++;
		}
	}

	float MaxBotCount = 10.f;

	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NrOfAliveBots >= MaxBotCount)
	{
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(
		this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AARGameModeBase::OnQueryFinished);
	}
}

void AARGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		
		RestartPlayer(Controller);
	}
}

void AARGameModeBase::OnActorKilled(AActor* KilledActor, AActor* Killer)
{
	AARCharacter* Player = Cast<AARCharacter>(KilledActor);

	if (Player)
	{
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("RespawnPlayerElapsed"), Player->GetController());

		float RespawnDelay = 2.f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, TimerDelegate, RespawnDelay, false);
	}
}

void AARGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AARGameModeBase::SpawnBotTimerElapsed,
	                                SpawnTimerInterval, true);
}

void AARGameModeBase::KillAllBots()
{
	for (TActorIterator<AARAICharacter> It(GetWorld()); It; ++It)
	{
		AARAICharacter* Bot = *It;

		UARAttributeComponent* AttributeComponent = UARAttributeComponent::GetAttributes(Bot);
		if (AttributeComponent && AttributeComponent->IsAlive())
		{
			AttributeComponent->Kill(this);
		}
	}
}


