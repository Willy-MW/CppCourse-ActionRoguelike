// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ARPlayerState.generated.h"

class UARSaveGame;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, AARPlayerState*, PlayerState, int32, NewCredits,
                                               int32, Delta);

UCLASS()
class ACTIONROGUELIKE_API AARPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category="Credits", ReplicatedUsing="OnRep_Credits")
	int32 Credits;

public:
	UFUNCTION(BlueprintCallable, Category="Credits")
	int32 GetCredits() const;

	UFUNCTION(BlueprintCallable, Category="Delta")
	void AddCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category="Delta")
	void RemoveCredits(int32 Delta);

	UPROPERTY(BlueprintAssignable, Category="Credits")
	FOnCreditsChanged OnCreditsChanged;

	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(UARSaveGame* SaveObject);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(UARSaveGame* SaveObject);

protected:
	UFUNCTION()
	void OnRep_Credits(int32 OldCredits);
};
