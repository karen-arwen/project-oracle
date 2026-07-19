// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Character/OracleCharacterMovementComponent.h"
#include "OracleAnimInstance.generated.h"

class AOracleCharacter;

/**
 * Base C++ do Animation Blueprint do personagem.
 * Calcula em código tudo que o ABP precisa — no editor, o ABP só liga
 * estas variáveis aos nós (Blend Space, State Machine), sem lógica.
 * Isso mantém o grafo limpo e o cálculo testável/performático.
 */
UCLASS()
class ORACLE_API UOracleAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	/** Velocidade 2D atual (cm/s). Eixo do Blend Space de locomoção. */
	UPROPERTY(BlueprintReadOnly, Category = "Oracle|Locomotion")
	float GroundSpeed = 0.f;

	/** true quando há velocidade E intenção (aceleração) — evita "moonwalk". */
	UPROPERTY(BlueprintReadOnly, Category = "Oracle|Locomotion")
	bool bShouldMove = false;

	UPROPERTY(BlueprintReadOnly, Category = "Oracle|Locomotion")
	bool bIsFalling = false;

	UPROPERTY(BlueprintReadOnly, Category = "Oracle|Locomotion")
	EOracleGait Gait = EOracleGait::Run;

private:
	TWeakObjectPtr<AOracleCharacter> Character;
	TWeakObjectPtr<UOracleCharacterMovementComponent> Movement;
};
