// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/OracleItemDefinition.h"
#include "OracleInventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FOracleInventoryEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<UOracleItemDefinition> Item;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	int32 Count = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOracleInventoryChanged);

/**
 * Inventário por empilhamento: lista de {definição, quantidade}.
 * Sem slots/grid por enquanto — UI de grade é camada de apresentação
 * futura (Common UI); a lógica aqui não muda quando ela chegar.
 */
UCLASS(ClassGroup = (Oracle), meta = (BlueprintSpawnableComponent))
class ORACLE_API UOracleInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Adiciona itens (respeita MaxStack criando múltiplas pilhas). */
	UFUNCTION(BlueprintCallable, Category = "Oracle|Inventory")
	void AddItem(UOracleItemDefinition* Item, int32 Count = 1);

	/** Remove até Count itens; retorna quantos realmente saíram. */
	UFUNCTION(BlueprintCallable, Category = "Oracle|Inventory")
	int32 RemoveItem(UOracleItemDefinition* Item, int32 Count = 1);

	UFUNCTION(BlueprintPure, Category = "Oracle|Inventory")
	int32 CountOf(const UOracleItemDefinition* Item) const;

	/** Primeiro item da categoria (ex.: primeira semente para plantar). */
	UFUNCTION(BlueprintPure, Category = "Oracle|Inventory")
	UOracleItemDefinition* FindFirstOfCategory(EOracleItemCategory Category) const;

	UFUNCTION(BlueprintPure, Category = "Oracle|Inventory")
	const TArray<FOracleInventoryEntry>& GetEntries() const { return Entries; }

	/** Substitui todo o conteúdo (usado pelo Load). */
	void SetEntries(const TArray<FOracleInventoryEntry>& NewEntries);

	UPROPERTY(BlueprintAssignable, Category = "Oracle|Inventory")
	FOracleInventoryChanged OnInventoryChanged;

private:
	UPROPERTY()
	TArray<FOracleInventoryEntry> Entries;
};
