// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OracleQuestDefinition.generated.h"

class UOracleItemDefinition;

/**
 * Missão como Data Asset. v1 cobre o objetivo mais comum de cozy game:
 * "traga N de um item". Novos tipos (falar com NPC, alcançar nível,
 * pescar X) entram como enum + lógica no QuestComponent, sem quebrar
 * as missões existentes.
 */
UCLASS(BlueprintType)
class ORACLE_API UOracleQuestDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	FText Title;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest", meta = (MultiLine = true))
	FText Description;

	/** Item que precisa ser entregue. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Objective")
	TObjectPtr<UOracleItemDefinition> TargetItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Objective", meta = (ClampMin = 1))
	int32 TargetCount = 5;

	/** Se true, os itens entregues são consumidos ao concluir. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Objective")
	bool bConsumeItems = true;

	// --- Recompensa ---
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Reward", meta = (ClampMin = 0))
	int32 RewardCoins = 50;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Reward")
	TObjectPtr<UOracleItemDefinition> RewardItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Reward", meta = (ClampMin = 0))
	int32 RewardItemCount = 0;
};
