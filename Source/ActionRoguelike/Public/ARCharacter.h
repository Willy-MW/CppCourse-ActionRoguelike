// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ARCharacter.generated.h"

class UARAttributeComponent;
class UARInteractionComponent;
struct FInputActionInstance;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class ACTIONROGUELIKE_API AARCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AARCharacter();

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TSoftObjectPtr<UInputMappingContext> CurrentMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> PrimaryAttackAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> SecondaryAttackAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> DashAttackAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> PrimaryInteractAction;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UARAttributeComponent> AttributeComp;

	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> PrimaryAttackClass;

	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> SecondaryAttackClass;

	UPROPERTY(EditAnywhere, Category="Attack")
	TSubclassOf<AActor> DashAttackClass;
	
	UPROPERTY(EditAnywhere, Category="Attack")
	TObjectPtr<UAnimMontage> AttackAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack")
	TObjectPtr<UParticleSystem> CastEffect;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UARInteractionComponent> InteractionComp;

	FTimerHandle TimerHandle_Attack;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void Move(const FInputActionInstance& Instance);

	void Look(const FInputActionInstance& Instance);

	UFUNCTION()
	void Attack_TimeElapsed(const TSubclassOf<AActor>& Projectile);
	
	void PrimaryAttack();
	
	void SecondaryAttack();

	void DashAttack();

	void PrimaryInteract();

	FVector PerformLineTraceFromCamera() const;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComp, float NewHealth, float DeltaHealth);

	virtual void PostInitializeComponents() override;

	virtual FVector GetPawnViewLocation() const override;

public:	
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100.f);
};
