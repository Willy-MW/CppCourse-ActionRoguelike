// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "ARAction.generated.h"


class UARActionComponent;

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;
};

UCLASS(Blueprintable)
class ACTIONROGUELIKE_API UARAction : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(UARActionComponent* NewActionComponent);
	
	UPROPERTY(EditDefaultsOnly, Category="Action")
	bool bAutoStart;
	
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

	bool IsSupportedForNetworking() const override
	{
		return true;
	}
	
protected:
	UPROPERTY(Replicated)
	UARActionComponent* ActionComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Tag")
	FGameplayTagContainer GrantsTags;
	
	UPROPERTY(EditDefaultsOnly, Category="Tag")
	FGameplayTagContainer BlockedTags;

	UFUNCTION(BlueprintCallable, Category="Action")
	UARActionComponent* GetOwningComponent() const;

	UPROPERTY(ReplicatedUsing="OnRep_RepData")
	FActionRepData RepData;

	UFUNCTION()
	void OnRep_RepData();
};
