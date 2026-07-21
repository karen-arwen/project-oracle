// Project Oracle - Cozy Open World AAA

#include "Skills/OracleSkillsComponent.h"

#include "Collections/OracleCollectionSubsystem.h"
#include "Engine/GameInstance.h"
#include "GameFramework/Actor.h"

void UOracleSkillsComponent::AddXP(const EOracleSkill Skill, const int32 Amount)
{
	if (Amount <= 0)
	{
		return;
	}

	const int32 OldLevel = GetLevel(Skill);
	XP.FindOrAdd(Skill) += Amount;
	const int32 NewLevel = GetLevel(Skill);

	if (NewLevel > OldLevel)
	{
		OnLevelUp.Broadcast(Skill, NewLevel);

		if (const AActor* Owner = GetOwner())
		{
			if (UOracleCollectionSubsystem* Toasts =
					Owner->GetGameInstance()->GetSubsystem<UOracleCollectionSubsystem>())
			{
				Toasts->AddToast(FText::Format(
					NSLOCTEXT("Oracle", "LevelUpToast", "⬆ {0} subiu para o nível {1}!"),
					GetSkillName(Skill), NewLevel), /*bGolden=*/true);
			}
		}
	}
}

int32 UOracleSkillsComponent::GetXP(const EOracleSkill Skill) const
{
	const int32* Found = XP.Find(Skill);
	return Found ? *Found : 0;
}

int32 UOracleSkillsComponent::GetLevel(const EOracleSkill Skill) const
{
	return 1 + GetXP(Skill) / XPPerLevel;
}

FText UOracleSkillsComponent::GetSkillName(const EOracleSkill Skill)
{
	switch (Skill)
	{
	case EOracleSkill::Coleta:     return NSLOCTEXT("Oracle", "SkGather", "Coleta");
	case EOracleSkill::Fazenda:    return NSLOCTEXT("Oracle", "SkFarm", "Fazenda");
	case EOracleSkill::Artesanato: return NSLOCTEXT("Oracle", "SkCraft", "Artesanato");
	case EOracleSkill::Construcao: return NSLOCTEXT("Oracle", "SkBuild", "Construção");
	case EOracleSkill::Exploracao: return NSLOCTEXT("Oracle", "SkExplore", "Exploração");
	case EOracleSkill::Pesca:      return NSLOCTEXT("Oracle", "SkFish", "Pesca");
	}
	return FText::GetEmpty();
}
