// Project Oracle - Cozy Open World AAA

#include "Audio/OracleAudioSubsystem.h"

#include "Components/AudioComponent.h"
#include "Core/OracleTimeSubsystem.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "World/OracleWeatherSubsystem.h"

void UOracleAudioSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	if (UOracleTimeSubsystem* Time = InWorld.GetSubsystem<UOracleTimeSubsystem>())
	{
		Time->OnHourChanged.AddDynamic(this, &UOracleAudioSubsystem::HandleHourChanged);
	}
	if (UOracleWeatherSubsystem* Weather = InWorld.GetSubsystem<UOracleWeatherSubsystem>())
	{
		Weather->OnWeatherChanged.AddDynamic(this, &UOracleAudioSubsystem::HandleWeatherChanged);
	}
}

void UOracleAudioSubsystem::ConfigureAmbience(USoundBase* Day, USoundBase* Night, USoundBase* Rain)
{
	DaySound = Day;
	NightSound = Night;
	RainSound = Rain;
	UpdateTargetAmbience();
}

void UOracleAudioSubsystem::HandleHourChanged(int32 /*NewHour*/)
{
	UpdateTargetAmbience();
}

void UOracleAudioSubsystem::HandleWeatherChanged(EOracleWeather /*NewWeather*/)
{
	UpdateTargetAmbience();
}

void UOracleAudioSubsystem::UpdateTargetAmbience()
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// Prioridade: chuva > noite > dia. Escolhe a camada certa e faz fade.
	USoundBase* Desired = DaySound;

	if (const UOracleWeatherSubsystem* Weather = World->GetSubsystem<UOracleWeatherSubsystem>())
	{
		if (Weather->GetWeather() == EOracleWeather::Rain && RainSound)
		{
			Desired = RainSound;
		}
	}
	if (Desired == DaySound)
	{
		if (const UOracleTimeSubsystem* Time = World->GetSubsystem<UOracleTimeSubsystem>())
		{
			const float H = Time->GetTimeOfDayHours();
			const bool bNight = H < 6.f || H >= 19.f;
			if (bNight && NightSound)
			{
				Desired = NightSound;
			}
		}
	}

	if (Desired != CurrentSound)
	{
		PlayLayer(Desired);
	}
}

void UOracleAudioSubsystem::PlayLayer(USoundBase* Sound)
{
	CurrentSound = Sound;
	if (!Sound)
	{
		if (AmbienceComp)
		{
			AmbienceComp->FadeOut(1.5f, 0.f);
			AmbienceComp = nullptr;
		}
		return;
	}

	if (AmbienceComp)
	{
		AmbienceComp->FadeOut(1.5f, 0.f);
	}
	// Crossfade: nova camada sobe suave enquanto a antiga desce.
	AmbienceComp = UGameplayStatics::SpawnSound2D(GetWorld(), Sound, 0.6f, 1.f, 0.f, nullptr, true);
	if (AmbienceComp)
	{
		AmbienceComp->FadeIn(1.5f, 0.6f);
	}
}

void UOracleAudioSubsystem::Tick(float /*DeltaTime*/)
{
	// Reservado para modulação futura (volume por interior/exterior, combate).
}
