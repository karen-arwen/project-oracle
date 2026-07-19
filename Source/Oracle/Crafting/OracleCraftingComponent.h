// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OracleCraftingComponent.generated.h"

class UOracleRecipeDefinition;

/**
 * Craft do jogador. Receitas conhecidas ficam aqui (futuro: aprender
 * receitas explorando/comprando). Tecla [C] cria a primeira receita
 * possível com o que há na mochila — menu bonito virá com a UI CommonUI.
 */
UCLASS(ClassGroup = (Oracle), meta = (BlueprintSpawnableComponent))
class ORACLE_API UOracleCraftingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Oracle|Craft")
	void LearnRecipe(UOracleRecipeDefinition* Recipe);

	/** true se a mochila cobre todos os ingredientes. */
	UFUNCTION(BlueprintPure, Category = "Oracle|Craft")
	bool CanCraft(const UOracleRecipeDefinition* Recipe) const;

	/** Primeira receita conhecida que dá para criar agora (para HUD/tecla C). */
	UFUNCTION(BlueprintPure, Category = "Oracle|Craft")
	UOracleRecipeDefinition* GetFirstCraftable() const;

	/** Consome ingredientes e entrega o resultado. false se faltam itens. */
	UFUNCTION(BlueprintCallable, Category = "Oracle|Craft")
	bool Craft(UOracleRecipeDefinition* Recipe);

	/** Atalho da tecla [C]: cria a primeira receita possível. */
	UFUNCTION(BlueprintCallable, Category = "Oracle|Craft")
	void CraftBest();

	UFUNCTION(BlueprintPure, Category = "Oracle|Craft")
	const TArray<UOracleRecipeDefinition*>& GetKnownRecipes() const { return KnownRecipes; }

private:
	class UOracleInventoryComponent* GetInventory() const;

	UPROPERTY()
	TArray<TObjectPtr<UOracleRecipeDefinition>> KnownRecipes;
};
