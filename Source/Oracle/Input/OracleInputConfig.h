// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OracleInputConfig.generated.h"

class UInputAction;

/**
 * Data Asset que agrupa as Input Actions do jogador.
 * Design: código nunca referencia teclas — só ações. Teclas ficam no
 * Mapping Context, o que torna rebinding e suporte a gamepad triviais.
 *
 * Ações futuras (Climb, Glide, Interact, PhotoMode...) entram aqui,
 * cada uma em seu contexto apropriado.
 */
UCLASS(BlueprintType)
class ORACLE_API UOracleInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	/** Axis2D: X = direita/esquerda, Y = frente/trás. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion")
	TObjectPtr<UInputAction> Move;

	/** Axis2D: rotação da câmera (mouse / analógico direito). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UInputAction> Look;

	/** Axis1D: zoom da câmera (scroll do mouse). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UInputAction> ZoomCamera;

	/** Digital: pulo. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion")
	TObjectPtr<UInputAction> Jump;

	/** Digital (hold): sprint. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion")
	TObjectPtr<UInputAction> Sprint;

	/** Digital (toggle): alterna andar/correr — essencial no ritmo cozy. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion")
	TObjectPtr<UInputAction> WalkToggle;

	/** Digital: interagir com o mundo (coletar, plantar, falar...). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<UInputAction> Interact;

	/** Digital: criar a primeira receita disponível. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crafting")
	TObjectPtr<UInputAction> Craft;

	/** Digital: liga/desliga o modo de decoração. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building")
	TObjectPtr<UInputAction> BuildToggle;

	/** Digital: confirma o posicionamento do móvel (modo decoração). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building")
	TObjectPtr<UInputAction> Place;

	/** Digital: gira o móvel 45° (modo decoração). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building")
	TObjectPtr<UInputAction> RotateProp;

	/** Digital: salvar rápido. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "System")
	TObjectPtr<UInputAction> QuickSave;

	/** Digital: carregar rápido. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "System")
	TObjectPtr<UInputAction> QuickLoad;
};
