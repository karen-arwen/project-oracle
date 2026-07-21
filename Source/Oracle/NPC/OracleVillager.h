// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interaction/OracleInteractable.h"
#include "OracleVillager.generated.h"

/**
 * Aldeão da vila (NPC v1). Entrega três pilares sociais da visão:
 * - Diálogo: falas rotativas ao interagir [E].
 * - Amizade: cada conversa/presente sobe o nível de amizade (toast em marcos).
 * - Rotina: de dia caminha até a praça, de noite volta para casa
 *   (escutando o UOracleTimeSubsystem — sem polling).
 *
 * v2: memória por save, presentes favoritos, eventos e romance usam
 * esta mesma base (amizade já é o alicerce numérico).
 */
UCLASS()
class ORACLE_API AOracleVillager : public ACharacter, public IOracleInteractable
{
	GENERATED_BODY()

public:
	AOracleVillager();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	// IOracleInteractable
	virtual FText GetInteractionText_Implementation() const override;
	virtual bool CanInteract_Implementation(AOracleCharacter* Interactor) const override;
	virtual void Interact_Implementation(AOracleCharacter* Interactor) override;

	/** Configuração pelo mundo demo: nome, falas e pontos de rotina. */
	void Setup(const FText& InName, const TArray<FText>& InLines,
	           const FVector& InHomeSpot, const FVector& InDaySpot);

	UFUNCTION(BlueprintPure, Category = "Oracle|NPC")
	int32 GetFriendship() const { return Friendship; }

protected:
	UFUNCTION()
	void HandleHourChanged(int32 NewHour);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Oracle|NPC")
	FText VillagerName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Oracle|NPC")
	TArray<FText> DialogueLines;

private:
	void Toast(const FText& Text, bool bGolden) const;

	FVector HomeSpot = FVector::ZeroVector;
	FVector DaySpot = FVector::ZeroVector;
	FVector TargetSpot = FVector::ZeroVector;

	int32 Friendship = 0;
	int32 NextLine = 0;
};
