// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRoguelike/Public/ARCharacter.h"

#include "ARActionComponent.h"
#include "ARAttributeComponent.h"
#include "ARInteractionComponent.h"
#include "ARMagicProjectile.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AARCharacter::AARCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	InteractionComp = CreateDefaultSubobject<UARInteractionComponent>(TEXT("InteractionComp"));
	AttributeComp = CreateDefaultSubobject<UARAttributeComponent>(TEXT("AttributeComp"));
	ActionComp = CreateDefaultSubobject<UARActionComponent>(TEXT("ActionComp"));
	
	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void AARCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AARCharacter::Move(const FInputActionInstance& Instance)
{
	const FVector2D MovementVector = Instance.GetValue().Get<FVector2D>();
	const FRotator YawRotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	// Move forward
	AddMovementInput(ForwardDirection, MovementVector.X);

	// Move right
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AARCharacter::Look(const FInputActionInstance& Instance)
{
	const FVector2D LookVector = Instance.GetValue().Get<FVector2D>();

	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(-LookVector.Y);
}

void AARCharacter::SprintStart()
{
	UE_LOG(LogTemp, Display, TEXT("Sprinting start"));
	ActionComp->StartActionByName(this,"Sprint");
}

void AARCharacter::SprintEnd()
{
	UE_LOG(LogTemp, Display, TEXT("Sprinting end"));
	ActionComp->StopActionByName(this,"Sprint");
}

void AARCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

void AARCharacter::SecondaryAttack()
{
	ActionComp->StartActionByName(this, "SecondaryAttack");
}

void AARCharacter::DashAttack()
{
	ActionComp->StartActionByName(this, "DashAttack");
}

void AARCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();	
	}
}

FVector AARCharacter::PerformLineTraceFromCamera() const
{
	FHitResult HitResult;

	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + Camera->GetComponentRotation().Vector() * 1000.f;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, ObjectQueryParams);

	if (bBlockingHit)
	{
		return HitResult.ImpactPoint;
	}

	return End;
}

void AARCharacter::OnHealthChanged(AActor* InstigatorActor, UARAttributeComponent* OwningComp, float NewHealth,
	float DeltaHealth)
{
	if (DeltaHealth < 0.f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials("TimeOfHit", GetWorld()->TimeSeconds);

		// 100% of damage received added to Rage attribute
		float RageToAdd = DeltaHealth * -1;
		OwningComp->ApplyRageChange(RageToAdd);
	}
	
	if (DeltaHealth < 0.f && NewHealth <= 0.f)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		DisableInput(PlayerController);

		SetLifeSpan(5.f);
	}
}

void AARCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &AARCharacter::OnHealthChanged);
}

FVector AARCharacter::GetPawnViewLocation() const
{
	return Camera->GetComponentLocation();
}

// Called to bind functionality to input
void AARCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetNetOwningPlayer()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (!CurrentMappingContext.IsNull())
			{
				InputSystem->AddMappingContext(CurrentMappingContext.LoadSynchronous(), 1);
			}
		}
	}

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent)
	{
		// Move
		EnhancedInputComponent->BindAction(MoveAction.Get(), ETriggerEvent::Triggered, this, &AARCharacter::Move);

		// Look
		EnhancedInputComponent->BindAction(LookAction.Get(), ETriggerEvent::Triggered, this, &AARCharacter::Look);

		// Sprinting
		EnhancedInputComponent->BindAction(SprintAction.Get(), ETriggerEvent::Started, this, &AARCharacter::SprintStart);
		EnhancedInputComponent->BindAction(SprintAction.Get(), ETriggerEvent::Completed, this, &AARCharacter::SprintEnd);

		// Primary attack
		EnhancedInputComponent->BindAction(PrimaryAttackAction.Get(), ETriggerEvent::Started, this, &AARCharacter::PrimaryAttack);

		// Secondary attack
		EnhancedInputComponent->BindAction(SecondaryAttackAction.Get(), ETriggerEvent::Started, this, &AARCharacter::SecondaryAttack);

		// Dash attack
		EnhancedInputComponent->BindAction(DashAttackAction.Get(), ETriggerEvent::Started, this, &AARCharacter::DashAttack);

		// Jump
		EnhancedInputComponent->BindAction(JumpAction.Get(), ETriggerEvent::Started, this, &ACharacter::Jump);

		// Primary interact
		EnhancedInputComponent->BindAction(PrimaryInteractAction.Get(), ETriggerEvent::Started, this, &AARCharacter::PrimaryInteract); 
	}

}

void AARCharacter::HealSelf(float Amount)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}

