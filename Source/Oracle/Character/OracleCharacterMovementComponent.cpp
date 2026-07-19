// Project Oracle - Cozy Open World AAA

#include "Character/OracleCharacterMovementComponent.h"

UOracleCharacterMovementComponent::UOracleCharacterMovementComponent()
{
	// Sensação cozy: aceleração e frenagem suaves, giro fluido.
	MaxWalkSpeed = RunSpeed;
	MaxAcceleration = 1200.f;
	BrakingDecelerationWalking = 1500.f;
	BrakingFrictionFactor = 1.f;
	GroundFriction = 6.f;

	// Rotação suave orientada ao movimento (o Character desliga o yaw do controller).
	bOrientRotationToMovement = true;
	RotationRate = FRotator(0.f, 420.f, 0.f);

	// Pulo com peso: gravidade acima de 1.0 encurta o tempo no ar e tira a
	// sensação de "astronauta" do padrão da engine. JumpZ compensa a altura.
	GravityScale = 1.5f;
	JumpZVelocity = 640.f;
	AirControl = 0.35f;
	BrakingDecelerationFalling = 300.f;
	FallingLateralFriction = 0.6f;

	// Qualidade de vida em mundo aberto: subir degraus/ladeiras sem engasgar.
	MaxStepHeight = 45.f;
	SetWalkableFloorAngle(46.f);
	NavAgentProps.bCanCrouch = true;
}

void UOracleCharacterMovementComponent::SetGait(const EOracleGait NewGait)
{
	CurrentGait = NewGait;

	switch (CurrentGait)
	{
	case EOracleGait::Walk:   MaxWalkSpeed = WalkSpeed;   break;
	case EOracleGait::Run:    MaxWalkSpeed = RunSpeed;    break;
	case EOracleGait::Sprint: MaxWalkSpeed = SprintSpeed; break;
	}
}
