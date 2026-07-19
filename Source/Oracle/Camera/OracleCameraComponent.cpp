// Project Oracle - Cozy Open World AAA

#include "Camera/OracleCameraComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

UOracleCameraComponent::UOracleCameraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UOracleCameraComponent::Initialize(USpringArmComponent* InSpringArm, UCameraComponent* InCamera)
{
	SpringArm = InSpringArm;
	Camera = InCamera;

	if (SpringArm.IsValid())
	{
		TargetArmLength = SpringArm->TargetArmLength;
		BaseSocketOffset = SpringArm->SocketOffset;
	}
	if (Camera.IsValid())
	{
		BaseFOV = Camera->FieldOfView;
	}
}

void UOracleCameraComponent::StartLandingDip(const float Intensity)
{
	DipIntensity = FMath::Clamp(Intensity, 0.f, 2.f);
	DipElapsed = 0.f;
}

void UOracleCameraComponent::AddZoom(const float Delta)
{
	TargetArmLength = FMath::Clamp(TargetArmLength - Delta * ZoomStep, MinArmLength, MaxArmLength);
}

void UOracleCameraComponent::TickComponent(const float DeltaTime, const ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Zoom suave.
	if (SpringArm.IsValid() && !FMath::IsNearlyEqual(SpringArm->TargetArmLength, TargetArmLength, 0.1f))
	{
		SpringArm->TargetArmLength =
			FMath::FInterpTo(SpringArm->TargetArmLength, TargetArmLength, DeltaTime, ZoomInterpSpeed);
	}

	// Dip de pouso: meia-onda senoidal — desce rápido, volta suave.
	if (DipElapsed >= 0.f && SpringArm.IsValid())
	{
		DipElapsed += DeltaTime;
		if (DipElapsed >= DipDuration)
		{
			DipElapsed = -1.f;
			SpringArm->SocketOffset = BaseSocketOffset;
		}
		else
		{
			const float Wave = FMath::Sin(PI * (DipElapsed / DipDuration));
			SpringArm->SocketOffset = BaseSocketOffset -
				FVector(0.f, 0.f, DipAmplitude * DipIntensity * Wave);
		}
	}

	// Kick de FOV: só quando o sprint está ativo E há velocidade real.
	if (Camera.IsValid())
	{
		const float Speed2D = GetOwner() ? GetOwner()->GetVelocity().Size2D() : 0.f;
		const bool bKick = bSprintFOVRequested && Speed2D >= SprintFOVMinSpeed;
		const float TargetFOV = BaseFOV + (bKick ? SprintFOVKick : 0.f);

		if (!FMath::IsNearlyEqual(Camera->FieldOfView, TargetFOV, 0.05f))
		{
			Camera->SetFieldOfView(
				FMath::FInterpTo(Camera->FieldOfView, TargetFOV, DeltaTime, FOVInterpSpeed));
		}
	}
}
