// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OracleSkillsComponent.generated.h"

/** Habilidades que evoluem PELO USO — pilar "sem classes" do Oracle. */
UENUM(BlueprintType)
enum class EOracleSkill : uint8
{
	Coleta,
	Fazenda,
	Artesanato,
	Construcao,
	Exploracao,
	Pesca
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOracleSkillLevelUp, EOracleSkill, Skill, int32, NewLevel);

/**
 * Progressão por uso: coletar sobe Coleta, plantar sobe Fazenda,
 * criar sobe Artesanato, decorar sobe Construção. Cada level up é
 * celebrado com toast dourado. Curva v1 linear (100 XP por nível) —
 * será curva exponencial configurável quando houver benefícios por nível.
 */
UCLASS(ClassGroup = (Oracle), meta = (BlueprintSpawnableComponent))
class ORACLE_API UOracleSkillsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Oracle|Skills")
	void AddXP(EOracleSkill Skill, int32 Amount);

	UFUNCTION(BlueprintPure, Category = "Oracle|Skills")
	int32 GetLevel(EOracleSkill Skill) const;

	UFUNCTION(BlueprintPure, Category = "Oracle|Skills")
	int32 GetXP(EOracleSkill Skill) const;

	static FText GetSkillName(EOracleSkill Skill);

	/** Persistência (SaveSubsystem). */
	const TMap<EOracleSkill, int32>& GetAllXP() const { return XP; }
	void RestoreFromSave(const TMap<EOracleSkill, int32>& InXP) { XP = InXP; }

	UPROPERTY(BlueprintAssignable, Category = "Oracle|Skills")
	FOracleSkillLevelUp OnLevelUp;

	/** XP necessário por nível (curva v1: linear). */
	static constexpr int32 XPPerLevel = 100;

private:
	UPROPERTY()
	TMap<EOracleSkill, int32> XP;
};
