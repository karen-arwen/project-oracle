// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OracleCropDefinition.generated.h"

class UOracleItemDefinition;

/**
 * Definição de cultivo (Data Asset): o que uma semente vira.
 * Novos cultivos = novos assets, sem código.
 */
UCLASS(BlueprintType)
class ORACLE_API UOracleCropDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop")
	FText DisplayName;

	/** Dias (regados) até a colheita. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop", meta = (ClampMin = 1))
	int32 GrowthDays = 3;

	/** Item entregue na colheita. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop")
	TObjectPtr<UOracleItemDefinition> Produce;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crop", meta = (ClampMin = 1))
	int32 ProduceAmount = 2;
};
