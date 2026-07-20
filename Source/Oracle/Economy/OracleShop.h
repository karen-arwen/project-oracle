// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/OracleInteractable.h"
#include "OracleShop.generated.h"

class UStaticMeshComponent;

/**
 * Banca de vendas da vila. Interagir [E] vende TODOS os itens vendáveis
 * da mochila pelo BaseValue de cada um, creditando Folhas na carteira.
 * v2: UI de loja com compra, preços por raridade, estoque diário.
 */
UCLASS()
class ORACLE_API AOracleShop : public AActor, public IOracleInteractable
{
	GENERATED_BODY()

public:
	AOracleShop();

	virtual FText GetInteractionText_Implementation() const override;
	virtual bool CanInteract_Implementation(AOracleCharacter* Interactor) const override;
	virtual void Interact_Implementation(AOracleCharacter* Interactor) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Oracle")
	TObjectPtr<UStaticMeshComponent> Mesh;

protected:
	/** Fração do BaseValue paga na venda (resto é a margem da loja). */
	UPROPERTY(EditAnywhere, Category = "Oracle|Shop", meta = (ClampMin = 0.1, ClampMax = 1))
	float SellRate = 0.8f;
};
