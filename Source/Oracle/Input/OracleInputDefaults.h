// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"

class UInputMappingContext;
class UOracleInputConfig;

/**
 * Fábrica do input padrão em runtime.
 *
 * Por quê: permite compilar e JOGAR imediatamente, sem criar nenhum asset
 * no editor. Quando os Data Assets forem criados no editor (workflow
 * definitivo, amigável a design), eles substituem isto automaticamente —
 * basta preenchê-los no OraclePlayerController.
 *
 * Teclado padrão: WASD mover, mouse olhar, scroll zoom, Espaço pular,
 * Shift sprint, Ctrl alterna andar/correr.
 */
struct FOracleInputDefaults
{
	/** Cria InputConfig + MappingContext padrão. Outer define o dono dos objetos. */
	static void Build(UObject* Outer, TObjectPtr<UOracleInputConfig>& OutConfig,
	                  TObjectPtr<UInputMappingContext>& OutContext);
};
