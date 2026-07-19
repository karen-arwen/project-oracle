// Project Oracle - Cozy Open World AAA

#include "Animation/OracleAnimInstance.h"
#include "Character/OracleCharacter.h"

void UOracleAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<AOracleCharacter>(TryGetPawnOwner());
	if (Character.IsValid())
	{
		Movement = Character->GetOracleMovement();
	}
}

void UOracleAnimInstance::NativeUpdateAnimation(const float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// Re-tenta o cache: na possessão, o pawn pode não existir no Initialize.
	if (!Character.IsValid())
	{
		Character = Cast<AOracleCharacter>(TryGetPawnOwner());
		Movement = Character.IsValid() ? Character->GetOracleMovement() : nullptr;
	}
	if (!Character.IsValid() || !Movement.IsValid())
	{
		return;
	}

	const FVector Velocity = Character->GetVelocity();
	GroundSpeed = Velocity.Size2D();
	bShouldMove = GroundSpeed > 3.f && !Movement->GetCurrentAcceleration().IsNearlyZero();
	bIsFalling = Movement->IsFalling();
	Gait = Movement->GetGait();
}
