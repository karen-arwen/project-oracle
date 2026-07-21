// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OracleDemoWorld.generated.h"

class UOracleCropDefinition;
class UOracleItemDefinition;
class UOracleRecipeDefinition;
class UOracleQuestDefinition;

/**
 * Mundo demo autogerado: cria itens/cultivo de teste em runtime e povoa
 * o mapa com arbustos coletáveis, troncos e canteiros — o loop completo
 * (coletar → plantar → regar → colher → decorar → salvar) jogável sem
 * NENHUM asset. O GameMode o spawna automaticamente.
 *
 * Quando os Data Assets reais existirem no editor, desligue
 * bSpawnDemoContent no GameMode. Este ator é DESCARTÁVEL por design.
 */
UCLASS()
class ORACLE_API AOracleDemoWorld : public AActor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

private:
	void CreateDemoDefinitions();
	void SpawnWorldContent();
	void GiveStartingItems();

	// Mantidos vivos por UPROPERTY (criados em runtime).
	UPROPERTY() TObjectPtr<UOracleItemDefinition> BerryItem;
	UPROPERTY() TObjectPtr<UOracleItemDefinition> SeedItem;
	UPROPERTY() TObjectPtr<UOracleItemDefinition> WoodItem;
	UPROPERTY() TObjectPtr<UOracleItemDefinition> ChairItem;
	UPROPERTY() TObjectPtr<UOracleItemDefinition> LampItem;
	UPROPERTY() TObjectPtr<UOracleItemDefinition> PieItem;
	UPROPERTY() TObjectPtr<UOracleCropDefinition> BerryCrop;
	UPROPERTY() TObjectPtr<UOracleRecipeDefinition> ChairRecipe;
	UPROPERTY() TObjectPtr<UOracleRecipeDefinition> PieRecipe;
	UPROPERTY() TObjectPtr<UOracleQuestDefinition> QuestWood;
	UPROPERTY() TObjectPtr<UOracleQuestDefinition> QuestBerries;
	UPROPERTY() TObjectPtr<UOracleItemDefinition> FishCommonItem;
	UPROPERTY() TObjectPtr<UOracleItemDefinition> FishRareItem;
	UPROPERTY() TArray<TObjectPtr<class UOracleFishDefinition>> DemoFish;
};
