// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OracleInteractionComponent.generated.h"

class AOracleCharacter;

/**
 * Detecta o interagível mais próximo do personagem (busca por overlap
 * em intervalo fixo — mais barato e estável que trace por frame) e
 * executa a interação no [E]. O HUD lê GetFocusedActor() para o prompt.
 */
UCLASS(ClassGroup = (Oracle), meta = (BlueprintSpawnableComponent))
class ORACLE_API UOracleInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UOracleInteractionComponent();

	virtual void BeginPlay() override;

	/** Interage com o alvo em foco, se houver. */
	UFUNCTION(BlueprintCallable, Category = "Oracle|Interaction")
	void TryInteract();

	UFUNCTION(BlueprintPure, Category = "Oracle|Interaction")
	AActor* GetFocusedActor() const { return FocusedActor.Get(); }

	/** Texto de prompt do alvo em foco (vazio se nenhum). */
	UFUNCTION(BlueprintPure, Category = "Oracle|Interaction")
	FText GetFocusedText() const;

protected:
	/** Raio de busca ao redor do personagem (cm). */
	UPROPERTY(EditDefaultsOnly, Category = "Oracle|Interaction", meta = (ClampMin = 50))
	float SearchRadius = 220.f;

	/** Frequência da busca (s). */
	UPROPERTY(EditDefaultsOnly, Category = "Oracle|Interaction", meta = (ClampMin = 0.02))
	float SearchInterval = 0.1f;

private:
	void UpdateFocus();

	TWeakObjectPtr<AActor> FocusedActor;
	FTimerHandle SearchTimer;
};
