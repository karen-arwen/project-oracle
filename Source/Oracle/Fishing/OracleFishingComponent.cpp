// Project Oracle - Cozy Open World AAA

#include "Fishing/OracleFishingComponent.h"

#include "Character/OracleCharacter.h"
#include "Collections/OracleCollectionSubsystem.h"
#include "Fishing/OracleFishDefinition.h"
#include "Inventory/OracleInventoryComponent.h"
#include "Inventory/OracleItemDefinition.h"
#include "Skills/OracleSkillsComponent.h"
#include "TimerManager.h"

void UOracleFishingComponent::SetFishPool(const TArray<UOracleFishDefinition*>& Pool)
{
	FishPool.Reset();
	for (UOracleFishDefinition* Fish : Pool)
	{
		FishPool.Add(Fish);
	}
}

void UOracleFishingComponent::ToggleFishing()
{
	switch (State)
	{
	case EOracleFishingState::Idle:    StartCast(); break;
	case EOracleFishingState::Biting:  Reel();      break;
	case EOracleFishingState::Casting: Cancel();    break;  // recolhe cedo = cancela
	}
}

void UOracleFishingComponent::StartCast()
{
	if (FishPool.Num() == 0)
	{
		Toast(NSLOCTEXT("Oracle", "NoFish", "Não há peixes por aqui..."), false);
		return;
	}

	State = EOracleFishingState::Casting;
	Toast(NSLOCTEXT("Oracle", "Casting", "🎣 Linha na água... aguarde a fisgada."), false);

	const float Wait = FMath::FRandRange(MinWaitTime, MaxWaitTime);
	GetWorld()->GetTimerManager().SetTimer(BiteTimer, this, &UOracleFishingComponent::OnBite, Wait, false);
}

void UOracleFishingComponent::OnBite()
{
	State = EOracleFishingState::Biting;
	Toast(NSLOCTEXT("Oracle", "Bite", "❗ FISGOU! Aperte para puxar!"), true);

	GetWorld()->GetTimerManager().SetTimer(WindowTimer, this, &UOracleFishingComponent::EndBite,
		BiteWindow, false);
}

void UOracleFishingComponent::EndBite()
{
	// Não puxou a tempo — o peixe escapou (sem punição, apenas recomeça).
	State = EOracleFishingState::Idle;
	Toast(NSLOCTEXT("Oracle", "Escaped", "O peixe escapou! Tente de novo."), false);
}

void UOracleFishingComponent::Reel()
{
	GetWorld()->GetTimerManager().ClearTimer(WindowTimer);
	State = EOracleFishingState::Idle;

	UOracleFishDefinition* Fish = RollFish();
	AOracleCharacter* Character = Cast<AOracleCharacter>(GetOwner());
	if (!Fish || !Character)
	{
		return;
	}

	if (Fish->Item)
	{
		Character->GetInventory()->AddItem(Fish->Item, 1);  // dispara compêndio
	}
	Character->GetSkills()->AddXP(EOracleSkill::Pesca, Fish->FishingXP);

	const bool bRare = Fish->Rarity >= EOracleFishRarity::Raro;
	Toast(FText::Format(NSLOCTEXT("Oracle", "Caught", "🐟 Fisgou: {0}!"), Fish->DisplayName), bRare);
	OnFishCaught.Broadcast(Fish);
}

void UOracleFishingComponent::Cancel()
{
	GetWorld()->GetTimerManager().ClearTimer(BiteTimer);
	State = EOracleFishingState::Idle;
}

UOracleFishDefinition* UOracleFishingComponent::RollFish() const
{
	float Total = 0.f;
	for (const UOracleFishDefinition* Fish : FishPool)
	{
		if (Fish)
		{
			Total += Fish->CatchWeight;
		}
	}
	if (Total <= 0.f)
	{
		return nullptr;
	}

	float Roll = FMath::FRandRange(0.f, Total);
	for (UOracleFishDefinition* Fish : FishPool)
	{
		if (!Fish)
		{
			continue;
		}
		Roll -= Fish->CatchWeight;
		if (Roll <= 0.f)
		{
			return Fish;
		}
	}
	return FishPool.Last();
}

void UOracleFishingComponent::Toast(const FText& Text, const bool bGolden) const
{
	if (const AActor* Owner = GetOwner())
	{
		if (UOracleCollectionSubsystem* Toasts =
				Owner->GetGameInstance()->GetSubsystem<UOracleCollectionSubsystem>())
		{
			Toasts->AddToast(Text, bGolden);
		}
	}
}
