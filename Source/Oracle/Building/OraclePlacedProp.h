// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/OracleInteractable.h"
#include "OraclePlacedProp.generated.h"

class UStaticMeshComponent;
class UOracleItemDefinition;

/**
 * Móvel/objeto posicionado pelo jogador no modo decoração.
 * Interagir [E] devolve o item ao inventário (guardar).
 */
UCLASS()
class ORACLE_API AOraclePlacedProp : public AActor, public IOracleInteractable
{
	GENERATED_BODY()

public:
	AOraclePlacedProp();

	/** Aplica malha/escala do item. Chamar logo após o spawn. */
	void InitFromItem(UOracleItemDefinition* Item);

	UOracleItemDefinition* GetSourceItem() const { return SourceItem; }

	// IOracleInteractable
	virtual FText GetInteractionText_Implementation() const override;
	virtual bool CanInteract_Implementation(AOracleCharacter* Interactor) const override;
	virtual void Interact_Implementation(AOracleCharacter* Interactor) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Oracle")
	TObjectPtr<UStaticMeshComponent> Mesh;

private:
	UPROPERTY()
	TObjectPtr<UOracleItemDefinition> SourceItem;
};
