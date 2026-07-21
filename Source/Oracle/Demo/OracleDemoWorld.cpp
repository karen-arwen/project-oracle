// Project Oracle - Cozy Open World AAA

#include "Demo/OracleDemoWorld.h"

#include "Character/OracleCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Crafting/OracleCraftingComponent.h"
#include "Crafting/OracleRecipeDefinition.h"
#include "Economy/OracleShop.h"
#include "Fishing/OracleFishDefinition.h"
#include "Fishing/OracleFishingSpot.h"
#include "Quests/OracleQuestComponent.h"
#include "Quests/OracleQuestDefinition.h"
#include "Farming/OracleCropDefinition.h"
#include "Farming/OracleFarmPlot.h"
#include "Inventory/OracleInventoryComponent.h"
#include "Inventory/OracleItemDefinition.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "World/OracleGatherable.h"

namespace
{
	// Props do pack Stylized Village (fallback: formas da engine se ausente).
	const TCHAR* PumpkinMesh = TEXT("/Game/StylizedIsland/Mesh/Props/SM_Food/SM_Stylized_Food_Mesh/SM_Stylized_Food_Pumpkin.SM_Stylized_Food_Pumpkin");
	const TCHAR* TrunkMesh   = TEXT("/Game/StylizedIsland/Mesh/Props/SM_Stylized_Stump/SM_Stylized_Stump/SM_TrunkTree.SM_TrunkTree");
	const TCHAR* ChairMesh   = TEXT("/Game/StylizedIsland/Mesh/Props/SM_Stylized_Chair/SM_Stylized_Chair_mesh/SM_Stylized_Chair.SM_Stylized_Chair");
	const TCHAR* LampMesh    = TEXT("/Game/StylizedIsland/Mesh/Props/SM_Stylized_Lamp/SM_Stylized_Lamp_Mesh/SM_Stylized_Lamp_A.SM_Stylized_Lamp_A");
	const TCHAR* MushroomMesh= TEXT("/Game/StylizedIsland/Mesh/Props/SM_Mushrooms/SM_Stylized_Big_Mushroom_01/SM_Stylized_Big_Mushroom_Mesh/SM_Stylized_Big_Mushroom_01.SM_Stylized_Big_Mushroom_01");

	UStaticMesh* LoadMeshOr(const TCHAR* Path, const TCHAR* FallbackPath)
	{
		UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, Path);
		return Mesh ? Mesh : LoadObject<UStaticMesh>(nullptr, FallbackPath);
	}

	UOracleItemDefinition* MakeItem(UObject* Outer, const FName Name, const FText& Display,
		const EOracleItemCategory Category, const TCHAR* MeshPath, const FVector& Scale)
	{
		UOracleItemDefinition* Item = NewObject<UOracleItemDefinition>(Outer, Name);
		Item->DisplayName = Display;
		Item->Category = Category;
		Item->WorldMesh = TSoftObjectPtr<UStaticMesh>(FSoftObjectPath(MeshPath));
		Item->WorldMeshScale = Scale;
		return Item;
	}

}

void AOracleDemoWorld::BeginPlay()
{
	Super::BeginPlay();
	CreateDemoDefinitions();
	SpawnWorldContent();
	GiveStartingItems();
}

void AOracleDemoWorld::CreateDemoDefinitions()
{
	const TCHAR* Sphere = TEXT("/Engine/BasicShapes/Sphere.Sphere");

	BerryItem = MakeItem(this, TEXT("Item_Fruta"),
		NSLOCTEXT("OracleDemo", "Berry", "Abóbora Estrela"),
		EOracleItemCategory::Food, PumpkinMesh, FVector(0.6f));

	SeedItem = MakeItem(this, TEXT("Item_Semente"),
		NSLOCTEXT("OracleDemo", "Seed", "Semente de Abóbora Estrela"),
		EOracleItemCategory::Seed, Sphere, FVector(0.15f));

	WoodItem = MakeItem(this, TEXT("Item_Madeira"),
		NSLOCTEXT("OracleDemo", "Wood", "Madeira"),
		EOracleItemCategory::Resource, TrunkMesh, FVector(0.5f));

	ChairItem = MakeItem(this, TEXT("Item_Banquinho"),
		NSLOCTEXT("OracleDemo", "Chair", "Cadeirinha da Vila"),
		EOracleItemCategory::Furniture, ChairMesh, FVector(1.f));

	LampItem = MakeItem(this, TEXT("Item_Poste"),
		NSLOCTEXT("OracleDemo", "Lamp", "Lampião de Jardim"),
		EOracleItemCategory::Furniture, LampMesh, FVector(1.f));

	PieItem = MakeItem(this, TEXT("Item_Torta"),
		NSLOCTEXT("OracleDemo", "Pie", "Torta de Abóbora"),
		EOracleItemCategory::Food,
		TEXT("/Game/StylizedIsland/Mesh/Props/SM_Food/SM_Stylized_Food_Mesh/SM_Stylized_Food_Bread.SM_Stylized_Food_Bread"),
		FVector(0.7f));

	// Receitas demo: madeira vira móvel; abóboras viram torta (culinária!).
	ChairRecipe = NewObject<UOracleRecipeDefinition>(this, TEXT("Recipe_Cadeirinha"));
	ChairRecipe->DisplayName = NSLOCTEXT("OracleDemo", "RChair", "Cadeirinha da Vila");
	ChairRecipe->Ingredients = {{WoodItem, 3}};
	ChairRecipe->Result = ChairItem;

	PieRecipe = NewObject<UOracleRecipeDefinition>(this, TEXT("Recipe_Torta"));
	PieRecipe->DisplayName = NSLOCTEXT("OracleDemo", "RPie", "Torta de Abóbora");
	PieRecipe->Ingredients = {{BerryItem, 2}};
	PieRecipe->Result = PieItem;

	// Missões demo em sequência — o jogador nunca fica sem objetivo.
	QuestWood = NewObject<UOracleQuestDefinition>(this, TEXT("Quest_Madeira"));
	QuestWood->Title = NSLOCTEXT("OracleDemo", "QWoodT", "Coletor Iniciante");
	QuestWood->Description = NSLOCTEXT("OracleDemo", "QWoodD", "A vila precisa de madeira. Junte 3 troncos!");
	QuestWood->TargetItem = WoodItem;
	QuestWood->TargetCount = 3;
	QuestWood->bConsumeItems = false;  // primeira missão gentil: não tira os itens
	QuestWood->RewardCoins = 50;

	QuestBerries = NewObject<UOracleQuestDefinition>(this, TEXT("Quest_Abobora"));
	QuestBerries->Title = NSLOCTEXT("OracleDemo", "QBerryT", "Colheita Estrelada");
	QuestBerries->Description = NSLOCTEXT("OracleDemo", "QBerryD", "Traga 4 Abóboras Estrela para o festival.");
	QuestBerries->TargetItem = BerryItem;
	QuestBerries->TargetCount = 4;
	QuestBerries->RewardCoins = 120;
	QuestBerries->RewardItem = SeedItem;
	QuestBerries->RewardItemCount = 3;

	// Peixes demo (comum + raro) e seus itens de coleção.
	FishCommonItem = MakeItem(this, TEXT("Item_Peixinho"),
		NSLOCTEXT("OracleDemo", "FishC", "Peixe-Lua Comum"),
		EOracleItemCategory::Food,
		TEXT("/Game/StylizedIsland/Mesh/Props/SM_Food/SM_Stylized_Food_Mesh/SM_Stylized_Food_Fish.SM_Stylized_Food_Fish"),
		FVector(0.5f));
	FishCommonItem->BaseValue = 8;

	FishRareItem = MakeItem(this, TEXT("Item_PeixeDourado"),
		NSLOCTEXT("OracleDemo", "FishR", "Peixe Dourado Estelar"),
		EOracleItemCategory::Food,
		TEXT("/Game/StylizedIsland/Mesh/Props/SM_Food/SM_Stylized_Food_Mesh/SM_Stylized_Food_Fish.SM_Stylized_Food_Fish"),
		FVector(0.7f));
	FishRareItem->BaseValue = 60;

	{
		UOracleFishDefinition* FishC = NewObject<UOracleFishDefinition>(this, TEXT("Fish_Comum"));
		FishC->DisplayName = FishCommonItem->DisplayName;
		FishC->Rarity = EOracleFishRarity::Comum;
		FishC->CatchWeight = 4.f;
		FishC->Item = FishCommonItem;
		FishC->FishingXP = 10;

		UOracleFishDefinition* FishR = NewObject<UOracleFishDefinition>(this, TEXT("Fish_Raro"));
		FishR->DisplayName = FishRareItem->DisplayName;
		FishR->Rarity = EOracleFishRarity::Lendario;
		FishR->CatchWeight = 1.f;
		FishR->Item = FishRareItem;
		FishR->FishingXP = 40;

		DemoFish = {FishC, FishR};
	}

	BerryCrop = NewObject<UOracleCropDefinition>(this, TEXT("Crop_FrutaEstrela"));
	BerryCrop->DisplayName = NSLOCTEXT("OracleDemo", "BerryCrop", "Fruta Estrela");
	BerryCrop->GrowthDays = 2;
	BerryCrop->Produce = BerryItem;
	BerryCrop->ProduceAmount = 3;
	SeedItem->Crop = BerryCrop;
}

void AOracleDemoWorld::SpawnWorldContent()
{
	UWorld* World = GetWorld();
	const FVector Origin = GetActorLocation();
	UStaticMesh* BushMesh = LoadMeshOr(MushroomMesh, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	UStaticMesh* LogMesh = LoadMeshOr(TrunkMesh, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));

	// Cogumelos coletáveis em arco à frente.
	for (int32 i = 0; i < 6; ++i)
	{
		const float Angle = -60.f + i * 24.f;
		const FVector Pos = Origin + FRotator(0.f, Angle, 0.f).Vector() * 700.f + FVector(0, 0, 10.f);
		if (AOracleGatherable* Bush = World->SpawnActor<AOracleGatherable>(Pos, FRotator::ZeroRotator))
		{
			Bush->Configure(BerryItem, 2, 45.f);
			if (BushMesh)
			{
				Bush->Mesh->SetStaticMesh(BushMesh);
				Bush->Mesh->SetWorldScale3D(FVector(0.9f));
			}
		}
	}

	// Troncos de madeira à direita.
	for (int32 i = 0; i < 4; ++i)
	{
		const FVector Pos = Origin + FVector(300.f + i * 180.f, 900.f, 10.f);
		if (AOracleGatherable* Log = World->SpawnActor<AOracleGatherable>(Pos, FRotator::ZeroRotator))
		{
			Log->Configure(WoodItem, 1, 60.f);
			if (LogMesh)
			{
				Log->Mesh->SetStaticMesh(LogMesh);
				Log->Mesh->SetWorldScale3D(FVector(0.7f));
			}
		}
	}

	// Fazendinha: grade 2x3 de canteiros à esquerda.
	for (int32 Row = 0; Row < 2; ++Row)
	{
		for (int32 Col = 0; Col < 3; ++Col)
		{
			const FVector Pos = Origin + FVector(500.f + Row * 220.f, -900.f + Col * 220.f, 8.f);
			World->SpawnActor<AOracleFarmPlot>(Pos, FRotator::ZeroRotator);
		}
	}

	// Pesqueiro à esquerda do spawn (perto da água da vila).
	if (AOracleFishingSpot* Spot = World->SpawnActor<AOracleFishingSpot>(
			Origin + FVector(-200.f, 400.f, 20.f), FRotator::ZeroRotator))
	{
		TArray<UOracleFishDefinition*> Pool;
		for (UOracleFishDefinition* Fish : DemoFish)
		{
			Pool.Add(Fish);
		}
		Spot->SetFishPool(Pool);
		if (UStaticMesh* Sphere = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder.Cylinder")))
		{
			Spot->Marker->SetStaticMesh(Sphere);
			Spot->Marker->SetWorldScale3D(FVector(0.4f, 0.4f, 0.1f));
		}
	}

	// Banca de vendas (barril do pack) logo à frente do spawn.
	if (AOracleShop* Shop = World->SpawnActor<AOracleShop>(
			Origin + FVector(250.f, -250.f, 40.f), FRotator::ZeroRotator))
	{
		if (UStaticMesh* Barrel = LoadObject<UStaticMesh>(nullptr,
				TEXT("/Game/StylizedIsland/Mesh/Props/SM_Barrel/SM_Stylized_Barrel_Mesh/_Stylized_Barrel._Stylized_Barrel")))
		{
			Shop->Mesh->SetStaticMesh(Barrel);
			Shop->Mesh->SetWorldScale3D(FVector(1.5f));
		}
	}
}

void AOracleDemoWorld::GiveStartingItems()
{
	if (const AOracleCharacter* Player =
			Cast<AOracleCharacter>(UGameplayStatics::GetPlayerPawn(this, 0)))
	{
		UOracleInventoryComponent* Inventory = Player->GetInventory();
		Inventory->AddItem(SeedItem, 4);
		Inventory->AddItem(ChairItem, 3);
		Inventory->AddItem(LampItem, 2);

		Player->GetCrafting()->LearnRecipe(ChairRecipe);
		Player->GetCrafting()->LearnRecipe(PieRecipe);

		UOracleQuestComponent* Q = Player->GetQuests();
		Q->QueueQuest(QuestWood);
		Q->QueueQuest(QuestBerries);
	}
}
