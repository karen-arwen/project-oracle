// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OracleItemDefinition.generated.h"

class UStaticMesh;
class UOracleCropDefinition;

UENUM(BlueprintType)
enum class EOracleItemCategory : uint8
{
	Resource,   // madeira, pedra...
	Food,       // frutas, pratos
	Seed,       // plantável em canteiros
	Furniture,  // posicionável no modo decoração
	Tool
};

/**
 * Definição de item como Data Asset — a "carta" do item.
 * Instâncias no inventário são apenas {definição, quantidade}.
 * Novos itens = novos assets, zero código (pilar de escalabilidade:
 * centenas de itens virão de design, não de programadores).
 */
UCLASS(BlueprintType)
class ORACLE_API UOracleItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (MultiLine = true))
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	EOracleItemCategory Category = EOracleItemCategory::Resource;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (ClampMin = 1))
	int32 MaxStack = 99;

	/** Valor base para a futura economia. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (ClampMin = 0))
	int32 BaseValue = 1;

	/** Malha usada no mundo (decoração, drops). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "World")
	TSoftObjectPtr<UStaticMesh> WorldMesh;

	/** Escala da malha ao ser posicionada no mundo. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "World")
	FVector WorldMeshScale = FVector::OneVector;

	/** Somente sementes: qual cultivo este item planta. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Farming",
		meta = (EditCondition = "Category == EOracleItemCategory::Seed"))
	TObjectPtr<UOracleCropDefinition> Crop;
};
