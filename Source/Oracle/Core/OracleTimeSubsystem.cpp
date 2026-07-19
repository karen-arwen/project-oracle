// Project Oracle - Cozy Open World AAA

#include "Core/OracleTimeSubsystem.h"

#include "Engine/DirectionalLight.h"
#include "EngineUtils.h"

void UOracleTimeSubsystem::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Avança o relógio.
	const float HoursPerRealSecond = 24.f / (DayLengthRealMinutes * 60.f);
	TimeOfDayHours += DeltaTime * HoursPerRealSecond;

	if (TimeOfDayHours >= 24.f)
	{
		TimeOfDayHours -= 24.f;
		++DayNumber;
		OnDayChanged.Broadcast(DayNumber);
	}

	const int32 Hour = FMath::FloorToInt32(TimeOfDayHours);
	if (Hour != LastBroadcastHour)
	{
		LastBroadcastHour = Hour;
		OnHourChanged.Broadcast(Hour);
	}

	UpdateSun();
}

void UOracleTimeSubsystem::UpdateSun()
{
	// O AOracleEnvironment assumiu o sol? Então este fallback não age.
	if (SunController.IsValid())
	{
		return;
	}

	if (!Sun.IsValid() && !bSearchedSun)
	{
		bSearchedSun = true;
		for (TActorIterator<ADirectionalLight> It(GetWorld()); It; ++It)
		{
			Sun = *It;
			break;
		}
	}

	if (Sun.IsValid())
	{
		// 6h = nascer (0°), 12h = pico (-90°), 18h = pôr (-180°).
		const float Pitch = ((TimeOfDayHours - 6.f) / 24.f) * -360.f;
		Sun->SetActorRotation(FRotator(Pitch, 40.f, 0.f));
	}
}

FString UOracleTimeSubsystem::GetClockText() const
{
	const int32 Hour = FMath::FloorToInt32(TimeOfDayHours);
	const int32 Minute = FMath::FloorToInt32((TimeOfDayHours - Hour) * 60.f);
	return FString::Printf(TEXT("Dia %d — %02d:%02d"), DayNumber, Hour, Minute);
}

void UOracleTimeSubsystem::SetTime(const int32 InDay, const float InHours)
{
	DayNumber = FMath::Max(1, InDay);
	TimeOfDayHours = FMath::Clamp(InHours, 0.f, 24.f);
	LastBroadcastHour = FMath::FloorToInt32(TimeOfDayHours);
	UpdateSun();
}
