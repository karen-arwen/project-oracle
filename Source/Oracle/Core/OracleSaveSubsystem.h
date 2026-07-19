// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OracleSaveSubsystem.generated.h"

class UOracleCropDefinition;
class UOracleItemDefinition;

/**
 * Salvar/carregar o estado do mundo (slot único por enquanto).
 * Coleta: jogador (posição, inventário), tempo, canteiros e móveis.
 * V2 (futuro): múltiplos slots, autosave, migração de versão, async.
 */
UCLASS()
class ORACLE_API UOracleSaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Oracle|Save")
	bool SaveWorld();

	UFUNCTION(BlueprintCallable, Category = "Oracle|Save")
	bool LoadWorld();

	/** Resolve definição por nome (Data Assets carregados + conteúdo demo). */
	static UOracleItemDefinition* FindItemByName(const FString& Name);
	static UOracleCropDefinition* FindCropByName(const FString& Name);

private:
	static const TCHAR* SlotName;
};
