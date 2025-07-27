// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "ARActionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStateChanged, UARActionComponent*, OwningComp, UARAction*, Action);

class UARAction;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API UARActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tags")
	FGameplayTagContainer ActiveGameplayTags;
	
	// Sets default values for this component's properties
	UARActionComponent();

	UFUNCTION(BlueprintCallable, Category="Actions")
	void AddAction(AActor* Instigator, TSubclassOf<UARAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category="Actions")
	void RemoveAction(UARAction* ActionClass);

	UFUNCTION(BlueprintCallable, Category="Actions")
	bool StartActionByName(AActor* Instigator, FName ActionName);
	
	UFUNCTION(BlueprintCallable, Category="Actions")
	bool StopActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category="Actions")
	bool HasAction(const TSubclassOf<UARAction>& ActionClass);

	UPROPERTY(BlueprintAssignable, Category="Actions")
	FOnActionStateChanged OnActionStarted;
	
	UPROPERTY(BlueprintAssignable, Category="Actions")
	FOnActionStateChanged OnActionStopped;
protected:

	UFUNCTION(Server, Reliable)
	void ServerStartAction(AActor* Instigator, FName ActionName);

	UFUNCTION(Server, Reliable)
	void ServerStopAction(AActor* Instigator, FName ActionName);

	UPROPERTY(EditAnywhere, Category="Actions")
	TArray<TSubclassOf<UARAction> > DefaultActions;

	UPROPERTY(BlueprintReadOnly, Replicated)
	TArray<UARAction*> Actions;
	
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
