// Project Oracle - Cozy Open World AAA

#include "Core/OracleSaveSubsystem.h"

#include "Building/OraclePlacedProp.h"
#include "Character/OracleCharacter.h"
#include "Collections/OracleCollectionSubsystem.h"
#include "Core/OracleSaveGame.h"
#include "Core/OracleTimeSubsystem.h"
#include "EngineUtils.h"
#include "Farming/OracleCropDefinition.h"
#include "Farming/OracleFarmPlot.h"
#include "Inventory/OracleInventoryComponent.h"
#include "Inventory/OracleItemDefinition.h"
#include "Kismet/GameplayStatics.h"
#include "Oracle.h"
#include "UObject/UObjectIterator.h"

const TCHAR* UOracleSaveSubsystem::SlotName = TEXT("Oracle_Slot1");

bool UOracleSaveSubsystem::SaveWorld()
{
	UWorld* World = GetGameInstance()->GetWorld();
	AOracleCharacter* Player = Cast<AOracleCharacter>(UGameplayStatics::GetPlayerPawn(World, 0));
	if (!World || !Player)
	{
		return false;
	}

	UOracleSaveGame* Save = Cast<UOracleSaveGame>(
		UGameplayStatics::CreateSaveGameObject(UOracleSaveGame::StaticClass()));

	Save->PlayerTransform = Player->GetActorTransform();

	if (const UOracleTimeSubsystem* Time = World->GetSubsystem<UOracleTimeSubsystem>())
	{
		Save->DayNumber = Time->GetDayNumber();
		Save->TimeOfDayHours = Time->GetTimeOfDayHours();
	}

	for (const FOracleInventoryEntry& Entry : Player->GetInventory()->GetEntries())
	{
		if (Entry.Item)
		{
			Save->Inventory.Add({Entry.Item->GetName(), Entry.Count});
		}
	}

	for (TActorIterator<AOracleFarmPlot> It(World); It; ++It)
	{
		FOracleSavedPlot Plot;
		Plot.PlotName = It->GetName();
		Plot.State = static_cast<uint8>(It->GetPlotState());
		Plot.CropName = It->GetCrop() ? It->GetCrop()->GetName() : FString();
		Plot.DaysGrown = It->GetDaysGrown();
		Plot.bWatered = It->IsWateredToday();
		Save->FarmPlots.Add(Plot);
	}

	for (TActorIterator<AOraclePlacedProp> It(World); It; ++It)
	{
		if (It->GetSourceItem())
		{
			Save->PlacedProps.Add({It->GetSourceItem()->GetName(), It->GetActorTransform()});
		}
	}

	if (const UOracleCollectionSubsystem* Collection =
			GetGameInstance()->GetSubsystem<UOracleCollectionSubsystem>())
	{
		Save->DiscoveredItems = Collection->GetDiscoveredForSave();
	}

	const bool bOk = UGameplayStatics::SaveGameToSlot(Save, SlotName, 0);
	UE_LOG(LogOracle, Log, TEXT("SaveWorld: %s"), bOk ? TEXT("ok") : TEXT("FALHOU"));
	return bOk;
}

bool UOracleSaveSubsystem::LoadWorld()
{
	UWorld* World = GetGameInstance()->GetWorld();
	AOracleCharacter* Player = Cast<AOracleCharacter>(UGameplayStatics::GetPlayerPawn(World, 0));
	if (!World || !Player || !UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		return false;
	}

	const UOracleSaveGame* Save = Cast<UOracleSaveGame>(
		UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	if (!Save)
	{
		return false;
	}

	Player->SetActorTransform(Save->PlayerTransform, false, nullptr, ETeleportType::TeleportPhysics);

	if (UOracleTimeSubsystem* Time = World->GetSubsystem<UOracleTimeSubsystem>())
	{
		Time->SetTime(Save->DayNumber, Save->TimeOfDayHours);
	}

	TArray<FOracleInventoryEntry> Entries;
	for (const FOracleSavedItem& Saved : Save->Inventory)
	{
		if (UOracleItemDefinition* Item = FindItemByName(Saved.ItemName))
		{
			Entries.Add({Item, Saved.Count});
		}
	}
	Player->GetInventory()->SetEntries(Entries);

	for (TActorIterator<AOracleFarmPlot> It(World); It; ++It)
	{
		for (const FOracleSavedPlot& Saved : Save->FarmPlots)
		{
			if (Saved.PlotName == It->GetName())
			{
				It->RestoreState(static_cast<EOraclePlotState>(Saved.State),
					FindCropByName(Saved.CropName), Saved.DaysGrown, Saved.bWatered);
				break;
			}
		}
	}

	// Móveis: remove os atuais e recria a partir do save.
	for (TActorIterator<AOraclePlacedProp> It(World); It; ++It)
	{
		It->Destroy();
	}
	for (const FOracleSavedProp& Saved : Save->PlacedProps)
	{
		if (UOracleItemDefinition* Item = FindItemByName(Saved.ItemName))
		{
			AOraclePlacedProp* Prop = World->SpawnActor<AOraclePlacedProp>(
				Saved.Transform.GetLocation(), Saved.Transform.Rotator());
			if (Prop)
			{
				Prop->InitFromItem(Item);
			}
		}
	}

	if (UOracleCollectionSubsystem* Collection =
			GetGameInstance()->GetSubsystem<UOracleCollectionSubsystem>())
	{
		Collection->RestoreFromSave(Save->DiscoveredItems);
	}

	UE_LOG(LogOracle, Log, TEXT("LoadWorld: ok"));
	return true;
}

UOracleItemDefinition* UOracleSaveSubsystem::FindItemByName(const FString& Name)
{
	if (Name.IsEmpty())
	{
		return nullptr;
	}
	for (TObjectIterator<UOracleItemDefinition> It; It; ++It)
	{
		if (It->GetName() == Name)
		{
			return *It;
		}
	}
	return nullptr;
}

UOracleCropDefinition* UOracleSaveSubsystem::FindCropByName(const FString& Name)
{
	if (Name.IsEmpty())
	{
		return nullptr;
	}
	for (TObjectIterator<UOracleCropDefinition> It; It; ++It)
	{
		if (It->GetName() == Name)
		{
			return *It;
		}
	}
	return nullptr;
}
