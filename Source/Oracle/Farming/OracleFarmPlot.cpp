// Project Oracle - Cozy Open World AAA

#include "Farming/OracleFarmPlot.h"

#include "Character/OracleCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Core/OracleTimeSubsystem.h"
#include "Farming/OracleCropDefinition.h"
#include "Inventory/OracleInventoryComponent.h"
#include "Inventory/OracleItemDefinition.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

AOracleFarmPlot::AOracleFarmPlot()
{
	PrimaryActorTick.bCanEverTick = false;

	// Placeholders com formas da engine: visível e jogável sem nenhum asset.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));

	SoilMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Soil"));
	SetRootComponent(SoilMesh);
	if (CubeMesh.Succeeded())
	{
		SoilMesh->SetStaticMesh(CubeMesh.Object);
	}
	SoilMesh->SetRelativeScale3D(FVector(1.4f, 1.4f, 0.12f));
	SoilMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	CropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Crop"));
	CropMesh->SetupAttachment(SoilMesh);
	// Broto bonito: abóbora do pack de vila; fallback esfera da engine.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PumpkinMesh(TEXT(
		"/Game/StylizedIsland/Mesh/Props/SM_Food/SM_Stylized_Food_Mesh/SM_Stylized_Food_Pumpkin.SM_Stylized_Food_Pumpkin"));
	if (PumpkinMesh.Succeeded())
	{
		CropMesh->SetStaticMesh(PumpkinMesh.Object);
		bUsingPrettyCrop = true;
	}
	else if (SphereMesh.Succeeded())
	{
		CropMesh->SetStaticMesh(SphereMesh.Object);
	}
	CropMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CropMesh->SetVisibility(false);
}

void AOracleFarmPlot::BeginPlay()
{
	Super::BeginPlay();

	if (UOracleTimeSubsystem* Time = GetWorld()->GetSubsystem<UOracleTimeSubsystem>())
	{
		Time->OnDayChanged.AddDynamic(this, &AOracleFarmPlot::HandleDayChanged);
	}

	// Terra marrom (se o material da engine expuser o parâmetro "Color").
	if (UMaterialInstanceDynamic* MID = SoilMesh->CreateAndSetMaterialInstanceDynamic(0))
	{
		MID->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.25f, 0.15f, 0.08f));
	}
	UpdateVisuals();
}

FText AOracleFarmPlot::GetInteractionText_Implementation() const
{
	switch (State)
	{
	case EOraclePlotState::Empty:
		return NSLOCTEXT("Oracle", "PlotPlant", "Plantar semente");
	case EOraclePlotState::Planted:
		return bWateredToday
			? FText::Format(NSLOCTEXT("Oracle", "PlotGrowing", "{0} crescendo (regado)"),
				Crop ? Crop->DisplayName : FText::GetEmpty())
			: NSLOCTEXT("Oracle", "PlotWater", "Regar");
	case EOraclePlotState::Ready:
		return FText::Format(NSLOCTEXT("Oracle", "PlotHarvest", "Colher {0}"),
			Crop ? Crop->DisplayName : FText::GetEmpty());
	}
	return FText::GetEmpty();
}

bool AOracleFarmPlot::CanInteract_Implementation(AOracleCharacter* Interactor) const
{
	if (State == EOraclePlotState::Empty)
	{
		// Só oferece plantar se houver semente no inventário.
		return Interactor && Interactor->GetInventory()->FindFirstOfCategory(EOracleItemCategory::Seed) != nullptr;
	}
	if (State == EOraclePlotState::Planted)
	{
		return !bWateredToday;
	}
	return true;  // Ready
}

void AOracleFarmPlot::Interact_Implementation(AOracleCharacter* Interactor)
{
	if (!Interactor)
	{
		return;
	}
	UOracleInventoryComponent* Inventory = Interactor->GetInventory();

	switch (State)
	{
	case EOraclePlotState::Empty:
	{
		UOracleItemDefinition* Seed = Inventory->FindFirstOfCategory(EOracleItemCategory::Seed);
		if (Seed && Seed->Crop && Inventory->RemoveItem(Seed, 1) > 0)
		{
			Crop = Seed->Crop;
			DaysGrown = 0;
			bWateredToday = false;
			State = EOraclePlotState::Planted;
		}
		break;
	}
	case EOraclePlotState::Planted:
		bWateredToday = true;
		break;

	case EOraclePlotState::Ready:
		if (Crop && Crop->Produce)
		{
			Inventory->AddItem(Crop->Produce, Crop->ProduceAmount);
		}
		Crop = nullptr;
		DaysGrown = 0;
		bWateredToday = false;
		State = EOraclePlotState::Empty;
		break;
	}

	UpdateVisuals();
}

void AOracleFarmPlot::HandleDayChanged(int32 /*NewDay*/)
{
	if (State == EOraclePlotState::Planted && Crop)
	{
		if (bWateredToday)
		{
			++DaysGrown;
			if (DaysGrown >= Crop->GrowthDays)
			{
				State = EOraclePlotState::Ready;
			}
		}
		bWateredToday = false;
		UpdateVisuals();
	}
}

void AOracleFarmPlot::RestoreState(const EOraclePlotState InState, UOracleCropDefinition* InCrop,
                                   const int32 InDaysGrown, const bool bInWatered)
{
	State = InState;
	Crop = InCrop;
	DaysGrown = InDaysGrown;
	bWateredToday = bInWatered;
	UpdateVisuals();
}

void AOracleFarmPlot::UpdateVisuals()
{
	const bool bShowCrop = State != EOraclePlotState::Empty && Crop;
	CropMesh->SetVisibility(bShowCrop);

	if (bShowCrop)
	{
		const float Progress = FMath::Clamp(
			static_cast<float>(DaysGrown) / FMath::Max(1, Crop->GrowthDays), 0.f, 1.f);
		// Maduro ganha um "pulo" de escala — colheita implora para ser notada.
		const float Ripe = State == EOraclePlotState::Ready ? 1.25f : 1.f;
		const float Scale = FMath::Lerp(0.3f, 1.f, Progress) * Ripe;
		// Compensa o achatamento do solo para o broto não nascer esmagado.
		CropMesh->SetRelativeScale3D(FVector(Scale, Scale, Scale / 0.12f));
		CropMesh->SetRelativeLocation(FVector(0.f, 0.f, 55.f));

		if (!bUsingPrettyCrop)
		{
			if (UMaterialInstanceDynamic* MID = CropMesh->CreateAndSetMaterialInstanceDynamic(0))
			{
				const FLinearColor Color = State == EOraclePlotState::Ready
					? FLinearColor(0.9f, 0.25f, 0.2f)
					: FLinearColor(0.2f, 0.7f, 0.25f);
				MID->SetVectorParameterValue(TEXT("Color"), Color);
			}
		}
	}
}
