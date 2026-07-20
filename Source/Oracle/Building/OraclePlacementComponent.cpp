// Project Oracle - Cozy Open World AAA

#include "Building/OraclePlacementComponent.h"

#include "Building/OraclePlacedProp.h"
#include "Camera/CameraComponent.h"
#include "Character/OracleCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Inventory/OracleInventoryComponent.h"
#include "Inventory/OracleItemDefinition.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Skills/OracleSkillsComponent.h"

UOraclePlacementComponent::UOraclePlacementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UOraclePlacementComponent::ToggleBuildMode()
{
	if (bActive)
	{
		ExitBuildMode();
	}
	else
	{
		EnterBuildMode();
	}
}

void UOraclePlacementComponent::EnterBuildMode()
{
	AOracleCharacter* Character = Cast<AOracleCharacter>(GetOwner());
	if (!Character)
	{
		return;
	}

	ActiveItem = Character->GetInventory()->FindFirstOfCategory(EOracleItemCategory::Furniture);
	if (!ActiveItem)
	{
		return;  // nada para posicionar; HUD orienta o jogador
	}

	if (!GhostMesh)
	{
		GhostMesh = NewObject<UStaticMeshComponent>(Character, TEXT("PlacementGhost"));
		GhostMesh->SetupAttachment(Character->GetRootComponent());
		GhostMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GhostMesh->RegisterComponent();
	}

	if (UStaticMesh* LoadedMesh = ActiveItem->WorldMesh.LoadSynchronous())
	{
		GhostMesh->SetStaticMesh(LoadedMesh);
	}
	GhostMesh->SetWorldScale3D(ActiveItem->WorldMeshScale);
	GhostMesh->SetVisibility(true);
	GhostYaw = 0.f;
	bActive = true;
}

void UOraclePlacementComponent::ExitBuildMode()
{
	bActive = false;
	ActiveItem = nullptr;
	if (GhostMesh)
	{
		GhostMesh->SetVisibility(false);
	}
}

void UOraclePlacementComponent::TickComponent(const float DeltaTime, const ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bActive)
	{
		bValidSpot = UpdateGhostTransform();

		// Verde = pode posicionar, vermelho = não.
		if (GhostMesh)
		{
			if (UMaterialInstanceDynamic* MID = GhostMesh->CreateAndSetMaterialInstanceDynamic(0))
			{
				MID->SetVectorParameterValue(TEXT("Color"), bValidSpot
					? FLinearColor(0.3f, 1.f, 0.4f) : FLinearColor(1.f, 0.3f, 0.3f));
			}
		}
	}
}

bool UOraclePlacementComponent::UpdateGhostTransform()
{
	AOracleCharacter* Character = Cast<AOracleCharacter>(GetOwner());
	if (!Character || !GhostMesh)
	{
		return false;
	}

	const UCameraComponent* Camera = Character->GetThirdPersonCamera();
	const FVector Start = Camera->GetComponentLocation();
	const FVector End = Start + Camera->GetForwardVector() * PlaceRange;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Character);
	const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

	// Precisa acertar chão razoavelmente plano.
	const bool bValid = bHit && Hit.ImpactNormal.Z > 0.85f;

	FVector Location = bHit ? Hit.ImpactPoint
	                        : Character->GetActorLocation() + Character->GetActorForwardVector() * 200.f;
	Location.X = FMath::GridSnap(Location.X, GridSize);
	Location.Y = FMath::GridSnap(Location.Y, GridSize);

	GhostMesh->SetWorldLocationAndRotation(Location, FRotator(0.f, GhostYaw, 0.f));
	return bValid;
}

void UOraclePlacementComponent::ConfirmPlacement()
{
	AOracleCharacter* Character = Cast<AOracleCharacter>(GetOwner());
	if (!bActive || !bValidSpot || !Character || !ActiveItem || !GhostMesh)
	{
		return;
	}

	if (Character->GetInventory()->RemoveItem(ActiveItem, 1) > 0)
	{
		AOraclePlacedProp* Prop = GetWorld()->SpawnActor<AOraclePlacedProp>(
			GhostMesh->GetComponentLocation(), GhostMesh->GetComponentRotation());
		if (Prop)
		{
			Prop->InitFromItem(ActiveItem);
		}
		Character->GetSkills()->AddXP(EOracleSkill::Construcao, 5);

		// Continua no modo se ainda houver itens; senão sai.
		if (Character->GetInventory()->CountOf(ActiveItem) == 0)
		{
			ExitBuildMode();
			EnterBuildMode();  // tenta o próximo móvel do inventário
		}
	}
}

void UOraclePlacementComponent::RotateGhost()
{
	if (bActive)
	{
		GhostYaw = FMath::Fmod(GhostYaw + 45.f, 360.f);
	}
}
