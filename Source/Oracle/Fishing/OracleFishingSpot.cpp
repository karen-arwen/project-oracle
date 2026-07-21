// Project Oracle - Cozy Open World AAA

#include "Fishing/OracleFishingSpot.h"

#include "Character/OracleCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Fishing/OracleFishingComponent.h"

AOracleFishingSpot::AOracleFishingSpot()
{
	PrimaryActorTick.bCanEverTick = false;

	Marker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Marker"));
	SetRootComponent(Marker);
	Marker->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

FText AOracleFishingSpot::GetInteractionText_Implementation() const
{
	return NSLOCTEXT("Oracle", "FishHere", "Pescar");
}

bool AOracleFishingSpot::CanInteract_Implementation(AOracleCharacter* Interactor) const
{
	return Interactor != nullptr;
}

void AOracleFishingSpot::Interact_Implementation(AOracleCharacter* Interactor)
{
	if (!Interactor)
	{
		return;
	}
	UOracleFishingComponent* Fishing = Interactor->GetFishing();

	TArray<UOracleFishDefinition*> Pool;
	for (UOracleFishDefinition* Fish : FishPool)
	{
		Pool.Add(Fish);
	}
	Fishing->SetFishPool(Pool);
	Fishing->ToggleFishing();
}
