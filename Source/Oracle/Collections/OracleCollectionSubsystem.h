// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OracleCollectionSubsystem.generated.h"

class UOracleItemDefinition;

/** Evento recente para o HUD (toast de coleta/descoberta). */
USTRUCT(BlueprintType)
struct FOracleCollectionEvent
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Collection")
	FText Text;

	UPROPERTY(BlueprintReadOnly, Category = "Collection")
	bool bIsDiscovery = false;

	double TimeSeconds = 0.0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOracleItemDiscovered, UOracleItemDefinition*, Item);

/**
 * Compêndio do jogo: registra tudo que o jogador já descobriu e coletou.
 * Pilar de design "colecionismo" — futuras coleções (peixes, insetos,
 * minerais, receitas, roupas) usam este mesmo registro por nome.
 *
 * Também alimenta o HUD com toasts de coleta ("+2 Abóbora") e de
 * descoberta ("✦ Novo na coleção!") — feedback imediato é game feel.
 */
UCLASS()
class ORACLE_API UOracleCollectionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/** Chamado pelo inventário sempre que itens entram. */
	void RegisterItemCollected(UOracleItemDefinition* Item, int32 Count);

	UFUNCTION(BlueprintPure, Category = "Oracle|Collection")
	bool IsDiscovered(const UOracleItemDefinition* Item) const;

	UFUNCTION(BlueprintPure, Category = "Oracle|Collection")
	int32 GetDiscoveredCount() const { return DiscoveredNames.Num(); }

	/** Total acumulado já coletado de um item (estatística do compêndio). */
	UFUNCTION(BlueprintPure, Category = "Oracle|Collection")
	int32 GetTotalCollected(const UOracleItemDefinition* Item) const;

	/** Eventos dos últimos segundos, já sem os expirados (para o HUD). */
	const TArray<FOracleCollectionEvent>& GetRecentEvents();

	/** Persistência (SaveSubsystem). */
	TArray<FString> GetDiscoveredForSave() const { return DiscoveredNames.Array(); }
	void RestoreFromSave(const TArray<FString>& InDiscovered);

	UPROPERTY(BlueprintAssignable, Category = "Oracle|Collection")
	FOracleItemDiscovered OnItemDiscovered;

	/** Quanto tempo (s) um toast permanece na tela. */
	static constexpr double EventLifetime = 4.0;

private:
	TSet<FString> DiscoveredNames;
	TMap<FString, int32> TotalCollected;
	TArray<FOracleCollectionEvent> RecentEvents;
};
