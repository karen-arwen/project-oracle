// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShakeBase.h"
#include "OracleLandingShake.generated.h"

/**
 * Placeholder para um shake de pouso feito como asset no editor
 * (Blueprint filho desta classe + padrão Perlin configurado lá).
 * O feedback de pouso padrão do jogo é o "dip" da câmera, implementado
 * no UOracleCameraComponent — sem dependência de classes que mudam
 * de módulo entre versões da engine.
 */
UCLASS()
class ORACLE_API UOracleLandingShake : public UCameraShakeBase
{
	GENERATED_BODY()
};
