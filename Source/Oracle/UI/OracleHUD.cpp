// Project Oracle - Cozy Open World AAA

#include "UI/OracleHUD.h"

#include "Building/OraclePlacementComponent.h"
#include "Character/OracleCharacter.h"
#include "Collections/OracleCollectionSubsystem.h"
#include "Crafting/OracleCraftingComponent.h"
#include "Crafting/OracleRecipeDefinition.h"
#include "Economy/OracleWalletComponent.h"
#include "Fishing/OracleFishingComponent.h"
#include "Quests/OracleQuestComponent.h"
#include "Skills/OracleSkillsComponent.h"
#include "World/OracleWeatherSubsystem.h"
#include "Core/OracleTimeSubsystem.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/Font.h"
#include "Interaction/OracleInteractionComponent.h"
#include "Inventory/OracleInventoryComponent.h"
#include "Inventory/OracleItemDefinition.h"

void AOracleHUD::DrawHUD()
{
	Super::DrawHUD();

	const AOracleCharacter* Player = Cast<AOracleCharacter>(GetOwningPawn());
	if (!Player || !Canvas)
	{
		return;
	}

	// Relógio + clima (topo central).
	if (const UOracleTimeSubsystem* Time = GetWorld()->GetSubsystem<UOracleTimeSubsystem>())
	{
		FString Clock = Time->GetClockText();
		if (const UOracleWeatherSubsystem* Weather = GetWorld()->GetSubsystem<UOracleWeatherSubsystem>())
		{
			Clock += TEXT("  ·  ") + Weather->GetWeatherName().ToString();
		}
		DrawPanelText(Clock, Canvas->SizeX * 0.5f - 110.f, 24.f,
			FLinearColor(1.f, 0.95f, 0.8f), 1.4f);
	}

	// Moedas (topo direito).
	DrawPanelText(FString::Printf(TEXT("%d Folhas"), Player->GetWallet()->GetCoins()),
		Canvas->SizeX - 180.f, 24.f, FLinearColor(1.f, 0.9f, 0.4f), 1.3f);

	// Missão ativa (topo esquerdo — sempre visível, é o norte do jogador).
	if (const UOracleQuestComponent* Quest = Player->GetQuests())
	{
		const FText Objective = Quest->GetObjectiveText();
		if (!Objective.IsEmpty())
		{
			DrawPanelText(TEXT("★ Missão"), 32.f, 24.f, FLinearColor(1.f, 0.85f, 0.4f), 1.1f);
			DrawPanelText(Objective.ToString(), 32.f, 48.f, FLinearColor::White, 1.05f);
		}
	}

	// Prompt de interação (centro-baixo).
	const FText Prompt = Player->GetInteraction()->GetFocusedText();
	if (!Prompt.IsEmpty())
	{
		DrawPanelText(FString::Printf(TEXT("[E] %s"), *Prompt.ToString()),
			Canvas->SizeX * 0.5f - 80.f, Canvas->SizeY * 0.72f,
			FLinearColor(0.7f, 1.f, 0.75f), 1.3f);
	}

	// Modo decoração (aviso central).
	if (Player->GetPlacement()->IsBuildModeActive())
	{
		const UOracleItemDefinition* Item = Player->GetPlacement()->GetActiveItem();
		DrawPanelText(FString::Printf(TEXT("DECORANDO: %s  |  Clique: posicionar   R: girar   B: sair"),
				Item ? *Item->DisplayName.ToString() : TEXT("")),
			Canvas->SizeX * 0.5f - 260.f, Canvas->SizeY * 0.14f,
			FLinearColor(0.6f, 0.85f, 1.f), 1.2f);
	}

	// Inventário (esquerda).
	float Y = Canvas->SizeY * 0.3f;
	DrawPanelText(TEXT("— Mochila —"), 32.f, Y, FLinearColor(1.f, 0.9f, 0.6f), 1.1f);
	Y += 26.f;
	for (const FOracleInventoryEntry& Entry : Player->GetInventory()->GetEntries())
	{
		if (Entry.Item)
		{
			DrawPanelText(FString::Printf(TEXT("%dx %s"), Entry.Count,
					*Entry.Item->DisplayName.ToString()),
				32.f, Y, FLinearColor::White);
			Y += 22.f;
		}
	}

	// Estado da pesca (centro, grande — é ação em tempo real).
	if (const UOracleFishingComponent* Fish = Player->GetFishing())
	{
		const EOracleFishingState FS = Fish->GetState();
		if (FS == EOracleFishingState::Casting)
		{
			DrawPanelText(TEXT("🎣 Aguardando a fisgada..."),
				Canvas->SizeX * 0.5f - 130.f, Canvas->SizeY * 0.6f,
				FLinearColor(0.7f, 0.9f, 1.f), 1.3f);
		}
		else if (FS == EOracleFishingState::Biting)
		{
			DrawPanelText(TEXT("❗ FISGOU! [E] PUXE AGORA!"),
				Canvas->SizeX * 0.5f - 150.f, Canvas->SizeY * 0.6f,
				FLinearColor(1.f, 0.85f, 0.3f), 1.6f);
		}
	}

	// Dica de craft: aparece quando há receita possível com a mochila atual.
	if (const UOracleRecipeDefinition* Recipe = Player->GetCrafting()->GetFirstCraftable())
	{
		DrawPanelText(FString::Printf(TEXT("[C] Criar: %s"), *Recipe->DisplayName.ToString()),
			Canvas->SizeX * 0.5f - 80.f, Canvas->SizeY * 0.78f,
			FLinearColor(1.f, 0.8f, 0.5f), 1.15f);
	}

	// Toasts de coleta/descoberta (direita) + contador do compêndio.
	if (UOracleCollectionSubsystem* Collection =
			GetGameInstance()->GetSubsystem<UOracleCollectionSubsystem>())
	{
		float ToastY = Canvas->SizeY * 0.35f;
		for (const FOracleCollectionEvent& Event : Collection->GetRecentEvents())
		{
			const FLinearColor Color = Event.bIsDiscovery
				? FLinearColor(1.f, 0.85f, 0.35f)   // descoberta: dourado
				: FLinearColor(0.85f, 1.f, 0.9f);   // coleta: verde-claro
			DrawPanelText(Event.Text.ToString(), Canvas->SizeX - 340.f, ToastY,
				Color, Event.bIsDiscovery ? 1.25f : 1.05f);
			ToastY += 30.f;
		}

		DrawPanelText(FString::Printf(TEXT("Compêndio: %d descobertos"),
				Collection->GetDiscoveredCount()),
			32.f, Y + 14.f, FLinearColor(0.75f, 0.85f, 1.f), 0.95f);
	}

	// Habilidades (progressão por uso).
	{
		const UOracleSkillsComponent* Sk = Player->GetSkills();
		const FString SkillsLine = FString::Printf(
			TEXT("Coleta %d · Fazenda %d · Artesanato %d · Construção %d"),
			Sk->GetLevel(EOracleSkill::Coleta), Sk->GetLevel(EOracleSkill::Fazenda),
			Sk->GetLevel(EOracleSkill::Artesanato), Sk->GetLevel(EOracleSkill::Construcao));
		DrawPanelText(SkillsLine, 32.f, Y + 40.f, FLinearColor(0.9f, 0.8f, 1.f), 0.9f);
	}

	// Dicas (canto inferior esquerdo).
	DrawPanelText(TEXT("WASD mover  |  Shift sprint  |  Ctrl andar  |  E interagir  |  B decorar  |  F5 salvar  F9 carregar"),
		32.f, Canvas->SizeY - 44.f, FLinearColor(1.f, 1.f, 1.f, 0.6f), 0.95f);
}

void AOracleHUD::DrawPanelText(const FString& Text, const float X, const float Y,
                               const FLinearColor& Color, const float Scale)
{
	if (UFont* Font = GEngine->GetLargeFont())
	{
		// Sombra para legibilidade sobre qualquer fundo.
		DrawText(Text, FLinearColor(0.f, 0.f, 0.f, 0.7f), X + 1.5f, Y + 1.5f, Font, Scale);
		DrawText(Text, Color, X, Y, Font, Scale);
	}
}
