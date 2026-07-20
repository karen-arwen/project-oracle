// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "World/OracleWeatherSubsystem.h"
#include "OracleAudioSubsystem.generated.h"

class UAudioComponent;
class USoundBase;

/**
 * Diretor de áudio do mundo: mantém uma camada de AMBIENTE em loop
 * (pássaros de dia, grilos de noite, chuva quando chove) que faz
 * crossfade suave conforme hora e clima mudam — escutando os
 * subsystems de Tempo e Clima, nunca por polling.
 *
 * Os sons são Data-driven (setados no editor); sem asset, silêncio —
 * o jogo nunca quebra. Trilha musical dinâmica entra na v2 aqui mesmo.
 */
UCLASS()
class ORACLE_API UOracleAudioSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UOracleAudioSubsystem, STATGROUP_Tickables);
	}

	/** Define os sons de ambiente (chamado pelo mundo demo ou por Data Asset). */
	void ConfigureAmbience(USoundBase* Day, USoundBase* Night, USoundBase* Rain);

protected:
	UFUNCTION()
	void HandleHourChanged(int32 NewHour);

	UFUNCTION()
	void HandleWeatherChanged(EOracleWeather NewWeather);

private:
	void UpdateTargetAmbience();
	void PlayLayer(USoundBase* Sound);

	UPROPERTY() TObjectPtr<USoundBase> DaySound;
	UPROPERTY() TObjectPtr<USoundBase> NightSound;
	UPROPERTY() TObjectPtr<USoundBase> RainSound;

	UPROPERTY() TObjectPtr<UAudioComponent> AmbienceComp;
	UPROPERTY() TObjectPtr<USoundBase> CurrentSound;

	float TargetVolume = 1.f;
};
