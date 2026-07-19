// Project Oracle - Cozy Open World AAA

#include "Inventory/OracleInventoryComponent.h"

#include "Collections/OracleCollectionSubsystem.h"
#include "Engine/GameInstance.h"
#include "GameFramework/Actor.h"

void UOracleInventoryComponent::AddItem(UOracleItemDefinition* Item, int32 Count)
{
	if (!Item || Count <= 0)
	{
		return;
	}

	// Compêndio: registra coleta e descobertas (toast no HUD).
	if (const AActor* Owner = GetOwner())
	{
		if (UGameInstance* GI = Owner->GetGameInstance())
		{
			if (UOracleCollectionSubsystem* Collection = GI->GetSubsystem<UOracleCollectionSubsystem>())
			{
				Collection->RegisterItemCollected(Item, Count);
			}
		}
	}

	// Preenche pilhas existentes antes de criar novas.
	for (FOracleInventoryEntry& Entry : Entries)
	{
		if (Entry.Item == Item && Entry.Count < Item->MaxStack)
		{
			const int32 Space = Item->MaxStack - Entry.Count;
			const int32 ToAdd = FMath::Min(Space, Count);
			Entry.Count += ToAdd;
			Count -= ToAdd;
			if (Count == 0)
			{
				break;
			}
		}
	}
	while (Count > 0)
	{
		const int32 ToAdd = FMath::Min(Item->MaxStack, Count);
		Entries.Add({Item, ToAdd});
		Count -= ToAdd;
	}

	OnInventoryChanged.Broadcast();
}

int32 UOracleInventoryComponent::RemoveItem(UOracleItemDefinition* Item, const int32 Count)
{
	if (!Item || Count <= 0)
	{
		return 0;
	}

	int32 Remaining = Count;
	for (int32 i = Entries.Num() - 1; i >= 0 && Remaining > 0; --i)
	{
		if (Entries[i].Item == Item)
		{
			const int32 ToRemove = FMath::Min(Entries[i].Count, Remaining);
			Entries[i].Count -= ToRemove;
			Remaining -= ToRemove;
			if (Entries[i].Count == 0)
			{
				Entries.RemoveAt(i);
			}
		}
	}

	const int32 Removed = Count - Remaining;
	if (Removed > 0)
	{
		OnInventoryChanged.Broadcast();
	}
	return Removed;
}

int32 UOracleInventoryComponent::CountOf(const UOracleItemDefinition* Item) const
{
	int32 Total = 0;
	for (const FOracleInventoryEntry& Entry : Entries)
	{
		if (Entry.Item == Item)
		{
			Total += Entry.Count;
		}
	}
	return Total;
}

UOracleItemDefinition* UOracleInventoryComponent::FindFirstOfCategory(const EOracleItemCategory Category) const
{
	for (const FOracleInventoryEntry& Entry : Entries)
	{
		if (Entry.Item && Entry.Item->Category == Category)
		{
			return Entry.Item;
		}
	}
	return nullptr;
}

void UOracleInventoryComponent::SetEntries(const TArray<FOracleInventoryEntry>& NewEntries)
{
	Entries = NewEntries;
	OnInventoryChanged.Broadcast();
}
