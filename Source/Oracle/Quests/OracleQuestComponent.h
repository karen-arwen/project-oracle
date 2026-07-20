// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OracleQuestComponent.generated.h"

class UOracleQuestDefinition;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOracleQuestCompleted, UOracleQuestDefinition*, Quest);

/**
 * Diário de missões do jogador. v1: uma missão de coleta ativa por vez
 * (o compêndio de missões com várias simultâneas vem com a UI). Observa
 * o inventário e conclui automaticamente quando o objetivo é atingido,
 * pagando a recompensa. Garante que o jogador SEMPRE tenha um objetivo.
 */
UCLASS(ClassGroup = (Oracle), meta = (BlueprintSpawnableComponent))
class ORACLE_API UOracleQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Oracle|Quest")
	void SetActiveQuest(UOracleQuestDefinition* Quest);

	/** Fila de missões a oferecer em sequência (mantém o loop vivo). */
	UFUNCTION(BlueprintCallable, Category = "Oracle|Quest")
	void QueueQuest(UOracleQuestDefinition* Quest);

	UFUNCTION(BlueprintPure, Category = "Oracle|Quest")
	UOracleQuestDefinition* GetActiveQuest() const { return ActiveQuest; }

	/** Progresso atual do objetivo (para o HUD). */
	UFUNCTION(BlueprintPure, Category = "Oracle|Quest")
	int32 GetProgress() const;

	UFUNCTION(BlueprintPure, Category = "Oracle|Quest")
	FText GetObjectiveText() const;

	UPROPERTY(BlueprintAssignable, Category = "Oracle|Quest")
	FOracleQuestCompleted OnQuestCompleted;

protected:
	UFUNCTION()
	void HandleInventoryChanged();

private:
	void TryComplete();
	void AdvanceQueue();

	UPROPERTY()
	TObjectPtr<UOracleQuestDefinition> ActiveQuest;

	UPROPERTY()
	TArray<TObjectPtr<UOracleQuestDefinition>> Queue;
};
