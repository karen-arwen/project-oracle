// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/OracleInteractable.h"
#include "OracleFarmPlot.generated.h"

class UStaticMeshComponent;
class UOracleCropDefinition;

UENUM(BlueprintType)
enum class EOraclePlotState : uint8
{
	Empty,    // pronto para plantar
	Planted,  // crescendo (precisa de água diária)
	Ready     // pronto para colher
};

/**
 * Canteiro de cultivo. Ciclo: plantar (semente do inventário) →
 * regar a cada dia → dias passam (TimeSubsystem) → colher.
 * Visual do broto escala com o crescimento (placeholder até os assets).
 */
UCLASS()
class ORACLE_API AOracleFarmPlot : public AActor, public IOracleInteractable
{
	GENERATED_BODY()

public:
	AOracleFarmPlot();

	virtual void BeginPlay() override;

	// IOracleInteractable
	virtual FText GetInteractionText_Implementation() const override;
	virtual bool CanInteract_Implementation(AOracleCharacter* Interactor) const override;
	virtual void Interact_Implementation(AOracleCharacter* Interactor) override;

	// --- Acesso para o Save ---
	EOraclePlotState GetPlotState() const { return State; }
	UOracleCropDefinition* GetCrop() const { return Crop; }
	int32 GetDaysGrown() const { return DaysGrown; }
	bool IsWateredToday() const { return bWateredToday; }
	void RestoreState(EOraclePlotState InState, UOracleCropDefinition* InCrop,
	                  int32 InDaysGrown, bool bInWatered);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Oracle")
	TObjectPtr<UStaticMeshComponent> SoilMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Oracle")
	TObjectPtr<UStaticMeshComponent> CropMesh;

protected:
	UFUNCTION()
	void HandleDayChanged(int32 NewDay);

private:
	void UpdateVisuals();

	UPROPERTY()
	TObjectPtr<UOracleCropDefinition> Crop;

	EOraclePlotState State = EOraclePlotState::Empty;
	int32 DaysGrown = 0;
	bool bWateredToday = false;
	bool bUsingPrettyCrop = false;
};
