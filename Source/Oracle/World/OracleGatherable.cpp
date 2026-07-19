// Project Oracle - Cozy Open World AAA

#include "World/OracleGatherable.h"

#include "Character/OracleCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Inventory/OracleInventoryComponent.h"
#include "Inventory/OracleItemDefinition.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "TimerManager.h"

AOracleGatherable::AOracleGatherable()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

void AOracleGatherable::Configure(UOracleItemDefinition* InItem, const int32 InAmount,
                                  const float InRespawnSeconds)
{
	ItemToGive = InItem;
	Amount = InAmount;
	RespawnSeconds = InRespawnSeconds;
}

FText AOracleGatherable::GetInteractionText_Implementation() const
{
	return ItemToGive
		? FText::Format(NSLOCTEXT("Oracle", "GatherPrompt", "Coletar {0}"), ItemToGive->DisplayName)
		: NSLOCTEXT("Oracle", "GatherPromptGeneric", "Coletar");
}

bool AOracleGatherable::CanInteract_Implementation(AOracleCharacter* Interactor) const
{
	return !bIsGathered && ItemToGive != nullptr;
}

void AOracleGatherable::Interact_Implementation(AOracleCharacter* Interactor)
{
	if (bIsGathered || !ItemToGive || !Interactor)
	{
		return;
	}

	Interactor->GetInventory()->AddItem(ItemToGive, Amount);

	if (GatherSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, GatherSound, GetActorLocation());
	}
	if (GatherVFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, GatherVFX, GetActorLocation());
	}

	SetGathered(true);
	if (RespawnSeconds > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this,
			&AOracleGatherable::Respawn, RespawnSeconds, false);
	}
}

void AOracleGatherable::SetGathered(const bool bGathered)
{
	bIsGathered = bGathered;
	SetActorHiddenInGame(bGathered);
	SetActorEnableCollision(!bGathered);
}

void AOracleGatherable::Respawn()
{
	SetGathered(false);
}
