// Project Oracle - Cozy Open World AAA

#include "Collections/OracleCollectionSubsystem.h"

#include "HAL/PlatformTime.h"
#include "Inventory/OracleItemDefinition.h"

void UOracleCollectionSubsystem::RegisterItemCollected(UOracleItemDefinition* Item, const int32 Count)
{
	if (!Item || Count <= 0)
	{
		return;
	}

	const FString Name = Item->GetName();
	TotalCollected.FindOrAdd(Name) += Count;

	const double Now = FPlatformTime::Seconds();

	// Toast de coleta: "+2 Abóbora Estrela".
	FOracleCollectionEvent Pickup;
	Pickup.Text = FText::Format(
		NSLOCTEXT("Oracle", "PickupToast", "+{0} {1}"), Count, Item->DisplayName);
	Pickup.TimeSeconds = Now;
	RecentEvents.Add(Pickup);

	// Primeira vez? Descoberta — momento de celebração do compêndio.
	if (!DiscoveredNames.Contains(Name))
	{
		DiscoveredNames.Add(Name);

		FOracleCollectionEvent Discovery;
		Discovery.Text = FText::Format(
			NSLOCTEXT("Oracle", "DiscoveryToast", "✦ Novo na coleção: {0}!"),
			Item->DisplayName);
		Discovery.bIsDiscovery = true;
		Discovery.TimeSeconds = Now;
		RecentEvents.Add(Discovery);

		OnItemDiscovered.Broadcast(Item);
	}
}

bool UOracleCollectionSubsystem::IsDiscovered(const UOracleItemDefinition* Item) const
{
	return Item && DiscoveredNames.Contains(Item->GetName());
}

int32 UOracleCollectionSubsystem::GetTotalCollected(const UOracleItemDefinition* Item) const
{
	if (!Item)
	{
		return 0;
	}
	const int32* Found = TotalCollected.Find(Item->GetName());
	return Found ? *Found : 0;
}

const TArray<FOracleCollectionEvent>& UOracleCollectionSubsystem::GetRecentEvents()
{
	const double Now = FPlatformTime::Seconds();
	RecentEvents.RemoveAll([Now](const FOracleCollectionEvent& Event)
	{
		return Now - Event.TimeSeconds > EventLifetime;
	});
	return RecentEvents;
}

void UOracleCollectionSubsystem::RestoreFromSave(const TArray<FString>& InDiscovered)
{
	DiscoveredNames.Empty();
	for (const FString& Name : InDiscovered)
	{
		DiscoveredNames.Add(Name);
	}
	RecentEvents.Empty();  // sem toasts fantasmas após o load
}
