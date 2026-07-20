// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "OracleWeatherSubsystem.generated.h"

UENUM(BlueprintType)
enum class EOracleWeather : uint8
{
	Clear,   // sol
	Cloudy,  // nublado
	Rain     // chuva (VFX de gotas entra quando houver asset Niagara)
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOracleWeatherChanged, EOracleWeather, NewWeather);

/**
 * Clima dinâmico do mundo. Sorteia novo clima a cada poucas horas de
 * jogo (escutando o TimeSubsystem — nunca polling). O Environment lê
 * o estado para escurecer o sol e engrossar a névoa; sistemas futuros
 * (cultivos que amam chuva, NPCs que abrem guarda-chuva, pesca melhor
 * na chuva) escutam OnWeatherChanged.
 */
UCLASS()
class ORACLE_API UOracleWeatherSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	UFUNCTION(BlueprintPure, Category = "Oracle|Weather")
	EOracleWeather GetWeather() const { return Weather; }

	UFUNCTION(BlueprintPure, Category = "Oracle|Weather")
	FText GetWeatherName() const;

	/** Multiplicador de luz do sol para o clima atual (1 = céu limpo). */
	float GetSunMultiplier() const;

	/** Multiplicador da densidade de névoa (1 = céu limpo). */
	float GetFogMultiplier() const;

	UPROPERTY(BlueprintAssignable, Category = "Oracle|Weather")
	FOracleWeatherChanged OnWeatherChanged;

	/** De quantas em quantas horas de jogo o clima pode mudar. */
	UPROPERTY(EditAnywhere, Category = "Oracle|Weather", meta = (ClampMin = 1))
	int32 HoursBetweenChanges = 3;

protected:
	UFUNCTION()
	void HandleHourChanged(int32 NewHour);

private:
	void RollWeather();

	EOracleWeather Weather = EOracleWeather::Clear;
	int32 HoursSinceChange = 0;
};
