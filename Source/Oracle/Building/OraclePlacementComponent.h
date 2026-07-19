// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OraclePlacementComponent.generated.h"

class UStaticMeshComponent;
class UOracleItemDefinition;

/**
 * Modo decoração: mostra um "fantasma" do móvel seguindo o olhar,
 * com snap em grade e rotação em passos de 45°. Confirmar consome o
 * item do inventário e cria um AOraclePlacedProp sólido.
 *
 * Futuro: validação de colisão com tint verde/vermelho, empilhar,
 * superfícies de parede, e o modo casa completo estilo Sims.
 */
UCLASS(ClassGroup = (Oracle), meta = (BlueprintSpawnableComponent))
class ORACLE_API UOraclePlacementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UOraclePlacementComponent();

	UFUNCTION(BlueprintCallable, Category = "Oracle|Building")
	void ToggleBuildMode();

	UFUNCTION(BlueprintCallable, Category = "Oracle|Building")
	void ConfirmPlacement();

	UFUNCTION(BlueprintCallable, Category = "Oracle|Building")
	void RotateGhost();

	UFUNCTION(BlueprintPure, Category = "Oracle|Building")
	bool IsBuildModeActive() const { return bActive; }

	/** Item atualmente no fantasma (para o HUD). */
	UFUNCTION(BlueprintPure, Category = "Oracle|Building")
	UOracleItemDefinition* GetActiveItem() const { return ActiveItem; }

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	/** Tamanho da célula do snap (cm). */
	UPROPERTY(EditDefaultsOnly, Category = "Oracle|Building", meta = (ClampMin = 1))
	float GridSize = 50.f;

	/** Alcance do posicionamento a partir da câmera (cm). */
	UPROPERTY(EditDefaultsOnly, Category = "Oracle|Building", meta = (ClampMin = 200))
	float PlaceRange = 900.f;

private:
	void EnterBuildMode();
	void ExitBuildMode();
	bool UpdateGhostTransform();

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> GhostMesh;

	UPROPERTY()
	TObjectPtr<UOracleItemDefinition> ActiveItem;

	bool bActive = false;
	bool bValidSpot = false;
	float GhostYaw = 0.f;
};
