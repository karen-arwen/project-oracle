// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OracleEnvironment.generated.h"

class UDirectionalLightComponent;
class UExponentialHeightFogComponent;
class UPostProcessComponent;
class USkyAtmosphereComponent;
class USkyLightComponent;
class UVolumetricCloudComponent;

/**
 * Rig completo de céu/iluminação/atmosfera do jogo, 100% em C++:
 * sol dinâmico, céu físico, nuvens volumétricas, névoa com godrays,
 * skylight em tempo real e color grading vibrante (bloom, saturação,
 * vinheta) — o "look anime cozy" base, sem depender de assets.
 *
 * Lê o UOracleTimeSubsystem e pinta o dia: luz quente no amanhecer/
 * entardecer, branca ao meio-dia, azulada e fraca à noite.
 *
 * Spawnado pelo GameMode se o mapa não tiver um. Substitui (remove)
 * luzes/céu padrão do mapa para não haver rig duplicado.
 */
UCLASS()
class ORACLE_API AOracleEnvironment : public AActor
{
	GENERATED_BODY()

public:
	AOracleEnvironment();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	/** Remove DirectionalLight/SkyLight/Fog/SkyAtmosphere/Clouds pré-existentes do mapa. */
	UPROPERTY(EditAnywhere, Category = "Oracle|Environment")
	bool bReplaceExistingSky = true;

	// --- Tuning do ciclo de luz ---
	UPROPERTY(EditAnywhere, Category = "Oracle|Sun")
	FLinearColor DayColor = FLinearColor(1.f, 0.95f, 0.88f);

	UPROPERTY(EditAnywhere, Category = "Oracle|Sun")
	FLinearColor DuskColor = FLinearColor(1.f, 0.45f, 0.2f);

	UPROPERTY(EditAnywhere, Category = "Oracle|Sun")
	FLinearColor NightColor = FLinearColor(0.35f, 0.45f, 0.8f);

	UPROPERTY(EditAnywhere, Category = "Oracle|Sun", meta = (ClampMin = 0))
	float DayIntensity = 8.f;

	UPROPERTY(EditAnywhere, Category = "Oracle|Sun", meta = (ClampMin = 0))
	float NightIntensity = 0.05f;

	// --- Componentes ---
	UPROPERTY(VisibleAnywhere, Category = "Oracle") TObjectPtr<UDirectionalLightComponent> Sun;
	UPROPERTY(VisibleAnywhere, Category = "Oracle") TObjectPtr<USkyAtmosphereComponent> SkyAtmosphere;
	UPROPERTY(VisibleAnywhere, Category = "Oracle") TObjectPtr<UVolumetricCloudComponent> Clouds;
	UPROPERTY(VisibleAnywhere, Category = "Oracle") TObjectPtr<USkyLightComponent> SkyLight;
	UPROPERTY(VisibleAnywhere, Category = "Oracle") TObjectPtr<UExponentialHeightFogComponent> Fog;
	UPROPERTY(VisibleAnywhere, Category = "Oracle") TObjectPtr<UPostProcessComponent> PostProcess;

private:
	void RemoveDuplicateSkyActors();
	void UpdateLighting();
};
