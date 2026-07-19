// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OracleSaveGame.generated.h"

USTRUCT()
struct FOracleSavedItem
{
	GENERATED_BODY()

	UPROPERTY() FString ItemName;
	UPROPERTY() int32 Count = 0;
};

USTRUCT()
struct FOracleSavedPlot
{
	GENERATED_BODY()

	UPROPERTY() FString PlotName;
	UPROPERTY() uint8 State = 0;
	UPROPERTY() FString CropName;
	UPROPERTY() int32 DaysGrown = 0;
	UPROPERTY() bool bWatered = false;
};

USTRUCT()
struct FOracleSavedProp
{
	GENERATED_BODY()

	UPROPERTY() FString ItemName;
	UPROPERTY() FTransform Transform;
};

/**
 * Dados persistidos. Versionado desde o início: mudou o formato,
 * incrementa Version e trata a migração no load.
 * Itens/cultivos são salvos por NOME e resolvidos no load — funciona
 * tanto para Data Assets quanto para o conteúdo demo criado em runtime.
 */
UCLASS()
class ORACLE_API UOracleSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	static constexpr int32 CurrentVersion = 1;

	UPROPERTY() int32 Version = CurrentVersion;

	UPROPERTY() FTransform PlayerTransform;
	UPROPERTY() int32 DayNumber = 1;
	UPROPERTY() float TimeOfDayHours = 9.f;

	UPROPERTY() TArray<FOracleSavedItem> Inventory;
	UPROPERTY() TArray<FOracleSavedPlot> FarmPlots;
	UPROPERTY() TArray<FOracleSavedProp> PlacedProps;
};
