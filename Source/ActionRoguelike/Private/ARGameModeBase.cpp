// Fill out your copyright notice in the Description page of Project Settings.


#include "ARGameModeBase.h"

#include "ARAttributeComponent.h"
#include "ARCharacter.h"
#include "ARGameplayInterface.h"
#include "ARPlayerState.h"
#include "ARSaveGame.h"
#include "EngineUtils.h"
#include "AI/ARAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("ar.SpawnBots"), true, TEXT("Enable spawning of bots via timer"), ECVF_Cheat);

AARGameModeBase::AARGameModeBase()
{
	SpawnTimerInterval = 2.f;
	BotKilledCredits = 30;

	SlotName = "SaveGame01";
}

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

		return;
	}

	AARAICharacter* Bot = Cast<AARAICharacter>(KilledActor);
	Player = Cast<AARCharacter>(Killer);

	if (Bot && Player)
	{
		AARPlayerState* PS = Cast<AARPlayerState>( Player->GetPlayerState());

		if (!PS)
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerState is not valid ARPLayerState class."));
			return;
		}

		PS->AddCredits(BotKilledCredits);
	}
}

void AARGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &AARGameModeBase::SpawnBotTimerElapsed,
	                                SpawnTimerInterval, true);
}

void AARGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	AARPlayerState* PS = NewPlayer->GetPlayerState<AARPlayerState>();
	if (PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}
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

void AARGameModeBase::WriteSaveGame()
{
	for (int32 i =0; i<GameState->PlayerArray.Num(); i++)
	{
		AARPlayerState* PS = Cast<AARPlayerState>( GameState->PlayerArray[i] );
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break;
		}
	}

	CurrentSaveGame->SavedActors.Empty();

	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;

		if (!Actor->Implements<UARGameplayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.Transform = Actor->GetActorTransform();

		FMemoryWriter MemWriter(ActorData.ByteData);

		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		Ar.ArIsSaveGame = true;
		
		Actor->Serialize(Ar);

		CurrentSaveGame->SavedActors.Add(ActorData);
	}
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}

void AARGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<UARSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName,0));

		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load save game."));
			return;
		}

		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data."));

		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;

			if (!Actor->Implements<UARGameplayInterface>())
			{
				continue;
			}

			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				if (ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);

					FMemoryReader MemReader(ActorData.ByteData);

					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					Ar.ArIsSaveGame = true;
		
					Actor->Serialize(Ar);

					IARGameplayInterface::Execute_OnActorLoaded(Actor);
					
					break;
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<UARSaveGame>(UGameplayStatics::CreateSaveGameObject(UARSaveGame::StaticClass()));
	}
}

void AARGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
}


