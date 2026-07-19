// Project Oracle - Cozy Open World AAA

#include "Character/OracleCharacter.h"

#include "Animation/AnimInstance.h"
#include "Building/OraclePlacementComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Camera/OracleCameraComponent.h"
#include "Character/OracleCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Core/OraclePlayerController.h"
#include "Core/OracleSaveSubsystem.h"
#include "EnhancedInputComponent.h"
#include "Engine/GameInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "Input/OracleInputConfig.h"
#include "Interaction/OracleInteractionComponent.h"
#include "Inventory/OracleInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Oracle.h"
#include "UObject/ConstructorHelpers.h"

AOracleCharacter::AOracleCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UOracleCharacterMovementComponent>(
		  ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(35.f, 90.f);

	// A câmera controla o yaw; o personagem gira em direção ao movimento (no CMC).
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// Pulo variável: segurar Espaço prolonga o impulso.
	JumpMaxHoldTime = 0.18f;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 450.f;
	SpringArm->SocketOffset = FVector(0.f, 0.f, 60.f);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 12.f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 20.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	CameraController = CreateDefaultSubobject<UOracleCameraComponent>(TEXT("CameraController"));
	Inventory = CreateDefaultSubobject<UOracleInventoryComponent>(TEXT("Inventory"));
	Interaction = CreateDefaultSubobject<UOracleInteractionComponent>(TEXT("Interaction"));
	Placement = CreateDefaultSubobject<UOraclePlacementComponent>(TEXT("Placement"));

	// Visual padrão: mannequin + animações do pacote Third Person (se presente).
	// Null-safe: sem o pacote, o personagem volta a ser cápsula, sem crash.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> QuinnMesh(
		TEXT("/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> UnarmedABP(
		TEXT("/Game/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed"));

	if (QuinnMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(QuinnMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, -90.f, 0.f));
	}
	if (UnarmedABP.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(UnarmedABP.Class);
	}
}

void AOracleCharacter::BeginPlay()
{
	Super::BeginPlay();
	CameraController->Initialize(SpringArm, Camera);
}

void AOracleCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	const UOracleCharacterMovementComponent* Move = GetOracleMovement();
	if (Move->IsMovingOnGround())
	{
		LastGroundedTime = GetWorld()->GetTimeSeconds();
	}
	else if (Move->IsFalling())
	{
		// Guarda o pico da queda para escalar o feedback do pouso.
		PeakFallSpeed = FMath::Max(PeakFallSpeed, -Move->Velocity.Z);
	}
}

UOracleCharacterMovementComponent* AOracleCharacter::GetOracleMovement() const
{
	return CastChecked<UOracleCharacterMovementComponent>(GetCharacterMovement());
}

bool AOracleCharacter::CanJumpInternal_Implementation() const
{
	if (Super::CanJumpInternal_Implementation())
	{
		return true;
	}

	// Coyote time: pouco depois de sair da borda, caindo, ainda vale pular.
	// (JumpCurrentCount <= 1 porque a engine pré-incrementa ao pular no ar.)
	const bool bWithinCoyote =
		GetCharacterMovement()->IsFalling() &&
		(GetWorld()->GetTimeSeconds() - LastGroundedTime) <= CoyoteTime &&
		GetVelocity().Z <= 0.f &&
		JumpCurrentCount <= 1;

	return bWithinCoyote;
}

void AOracleCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	// Jump buffer: pulo apertado no ar dispara agora.
	if (GetWorld()->GetTimeSeconds() - JumpPressedTime <= JumpBufferTime)
	{
		JumpPressedTime = -1000.f;
		Jump();
	}

	// Feedback proporcional à queda.
	if (PeakFallSpeed >= MinLandFeedbackFallSpeed)
	{
		const float Intensity = FMath::GetMappedRangeValueClamped(
			FVector2D(MinLandFeedbackFallSpeed, MaxLandFeedbackFallSpeed),
			FVector2D(0.35f, 1.5f), PeakFallSpeed);

		// Feedback padrão: dip da câmera (código nosso, sempre funciona).
		CameraController->StartLandingDip(Intensity);

		// Shake extra opcional (asset criado no editor, se atribuído).
		if (LandShake)
		{
			if (APlayerController* PC = GetController<APlayerController>())
			{
				PC->ClientStartCameraShake(LandShake, Intensity);
			}
		}
		if (LandSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, LandSound, GetActorLocation(),
				FMath::Min(Intensity, 1.f));
		}
		if (LandDustVFX)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, LandDustVFX,
				Hit.ImpactPoint, FRotator::ZeroRotator,
				FVector(FMath::Min(Intensity, 1.2f)));
		}
	}
	PeakFallSpeed = 0.f;
}

void AOracleCharacter::PlayFootstep()
{
	if (FootstepSound && GetOracleMovement()->IsMovingOnGround())
	{
		UGameplayStatics::PlaySoundAtLocation(this, FootstepSound, GetActorLocation(), 0.6f);
	}
}

void AOracleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	AOraclePlayerController* PC = GetController<AOraclePlayerController>();
	const UOracleInputConfig* Config = PC ? PC->GetInputConfig() : nullptr;
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (!Config || !Input)
	{
		UE_LOG(LogOracle, Error, TEXT("Input nao configurado (Config=%d, EnhancedInput=%d)."),
			Config != nullptr, Input != nullptr);
		return;
	}

	Input->BindAction(Config->Move,       ETriggerEvent::Triggered, this, &AOracleCharacter::Input_Move);
	Input->BindAction(Config->Look,       ETriggerEvent::Triggered, this, &AOracleCharacter::Input_Look);
	Input->BindAction(Config->ZoomCamera, ETriggerEvent::Triggered, this, &AOracleCharacter::Input_Zoom);
	Input->BindAction(Config->Jump,       ETriggerEvent::Started,   this, &AOracleCharacter::Input_JumpPressed);
	Input->BindAction(Config->Jump,       ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	Input->BindAction(Config->Sprint,     ETriggerEvent::Started,   this, &AOracleCharacter::Input_SprintStart);
	Input->BindAction(Config->Sprint,     ETriggerEvent::Completed, this, &AOracleCharacter::Input_SprintEnd);
	Input->BindAction(Config->WalkToggle, ETriggerEvent::Started,   this, &AOracleCharacter::Input_WalkToggle);
	Input->BindAction(Config->Interact,    ETriggerEvent::Started, this, &AOracleCharacter::Input_Interact);
	Input->BindAction(Config->BuildToggle, ETriggerEvent::Started, this, &AOracleCharacter::Input_BuildToggle);
	Input->BindAction(Config->Place,       ETriggerEvent::Started, this, &AOracleCharacter::Input_Place);
	Input->BindAction(Config->RotateProp,  ETriggerEvent::Started, this, &AOracleCharacter::Input_RotateProp);
	Input->BindAction(Config->QuickSave,   ETriggerEvent::Started, this, &AOracleCharacter::Input_QuickSave);
	Input->BindAction(Config->QuickLoad,   ETriggerEvent::Started, this, &AOracleCharacter::Input_QuickLoad);
}

void AOracleCharacter::Input_Interact(const FInputActionValue&)
{
	Interaction->TryInteract();
}

void AOracleCharacter::Input_BuildToggle(const FInputActionValue&)
{
	Placement->ToggleBuildMode();
}

void AOracleCharacter::Input_Place(const FInputActionValue&)
{
	Placement->ConfirmPlacement();
}

void AOracleCharacter::Input_RotateProp(const FInputActionValue&)
{
	Placement->RotateGhost();
}

void AOracleCharacter::Input_QuickSave(const FInputActionValue&)
{
	if (UOracleSaveSubsystem* SaveSys = GetGameInstance()->GetSubsystem<UOracleSaveSubsystem>())
	{
		SaveSys->SaveWorld();
	}
}

void AOracleCharacter::Input_QuickLoad(const FInputActionValue&)
{
	if (UOracleSaveSubsystem* SaveSys = GetGameInstance()->GetSubsystem<UOracleSaveSubsystem>())
	{
		SaveSys->LoadWorld();
	}
}

void AOracleCharacter::Input_Move(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	if (!Controller || Axis.IsNearlyZero())
	{
		return;
	}

	// Movimento em 8 direções relativo à câmera (só o yaw importa).
	const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Forward, Axis.Y);
	AddMovementInput(Right, Axis.X);
}

void AOracleCharacter::Input_Look(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	AddControllerYawInput(Axis.X);
	AddControllerPitchInput(Axis.Y);
}

void AOracleCharacter::Input_Zoom(const FInputActionValue& Value)
{
	CameraController->AddZoom(Value.Get<float>());
}

void AOracleCharacter::Input_JumpPressed(const FInputActionValue&)
{
	JumpPressedTime = GetWorld()->GetTimeSeconds();
	Jump();
}

void AOracleCharacter::Input_SprintStart(const FInputActionValue&)
{
	GetOracleMovement()->SetGait(EOracleGait::Sprint);
	CameraController->SetSprintFOVActive(true);
}

void AOracleCharacter::Input_SprintEnd(const FInputActionValue&)
{
	GetOracleMovement()->SetGait(bWalkLocked ? EOracleGait::Walk : EOracleGait::Run);
	CameraController->SetSprintFOVActive(false);
}

void AOracleCharacter::Input_WalkToggle(const FInputActionValue&)
{
	bWalkLocked = !bWalkLocked;
	GetOracleMovement()->SetGait(bWalkLocked ? EOracleGait::Walk : EOracleGait::Run);
}
