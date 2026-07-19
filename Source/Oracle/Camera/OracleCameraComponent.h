// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OracleCameraComponent.generated.h"

class UCameraComponent;
class USpringArmComponent;

/**
 * Controlador da câmera third person.
 * Vive como componente do Character e cuida do game feel da câmera:
 * zoom interpolado com limites e kick de FOV durante o sprint.
 *
 * Futuro: presets (exploração, interior, diálogo, foto) viverão aqui,
 * e o Photo Mode assumirá o controle desta câmera.
 */
UCLASS(ClassGroup = (Oracle), meta = (BlueprintSpawnableComponent))
class ORACLE_API UOracleCameraComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UOracleCameraComponent();

	/** Conecta o rig que este componente controla. Chamar no BeginPlay do dono. */
	void Initialize(USpringArmComponent* InSpringArm, UCameraComponent* InCamera);

	/** Ajusta o zoom desejado. Delta positivo aproxima. */
	UFUNCTION(BlueprintCallable, Category = "Oracle|Camera")
	void AddZoom(float Delta);

	/** Liga/desliga o kick de FOV do sprint (aplicado só com velocidade real). */
	UFUNCTION(BlueprintCallable, Category = "Oracle|Camera")
	void SetSprintFOVActive(bool bActive) { bSprintFOVRequested = bActive; }

	/** "Assenta" a câmera no pouso: mergulho rápido e retorno suave. */
	UFUNCTION(BlueprintCallable, Category = "Oracle|Camera")
	void StartLandingDip(float Intensity);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// --- Zoom ---
	UPROPERTY(EditDefaultsOnly, Category = "Oracle|Zoom", meta = (ClampMin = 100))
	float MinArmLength = 150.f;

	UPROPERTY(EditDefaultsOnly, Category = "Oracle|Zoom", meta = (ClampMin = 200))
	float MaxArmLength = 800.f;

	/** Quanto cada "clique" de scroll altera a distância (cm). */
	UPROPERTY(EditDefaultsOnly, Category = "Oracle|Zoom", meta = (ClampMin = 1))
	float ZoomStep = 60.f;

	UPROPERTY(EditDefaultsOnly, Category = "Oracle|Zoom", meta = (ClampMin = 0.1))
	float ZoomInterpSpeed = 8.f;

	// --- FOV do sprint ---
	/** Graus adicionados ao FOV durante o sprint (sensação de velocidade). */
	UPROPERTY(EditDefaultsOnly, Category = "Oracle|FOV", meta = (ClampMin = 0, ClampMax = 30))
	float SprintFOVKick = 8.f;

	/** Velocidade 2D mínima (cm/s) para o kick valer — sprint parado não conta. */
	UPROPERTY(EditDefaultsOnly, Category = "Oracle|FOV", meta = (ClampMin = 0))
	float SprintFOVMinSpeed = 400.f;

	UPROPERTY(EditDefaultsOnly, Category = "Oracle|FOV", meta = (ClampMin = 0.1))
	float FOVInterpSpeed = 6.f;

	// --- Dip de pouso ---
	/** Quanto a câmera desce no pouso (cm), escalado pela intensidade. */
	UPROPERTY(EditDefaultsOnly, Category = "Oracle|LandingDip", meta = (ClampMin = 0))
	float DipAmplitude = 14.f;

	UPROPERTY(EditDefaultsOnly, Category = "Oracle|LandingDip", meta = (ClampMin = 0.05))
	float DipDuration = 0.28f;

private:
	TWeakObjectPtr<USpringArmComponent> SpringArm;
	TWeakObjectPtr<UCameraComponent> Camera;
	float TargetArmLength = 450.f;
	float BaseFOV = 90.f;
	bool bSprintFOVRequested = false;
	FVector BaseSocketOffset = FVector::ZeroVector;
	float DipElapsed = -1.f;   // <0 = inativo
	float DipIntensity = 1.f;
};
