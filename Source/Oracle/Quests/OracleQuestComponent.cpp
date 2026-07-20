// Project Oracle - Cozy Open World AAA

#include "Quests/OracleQuestComponent.h"

#include "Character/OracleCharacter.h"
#include "Collections/OracleCollectionSubsystem.h"
#include "Economy/OracleWalletComponent.h"
#include "Engine/GameInstance.h"
#include "Inventory/OracleInventoryComponent.h"
#include "Inventory/OracleItemDefinition.h"
#include "Quests/OracleQuestDefinition.h"

void UOracleQuestComponent::BeginPlay()
{
	Super::BeginPlay();

	if (const AOracleCharacter* Character = Cast<AOracleCharacter>(GetOwner()))
	{
		Character->GetInventory()->OnInventoryChanged.AddDynamic(
			this, &UOracleQuestComponent::HandleInventoryChanged);
	}
}

void UOracleQuestComponent::SetActiveQuest(UOracleQuestDefinition* Quest)
{
	ActiveQuest = Quest;
	if (Quest)
	{
		if (UOracleCollectionSubsystem* Toasts =
				GetOwner()->GetGameInstance()->GetSubsystem<UOracleCollectionSubsystem>())
		{
			Toasts->AddToast(FText::Format(
				NSLOCTEXT("Oracle", "QuestNew", "✦ Nova missão: {0}"), Quest->Title), /*bGolden=*/true);
		}
		TryComplete();  // caso o jogador já tenha os itens
	}
}

void UOracleQuestComponent::QueueQuest(UOracleQuestDefinition* Quest)
{
	if (!Quest)
	{
		return;
	}
	if (!ActiveQuest)
	{
		SetActiveQuest(Quest);
	}
	else
	{
		Queue.Add(Quest);
	}
}

void UOracleQuestComponent::AdvanceQueue()
{
	if (Queue.Num() > 0)
	{
		UOracleQuestDefinition* Next = Queue[0];
		Queue.RemoveAt(0);
		SetActiveQuest(Next);
	}
	else
	{
		ActiveQuest = nullptr;
	}
}

int32 UOracleQuestComponent::GetProgress() const
{
	const AOracleCharacter* Character = Cast<AOracleCharacter>(GetOwner());
	if (!ActiveQuest || !ActiveQuest->TargetItem || !Character)
	{
		return 0;
	}
	return FMath::Min(Character->GetInventory()->CountOf(ActiveQuest->TargetItem),
		ActiveQuest->TargetCount);
}

FText UOracleQuestComponent::GetObjectiveText() const
{
	if (!ActiveQuest || !ActiveQuest->TargetItem)
	{
		return FText::GetEmpty();
	}
	return FText::Format(NSLOCTEXT("Oracle", "QuestObj", "{0}: {1}/{2} {3}"),
		ActiveQuest->Title, GetProgress(), ActiveQuest->TargetCount,
		ActiveQuest->TargetItem->DisplayName);
}

void UOracleQuestComponent::HandleInventoryChanged()
{
	TryComplete();
}

void UOracleQuestComponent::TryComplete()
{
	AOracleCharacter* Character = Cast<AOracleCharacter>(GetOwner());
	if (!ActiveQuest || !ActiveQuest->TargetItem || !Character)
	{
		return;
	}

	UOracleInventoryComponent* Inventory = Character->GetInventory();
	if (Inventory->CountOf(ActiveQuest->TargetItem) < ActiveQuest->TargetCount)
	{
		return;
	}

	// Objetivo atingido — paga recompensa.
	if (ActiveQuest->bConsumeItems)
	{
		Inventory->RemoveItem(ActiveQuest->TargetItem, ActiveQuest->TargetCount);
	}
	if (ActiveQuest->RewardCoins > 0)
	{
		Character->GetWallet()->AddCoins(ActiveQuest->RewardCoins);
	}
	if (ActiveQuest->RewardItem && ActiveQuest->RewardItemCount > 0)
	{
		Inventory->AddItem(ActiveQuest->RewardItem, ActiveQuest->RewardItemCount);
	}

	if (UOracleCollectionSubsystem* Toasts =
			GetOwner()->GetGameInstance()->GetSubsystem<UOracleCollectionSubsystem>())
	{
		Toasts->AddToast(FText::Format(
			NSLOCTEXT("Oracle", "QuestDone", "✔ Missão concluída: {0} (+{1} Folhas)"),
			ActiveQuest->Title, ActiveQuest->RewardCoins), /*bGolden=*/true);
	}

	OnQuestCompleted.Broadcast(ActiveQuest);
	AdvanceQueue();
}
