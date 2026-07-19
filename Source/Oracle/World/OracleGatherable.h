// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/OracleInteractable.h"
#include "OracleGatherable.generated.h"

class UNiagaraSystem;
class USoundBase;
class UStaticMeshComponent;
class UOracleItemDefinition;

/**
 * Recurso coletável do mundo: arbusto de frutas, tronco, pedra, flor...
 * Interagiu → dá itens, some, e reaparece após o tempo de respawn.
 * Feedback (som/partícula) é null-safe: sem asset, sem efeito.
 */
UCLASS()
class ORACLE_API AOracleGatherable : public AActor, public IOracleInteractable
{
	GENERATED_BODY()

public:
	AOracleGatherable();

	// IOracleInteractable
	virtual FText GetInteractionText_Implementation() const override;
	virtual bool CanInteract_Implementation(AOracleCharacter* Interactor) const override;
	virtual void Interact_Implementation(AOracleCharacter* Interactor) override;

	/** Configuração via código (usada pelo mundo demo). */
	void Configure(UOracleItemDefinition* InItem, int32 InAmount, float InRespawnSeconds);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Oracle")
	TObjectPtr<UStaticMeshComponent> Mesh;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Oracle|Gather")
	TObjectPtr<UOracleItemDefinition> ItemToGive;

	UPROPERTY(EditAnywhere, Category = "Oracle|Gather", meta = (ClampMin = 1))
	int32 Amount = 1;

	/** 0 = não reaparece. */
	UPROPERTY(EditAnywhere, Category = "Oracle|Gather", meta = (ClampMin = 0))
	float RespawnSeconds = 30.f;

	UPROPERTY(EditAnywhere, Category = "Oracle|Feedback")
	TObjectPtr<USoundBase> GatherSound;

	UPROPERTY(EditAnywhere, Category = "Oracle|Feedback")
	TObjectPtr<UNiagaraSystem> GatherVFX;

private:
	void SetGathered(bool bGathered);
	void Respawn();

	bool bIsGathered = false;
	FTimerHandle RespawnTimer;
};
