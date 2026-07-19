// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OraclePlayerController.generated.h"

class UInputMappingContext;
class UOracleInputConfig;

/**
 * PlayerController do jogador.
 * Responsabilidade única: registrar os Mapping Contexts no Enhanced Input.
 * O bind das ações fica no Pawn (OracleCharacter), que é quem reage a elas.
 *
 * InputConfig pode ser definido no editor (Data Asset). Se ficar vazio,
 * um conjunto padrão de teclas é criado em runtime (ver OracleInputDefaults),
 * garantindo que o projeto seja jogável imediatamente após compilar.
 */
UCLASS()
class ORACLE_API AOraclePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/**
	 * Config de input em uso (asset do editor ou padrão de runtime).
	 * Lazy: garante que o config exista mesmo se chamado antes do BeginPlay
	 * (o SetupPlayerInputComponent do Pawn pode rodar primeiro na possessão).
	 */
	UFUNCTION(BlueprintCallable, Category = "Oracle|Input")
	const UOracleInputConfig* GetInputConfig();

protected:
	virtual void BeginPlay() override;

	/** Cria config/contexto padrão em runtime caso não definidos no editor. */
	void EnsureInputAssets();

	/** Data Asset com as Input Actions. Opcional: vazio = padrão em runtime. */
	UPROPERTY(EditDefaultsOnly, Category = "Oracle|Input")
	TObjectPtr<UOracleInputConfig> InputConfig;

	/** Mapping Context principal. Opcional: vazio = padrão em runtime. */
	UPROPERTY(EditDefaultsOnly, Category = "Oracle|Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/** Prioridade do contexto padrão (contextos de UI/veículos usarão prioridades maiores). */
	UPROPERTY(EditDefaultsOnly, Category = "Oracle|Input")
	int32 DefaultContextPriority = 0;
};
