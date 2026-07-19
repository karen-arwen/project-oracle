// Project Oracle - Cozy Open World AAA

#include "Core/OraclePlayerController.h"

#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Input/OracleInputConfig.h"
#include "Input/OracleInputDefaults.h"
#include "Oracle.h"

const UOracleInputConfig* AOraclePlayerController::GetInputConfig()
{
	EnsureInputAssets();
	return InputConfig;
}

void AOraclePlayerController::EnsureInputAssets()
{
	if (!InputConfig || !DefaultMappingContext)
	{
		UE_LOG(LogOracle, Log, TEXT("InputConfig/MappingContext ausentes - usando padrao de runtime."));
		FOracleInputDefaults::Build(this, InputConfig, DefaultMappingContext);
	}
}

void AOraclePlayerController::BeginPlay()
{
	Super::BeginPlay();

	EnsureInputAssets();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, DefaultContextPriority);
	}
	else
	{
		UE_LOG(LogOracle, Error, TEXT("EnhancedInputLocalPlayerSubsystem indisponivel."));
	}
}
