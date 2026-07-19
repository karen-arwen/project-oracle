// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OracleGameMode.generated.h"

/**
 * GameMode base do projeto.
 * Define as classes padrão do Gameplay Framework (Pawn, PlayerController).
 * Sistemas futuros (Save, Quest, Time-of-Day) serão orquestrados por
 * Subsystems próprios — o GameMode permanece enxuto de propósito.
 */
UCLASS()
class ORACLE_API AOracleGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOracleGameMode();

	virtual void BeginPlay() override;

protected:
	/**
	 * Spawna o mundo demo (itens, arbustos, canteiros) se nenhum existir
	 * no mapa. Desligue quando o conteúdo real (Data Assets) existir.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Oracle|Demo")
	bool bSpawnDemoContent = true;
};
