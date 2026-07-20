// Project Oracle - Cozy Open World AAA

#include "World/OracleWeatherSubsystem.h"

#include "Collections/OracleCollectionSubsystem.h"
#include "Core/OracleTimeSubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"

void UOracleWeatherSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	if (UOracleTimeSubsystem* Time = InWorld.GetSubsystem<UOracleTimeSubsystem>())
	{
		Time->OnHourChanged.AddDynamic(this, &UOracleWeatherSubsystem::HandleHourChanged);
	}
}

void UOracleWeatherSubsystem::HandleHourChanged(int32 /*NewHour*/)
{
	if (++HoursSinceChange >= HoursBetweenChanges)
	{
		HoursSinceChange = 0;
		RollWeather();
	}
}

void UOracleWeatherSubsystem::RollWeather()
{
	// Cozy: sol é maioria; chuva é evento charmoso, não castigo.
	const float Roll = FMath::FRand();
	const EOracleWeather NewWeather =
		Roll < 0.55f ? EOracleWeather::Clear :
		Roll < 0.85f ? EOracleWeather::Cloudy :
		               EOracleWeather::Rain;

	if (NewWeather == Weather)
	{
		return;
	}
	Weather = NewWeather;
	OnWeatherChanged.Broadcast(Weather);

	// Toast avisando a mudança — o mundo conversa com o jogador.
	if (const UWorld* World = GetWorld())
	{
		if (UOracleCollectionSubsystem* Toasts =
				World->GetGameInstance()->GetSubsystem<UOracleCollectionSubsystem>())
		{
			Toasts->AddToast(FText::Format(
				NSLOCTEXT("Oracle", "WeatherToast", "☁ O tempo mudou: {0}"), GetWeatherName()));
		}
	}
}

FText UOracleWeatherSubsystem::GetWeatherName() const
{
	switch (Weather)
	{
	case EOracleWeather::Cloudy: return NSLOCTEXT("Oracle", "WCloudy", "Nublado");
	case EOracleWeather::Rain:   return NSLOCTEXT("Oracle", "WRain", "Chuva");
	default:                     return NSLOCTEXT("Oracle", "WClear", "Sol");
	}
}

float UOracleWeatherSubsystem::GetSunMultiplier() const
{
	switch (Weather)
	{
	case EOracleWeather::Cloudy: return 0.55f;
	case EOracleWeather::Rain:   return 0.35f;
	default:                     return 1.f;
	}
}

float UOracleWeatherSubsystem::GetFogMultiplier() const
{
	switch (Weather)
	{
	case EOracleWeather::Cloudy: return 1.6f;
	case EOracleWeather::Rain:   return 2.4f;
	default:                     return 1.f;
	}
}
