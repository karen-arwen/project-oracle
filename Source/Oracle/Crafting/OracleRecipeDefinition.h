// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OracleRecipeDefinition.generated.h"

class UOracleItemDefinition;

USTRUCT(BlueprintType)
struct FOracleIngredient
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recipe")
	TObjectPtr<UOracleItemDefinition> Item;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recipe", meta = (ClampMin = 1))
	int32 Count = 1;
};

/**
 * Receita de craft/culinária como Data Asset.
 * Centenas de receitas futuras (culinária, artesanato, máquinas) são
 * assets criados por design — zero código novo por receita.
 */
UCLASS(BlueprintType)
class ORACLE_API UOracleRecipeDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recipe")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recipe")
	TArray<FOracleIngredient> Ingredients;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recipe")
	TObjectPtr<UOracleItemDefinition> Result;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recipe", meta = (ClampMin = 1))
	int32 ResultCount = 1;
};
