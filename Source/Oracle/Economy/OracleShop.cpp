// Project Oracle - Cozy Open World AAA

#include "Economy/OracleShop.h"

#include "Character/OracleCharacter.h"
#include "Collections/OracleCollectionSubsystem.h"
#include "Components/StaticMeshComponent.h"
#include "Economy/OracleWalletComponent.h"
#include "Engine/GameInstance.h"
#include "Inventory/OracleInventoryComponent.h"
#include "Inventory/OracleItemDefinition.h"

AOracleShop::AOracleShop()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

FText AOracleShop::GetInteractionText_Implementation() const
{
	return NSLOCTEXT("Oracle", "ShopSell", "Vender itens (recursos e comida)");
}

bool AOracleShop::CanInteract_Implementation(AOracleCharacter* Interactor) const
{
	return Interactor != nullptr;
}

void AOracleShop::Interact_Implementation(AOracleCharacter* Interactor)
{
	if (!Interactor)
	{
		return;
	}

	UOracleInventoryComponent* Inventory = Interactor->GetInventory();
	int32 TotalEarned = 0;

	// Vende cópia da lista (RemoveItem muta o inventário).
	TArray<FOracleInventoryEntry> Entries = Inventory->GetEntries();
	for (const FOracleInventoryEntry& Entry : Entries)
	{
		if (!Entry.Item)
		{
			continue;
		}
		// Só vende recursos e comida — móveis/sementes/ferramentas ficam.
		const bool bSellable =
			Entry.Item->Category == EOracleItemCategory::Resource ||
			Entry.Item->Category == EOracleItemCategory::Food;
		if (!bSellable)
		{
			continue;
		}

		const int32 Value = FMath::Max(1, FMath::RoundToInt(Entry.Item->BaseValue * SellRate));
		TotalEarned += Value * Entry.Count;
		Inventory->RemoveItem(Entry.Item, Entry.Count);
	}

	if (TotalEarned > 0)
	{
		Interactor->GetWallet()->AddCoins(TotalEarned);

		if (UOracleCollectionSubsystem* Toasts =
				GetGameInstance()->GetSubsystem<UOracleCollectionSubsystem>())
		{
			Toasts->AddToast(FText::Format(
				NSLOCTEXT("Oracle", "SoldToast", "Vendido por {0} Folhas!"), TotalEarned),
				/*bGolden=*/true);
		}
	}
}
