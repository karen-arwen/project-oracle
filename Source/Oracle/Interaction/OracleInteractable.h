// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OracleInteractable.generated.h"

class AOracleCharacter;

UINTERFACE(BlueprintType)
class UOracleInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Contrato de tudo que o jogador pode usar com [E]:
 * coletáveis, canteiros, portas, NPCs, baús...
 * BlueprintNativeEvent: implementável tanto em C++ quanto em Blueprint.
 * Chamadas sempre via IOracleInteractable::Execute_*().
 */
class ORACLE_API IOracleInteractable
{
	GENERATED_BODY()

public:
	/** Texto do prompt, ex.: "Coletar Fruta". */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Oracle|Interaction")
	FText GetInteractionText() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Oracle|Interaction")
	bool CanInteract(AOracleCharacter* Interactor) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Oracle|Interaction")
	void Interact(AOracleCharacter* Interactor);
};
