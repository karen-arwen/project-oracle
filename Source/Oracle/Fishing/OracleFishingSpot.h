// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/OracleInteractable.h"
#include "OracleFishingSpot.generated.h"

class UStaticMeshComponent;
class UOracleFishDefinition;

/**
 * Pesqueiro: um ponto de água onde se pesca. Interagir passa a lista de
 * peixes locais ao FishingComponent do jogador e alterna o minigame.
 * Cada bioma terá seu pool (lago, mar, caverna) — só trocar o array.
 */
UCLASS()
class ORACLE_API AOracleFishingSpot : public AActor, public IOracleInteractable
{
	GENERATED_BODY()

public:
	AOracleFishingSpot();

	virtual FText GetInteractionText_Implementation() const override;
	virtual bool CanInteract_Implementation(AOracleCharacter* Interactor) const override;
	virtual void Interact_Implementation(AOracleCharacter* Interactor) override;

	void SetFishPool(const TArray<UOracleFishDefinition*>& InPool) { FishPool = InPool; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Oracle")
	TObjectPtr<UStaticMeshComponent> Marker;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Oracle|Fishing")
	TArray<TObjectPtr<UOracleFishDefinition>> FishPool;
};
