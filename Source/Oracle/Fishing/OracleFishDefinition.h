// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OracleFishDefinition.generated.h"

class UOracleItemDefinition;

UENUM(BlueprintType)
enum class EOracleFishRarity : uint8
{
	Comum,
	Incomum,
	Raro,
	Lendario
};

/**
 * Definição de peixe (Data Asset). "Centenas de peixes" da visão são
 * assets de design. A raridade define peso do sorteio e dificuldade da
 * fisga; horário/clima preferidos entram como filtros na v2.
 */
UCLASS(BlueprintType)
class ORACLE_API UOracleFishDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fish")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fish")
	EOracleFishRarity Rarity = EOracleFishRarity::Comum;

	/** Peso relativo no sorteio (raros têm peso menor). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fish", meta = (ClampMin = 0.01))
	float CatchWeight = 1.f;

	/** Item que vai para a mochila/compêndio ao fisgar. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fish")
	TObjectPtr<UOracleItemDefinition> Item;

	/** XP de Pesca concedido. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fish", meta = (ClampMin = 1))
	int32 FishingXP = 12;
};
