// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "OracleTimeSubsystem.generated.h"

class ADirectionalLight;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOracleDayChanged, int32, NewDay);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOracleHourChanged, int32, NewHour);

/**
 * Relógio do mundo: hora do dia, contagem de dias, e rotação do sol.
 * WorldSubsystem = existe um por mundo, sem precisar colocar ator no mapa.
 * Tudo que depende do tempo (cultivos, rotina de NPCs, lojas, eventos)
 * escuta OnDayChanged/OnHourChanged — nunca faz polling.
 */
UCLASS()
class ORACLE_API UOracleTimeSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UOracleTimeSubsystem, STATGROUP_Tickables);
	}

	UFUNCTION(BlueprintPure, Category = "Oracle|Time")
	float GetTimeOfDayHours() const { return TimeOfDayHours; }

	UFUNCTION(BlueprintPure, Category = "Oracle|Time")
	int32 GetDayNumber() const { return DayNumber; }

	/** Ex.: "Dia 3 — 14:35". */
	UFUNCTION(BlueprintPure, Category = "Oracle|Time")
	FString GetClockText() const;

	/** Usado pelo Load. */
	void SetTime(int32 InDay, float InHours);

	/**
	 * Quem controla o sol. Se um AOracleEnvironment existir, ele assume
	 * (cores, intensidade, névoa); o fallback simples daqui desliga.
	 */
	void SetSunController(AActor* Controller) { SunController = Controller; }

	UPROPERTY(BlueprintAssignable, Category = "Oracle|Time")
	FOracleDayChanged OnDayChanged;

	UPROPERTY(BlueprintAssignable, Category = "Oracle|Time")
	FOracleHourChanged OnHourChanged;

	/** Minutos reais por dia de jogo. 20min = ritmo Stardew-like. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Oracle|Time", meta = (ClampMin = 1))
	float DayLengthRealMinutes = 20.f;

private:
	void UpdateSun();

	float TimeOfDayHours = 10.5f;  // começa no meio da manhã, luz dourada alta
	int32 DayNumber = 1;
	int32 LastBroadcastHour = 10;

	TWeakObjectPtr<ADirectionalLight> Sun;
	TWeakObjectPtr<AActor> SunController;
	bool bSearchedSun = false;
};
