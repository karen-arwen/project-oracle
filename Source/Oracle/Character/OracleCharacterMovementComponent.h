// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "OracleCharacterMovementComponent.generated.h"

/** Marcha atual da locomoção terrestre. */
UENUM(BlueprintType)
enum class EOracleGait : uint8
{
	Walk,
	Run,
	Sprint
};

/**
 * Modos de movimento customizados futuros.
 * Usados com MOVE_Custom no CharacterMovementComponent.
 * (Natação usa o MOVE_Swimming nativo da engine.)
 */
UENUM(BlueprintType)
enum class EOracleCustomMovement : uint8
{
	None    = 0,
	Climb   = 1,  // Escalada estilo BOTW
	Glide   = 2,  // Planador
	Mount   = 3,  // Montarias
	Vehicle = 4   // Veículos (provavelmente migrará para Pawn próprio)
};

/**
 * Movimento do personagem.
 * Centraliza velocidades por marcha e será o lar dos modos customizados
 * (escalada, planador). Manter locomoção AQUI — nunca no Character —
 * garante replicação de rede correta e um único ponto de tuning.
 */
UCLASS()
class ORACLE_API UOracleCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UOracleCharacterMovementComponent();

	/** Define a marcha e aplica a velocidade correspondente. */
	UFUNCTION(BlueprintCallable, Category = "Oracle|Movement")
	void SetGait(EOracleGait NewGait);

	UFUNCTION(BlueprintPure, Category = "Oracle|Movement")
	EOracleGait GetGait() const { return CurrentGait; }

protected:
	/** Velocidade andando (cm/s). Ritmo contemplativo. */
	UPROPERTY(EditDefaultsOnly, Category = "Oracle|Gait", meta = (ClampMin = 0))
	float WalkSpeed = 200.f;

	/** Velocidade padrão de deslocamento (cm/s). */
	UPROPERTY(EditDefaultsOnly, Category = "Oracle|Gait", meta = (ClampMin = 0))
	float RunSpeed = 500.f;

	/** Velocidade de sprint (cm/s). */
	UPROPERTY(EditDefaultsOnly, Category = "Oracle|Gait", meta = (ClampMin = 0))
	float SprintSpeed = 750.f;

private:
	EOracleGait CurrentGait = EOracleGait::Run;
};
