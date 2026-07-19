// Project Oracle - Cozy Open World AAA

#include "Building/OraclePlacedProp.h"

#include "Character/OracleCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Inventory/OracleInventoryComponent.h"
#include "Inventory/OracleItemDefinition.h"

AOraclePlacedProp::AOraclePlacedProp()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

void AOraclePlacedProp::InitFromItem(UOracleItemDefinition* Item)
{
	SourceItem = Item;
	if (Item)
	{
		if (UStaticMesh* LoadedMesh = Item->WorldMesh.LoadSynchronous())
		{
			Mesh->SetStaticMesh(LoadedMesh);
		}
		Mesh->SetRelativeScale3D(Item->WorldMeshScale);
	}
}

FText AOraclePlacedProp::GetInteractionText_Implementation() const
{
	return FText::Format(NSLOCTEXT("Oracle", "StowProp", "Guardar {0}"),
		SourceItem ? SourceItem->DisplayName : FText::GetEmpty());
}

bool AOraclePlacedProp::CanInteract_Implementation(AOracleCharacter* Interactor) const
{
	return SourceItem != nullptr;
}

void AOraclePlacedProp::Interact_Implementation(AOracleCharacter* Interactor)
{
	if (Interactor && SourceItem)
	{
		Interactor->GetInventory()->AddItem(SourceItem, 1);
		Destroy();
	}
}
