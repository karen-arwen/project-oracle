// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "OracleHUD.generated.h"

/**
 * HUD provisório desenhado em Canvas: relógio, prompt de interação,
 * inventário e dicas de controle. Zero assets — funciona no primeiro Play.
 * Será substituído por Common UI (widgets bonitos) na fase de UI;
 * a lógica exibida vem toda de componentes, então a troca é só visual.
 */
UCLASS()
class ORACLE_API AOracleHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

private:
	void DrawPanelText(const FString& Text, float X, float Y, const FLinearColor& Color, float Scale = 1.f);
};
