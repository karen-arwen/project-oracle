// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OracleFishingComponent.generated.h"

class UOracleFishDefinition;

UENUM(BlueprintType)
enum class EOracleFishingState : uint8
{
	Idle,     // sem pescar
	Casting,  // linha na água, esperando fisgada
	Biting    // PEIXE! janela para apertar Interagir
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOracleFishCaught, UOracleFishDefinition*, Fish);

/**
 * Minigame de pesca por timing (cozy, sem estresse):
 * 1) Interagir num pesqueiro → lança a linha (Casting).
 * 2) Após um tempo aleatório, "morde" (Biting) por uma janela curta.
 * 3) Interagir dentro da janela → fisga; fora → o peixe escapa (tenta de novo).
 *
 * O peixe é sorteado por peso/raridade da lista conhecida. A UI de barra
 * de tensão entra na v2; a base de gameplay e progressão já vive aqui.
 */
UCLASS(ClassGroup = (Oracle), meta = (BlueprintSpawnableComponent))
class ORACLE_API UOracleFishingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Define os peixes possíveis neste local (chamado pelo pesqueiro). */
	void SetFishPool(const TArray<UOracleFishDefinition*>& Pool);

	/** Alterna: começa a pescar, ou tenta fisgar se um peixe está mordendo. */
	UFUNCTION(BlueprintCallable, Category = "Oracle|Fishing")
	void ToggleFishing();

	UFUNCTION(BlueprintPure, Category = "Oracle|Fishing")
	EOracleFishingState GetState() const { return State; }

	UPROPERTY(BlueprintAssignable, Category = "Oracle|Fishing")
	FOracleFishCaught OnFishCaught;

protected:
	/** Janela (s) em que dá para fisgar após a mordida. Generosa = cozy. */
	UPROPERTY(EditDefaultsOnly, Category = "Oracle|Fishing", meta = (ClampMin = 0.3))
	float BiteWindow = 1.6f;

	UPROPERTY(EditDefaultsOnly, Category = "Oracle|Fishing", meta = (ClampMin = 0.5))
	float MinWaitTime = 2.f;

	UPROPERTY(EditDefaultsOnly, Category = "Oracle|Fishing", meta = (ClampMin = 1))
	float MaxWaitTime = 6.f;

private:
	void StartCast();
	void OnBite();
	void EndBite();
	void Reel();
	void Cancel();
	UOracleFishDefinition* RollFish() const;
	void Toast(const FText& Text, bool bGolden) const;

	UPROPERTY()
	TArray<TObjectPtr<UOracleFishDefinition>> FishPool;

	EOracleFishingState State = EOracleFishingState::Idle;
	FTimerHandle BiteTimer;
	FTimerHandle WindowTimer;
};
