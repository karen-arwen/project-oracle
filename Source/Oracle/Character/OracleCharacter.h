// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "OracleCharacter.generated.h"

class UCameraComponent;
class UCameraShakeBase;
class UNiagaraSystem;
class USoundBase;
class USpringArmComponent;
class UOracleCameraComponent;
class UOracleCharacterMovementComponent;
class UOracleCraftingComponent;
class UOracleSkillsComponent;
class UOracleInteractionComponent;
class UOracleInventoryComponent;
class UOraclePlacementComponent;
struct FInputActionValue;

/**
 * Personagem jogável.
 *
 * Filosofia: o Character é um COORDENADOR fino. Lógica de verdade vive em
 * componentes (movimento, câmera; futuramente interação, habilidades,
 * inventário). Isso mantém a classe pequena mesmo com dezenas de sistemas.
 *
 * Game feel implementado aqui (é resposta a input, papel do Pawn):
 * - Coyote time: pulo ainda vale ~0.15s após sair de uma borda.
 * - Jump buffer: apertar pulo pouco antes de pousar dispara ao tocar o chão.
 * - Pulo variável: segurar Espaço pula mais alto (JumpMaxHoldTime).
 * - Feedback de pouso: shake + som + poeira, escalados pela queda.
 */
UCLASS()
class ORACLE_API AOracleCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	explicit AOracleCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Landed(const FHitResult& Hit) override;

	UOracleCharacterMovementComponent* GetOracleMovement() const;

	UFUNCTION(BlueprintPure, Category = "Oracle")
	UOracleInventoryComponent* GetInventory() const { return Inventory; }

	UFUNCTION(BlueprintPure, Category = "Oracle")
	UOracleInteractionComponent* GetInteraction() const { return Interaction; }

	UFUNCTION(BlueprintPure, Category = "Oracle")
	UOraclePlacementComponent* GetPlacement() const { return Placement; }

	UFUNCTION(BlueprintPure, Category = "Oracle")
	UOracleCraftingComponent* GetCrafting() const { return Crafting; }

	UFUNCTION(BlueprintPure, Category = "Oracle")
	UOracleSkillsComponent* GetSkills() const { return Skills; }

	UCameraComponent* GetThirdPersonCamera() const { return Camera; }

	/** Chamado por Anim Notify (passos). Null-safe: sem asset, sem som. */
	UFUNCTION(BlueprintCallable, Category = "Oracle|Feedback")
	void PlayFootstep();

protected:
	virtual void BeginPlay() override;
	virtual bool CanJumpInternal_Implementation() const override;

	// --- Handlers de input ---
	void Input_Move(const FInputActionValue& Value);
	void Input_Look(const FInputActionValue& Value);
	void Input_Zoom(const FInputActionValue& Value);
	void Input_JumpPressed(const FInputActionValue& Value);
	void Input_SprintStart(const FInputActionValue& Value);
	void Input_SprintEnd(const FInputActionValue& Value);
	void Input_WalkToggle(const FInputActionValue& Value);
	void Input_Interact(const FInputActionValue& Value);
	void Input_Craft(const FInputActionValue& Value);
	void Input_BuildToggle(const FInputActionValue& Value);
	void Input_Place(const FInputActionValue& Value);
	void Input_RotateProp(const FInputActionValue& Value);
	void Input_QuickSave(const FInputActionValue& Value);
	void Input_QuickLoad(const FInputActionValue& Value);

	// --- Componentes ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Oracle|Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Oracle|Camera")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Oracle|Camera")
	TObjectPtr<UOracleCameraComponent> CameraController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Oracle|Systems")
	TObjectPtr<UOracleInventoryComponent> Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Oracle|Systems")
	TObjectPtr<UOracleInteractionComponent> Interaction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Oracle|Systems")
	TObjectPtr<UOraclePlacementComponent> Placement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Oracle|Systems")
	TObjectPtr<UOracleCraftingComponent> Crafting;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Oracle|Systems")
	TObjectPtr<UOracleSkillsComponent> Skills;

	// --- Game feel: pulo ---
	/** Janela (s) em que o pulo ainda vale após sair de uma borda. */
	UPROPERTY(EditDefaultsOnly, Category = "Oracle|Jump", meta = (ClampMin = 0, ClampMax = 0.5))
	float CoyoteTime = 0.15f;

	/** Janela (s) em que um pulo apertado no ar dispara ao pousar. */
	UPROPERTY(EditDefaultsOnly, Category = "Oracle|Jump", meta = (ClampMin = 0, ClampMax = 0.5))
	float JumpBufferTime = 0.15f;

	// --- Feedback de pouso/passos (assets opcionais, atribuídos no editor) ---
	UPROPERTY(EditDefaultsOnly, Category = "Oracle|Feedback")
	TSubclassOf<UCameraShakeBase> LandShake;

	UPROPERTY(EditDefaultsOnly, Category = "Oracle|Feedback")
	TObjectPtr<USoundBase> LandSound;

	UPROPERTY(EditDefaultsOnly, Category = "Oracle|Feedback")
	TObjectPtr<UNiagaraSystem> LandDustVFX;

	UPROPERTY(EditDefaultsOnly, Category = "Oracle|Feedback")
	TObjectPtr<USoundBase> FootstepSound;

	/** Queda mínima (cm/s) para gerar feedback de pouso. */
	UPROPERTY(EditDefaultsOnly, Category = "Oracle|Feedback", meta = (ClampMin = 0))
	float MinLandFeedbackFallSpeed = 350.f;

	/** Queda (cm/s) que corresponde ao feedback máximo. */
	UPROPERTY(EditDefaultsOnly, Category = "Oracle|Feedback", meta = (ClampMin = 0))
	float MaxLandFeedbackFallSpeed = 1200.f;

private:
	bool bWalkLocked = false;
	float LastGroundedTime = -1000.f;
	float JumpPressedTime = -1000.f;
	float PeakFallSpeed = 0.f;  // registrada durante a queda, consumida no pouso
};
