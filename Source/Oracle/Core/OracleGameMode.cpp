// Project Oracle - Cozy Open World AAA

#include "Core/OracleGameMode.h"

#include "Character/OracleCharacter.h"
#include "Core/OraclePlayerController.h"
#include "Demo/OracleDemoWorld.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "UI/OracleHUD.h"
#include "World/OracleEnvironment.h"

AOracleGameMode::AOracleGameMode()
{
	DefaultPawnClass = AOracleCharacter::StaticClass();
	PlayerControllerClass = AOraclePlayerController::StaticClass();
	HUDClass = AOracleHUD::StaticClass();
}

void AOracleGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Rig de céu/iluminação dinâmica (se o mapa ainda não tiver um).
	bool bEnvExists = false;
	for (TActorIterator<AOracleEnvironment> It(GetWorld()); It; ++It)
	{
		bEnvExists = true;
		break;
	}
	if (!bEnvExists)
	{
		GetWorld()->SpawnActor<AOracleEnvironment>(FVector::ZeroVector, FRotator::ZeroRotator);
	}

	bool bDemoExists = false;
	for (TActorIterator<AOracleDemoWorld> It(GetWorld()); It; ++It)
	{
		bDemoExists = true;
		break;
	}

	if (bSpawnDemoContent && !bDemoExists)
	{
		FVector Origin = FVector::ZeroVector;
		if (const APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0))
		{
			Origin = Pawn->GetActorLocation();
		}
		GetWorld()->SpawnActor<AOracleDemoWorld>(Origin, FRotator::ZeroRotator);
	}
}
