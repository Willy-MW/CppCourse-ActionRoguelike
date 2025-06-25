// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "ARAction.generated.h"


UCLASS(Blueprintable)
class ACTIONROGUELIKE_API UARAction : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Action")
	FName ActionName;

	UFUNCTION(BlueprintNativeEvent, Category="Action")
	void StartAction(AActor* Instigator);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Action")
	void StopAction(AActor* Instigator);

	UWorld* GetWorld() const override;

	UFUNCTION(BlueprintNativeEvent, Category="Actions")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category="Actions")
	bool IsRunning() const;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Tag")
	FGameplayTagContainer GrantsTags;
	
	UPROPERTY(EditDefaultsOnly, Category="Tag")
	FGameplayTagContainer BlockedTags;

	UFUNCTION(BlueprintCallable, Category="Action")
	UARActionComponent* GetOwningComponent() const;

	bool bIsRunning;
};
