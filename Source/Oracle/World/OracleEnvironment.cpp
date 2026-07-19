// Project Oracle - Cozy Open World AAA

#include "World/OracleEnvironment.h"

#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/PostProcessComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/VolumetricCloudComponent.h"
#include "Core/OracleTimeSubsystem.h"
#include "EngineUtils.h"
#include "Oracle.h"

AOracleEnvironment::AOracleEnvironment()
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	Sun = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("Sun"));
	Sun->SetupAttachment(RootComponent);
	Sun->SetMobility(EComponentMobility::Movable);
	Sun->SetIntensity(DayIntensity);
	Sun->SetAtmosphereSunLight(true);

	SkyAtmosphere = CreateDefaultSubobject<USkyAtmosphereComponent>(TEXT("SkyAtmosphere"));
	SkyAtmosphere->SetupAttachment(RootComponent);

	Clouds = CreateDefaultSubobject<UVolumetricCloudComponent>(TEXT("Clouds"));
	Clouds->SetupAttachment(RootComponent);

	SkyLight = CreateDefaultSubobject<USkyLightComponent>(TEXT("SkyLight"));
	SkyLight->SetupAttachment(RootComponent);
	SkyLight->SetMobility(EComponentMobility::Movable);

	Fog = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("Fog"));
	Fog->SetupAttachment(RootComponent);
	Fog->SetFogDensity(0.015f);
	Fog->SetVolumetricFog(true);  // godrays com Lumen

	PostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcess"));
	PostProcess->SetupAttachment(RootComponent);
	PostProcess->bUnbound = true;

	// Color grading "anime cozy": vibrante, quente, com bloom suave.
	FPostProcessSettings& S = PostProcess->Settings;
	S.bOverride_BloomIntensity = true;
	S.BloomIntensity = 0.7f;
	S.bOverride_ColorSaturation = true;
	S.ColorSaturation = FVector4(1.14f, 1.14f, 1.14f, 1.f);
	S.bOverride_ColorContrast = true;
	S.ColorContrast = FVector4(1.05f, 1.05f, 1.05f, 1.f);
	S.bOverride_VignetteIntensity = true;
	S.VignetteIntensity = 0.35f;
	S.bOverride_AutoExposureBias = true;
	S.AutoExposureBias = 0.4f;
}

void AOracleEnvironment::BeginPlay()
{
	Super::BeginPlay();

	if (bReplaceExistingSky)
	{
		RemoveDuplicateSkyActors();
	}

	// Skylight acompanha o céu dinâmico.
	SkyLight->SetRealTimeCaptureEnabled(true);

	// Avisa o relógio que o sol agora é nosso.
	if (UOracleTimeSubsystem* Time = GetWorld()->GetSubsystem<UOracleTimeSubsystem>())
	{
		Time->SetSunController(this);
	}

	UpdateLighting();
}

void AOracleEnvironment::RemoveDuplicateSkyActors()
{
	TArray<AActor*> ToRemove;
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (Actor == this)
		{
			continue;
		}
		if (Actor->FindComponentByClass<UDirectionalLightComponent>() ||
			Actor->FindComponentByClass<USkyLightComponent>() ||
			Actor->FindComponentByClass<USkyAtmosphereComponent>() ||
			Actor->FindComponentByClass<UVolumetricCloudComponent>() ||
			Actor->FindComponentByClass<UExponentialHeightFogComponent>())
		{
			ToRemove.Add(Actor);
		}
	}
	for (AActor* Actor : ToRemove)
	{
		UE_LOG(LogOracle, Log, TEXT("Environment: removendo rig duplicado '%s'."), *Actor->GetName());
		Actor->Destroy();
	}
}

void AOracleEnvironment::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateLighting();
}

void AOracleEnvironment::UpdateLighting()
{
	const UOracleTimeSubsystem* Time = GetWorld()->GetSubsystem<UOracleTimeSubsystem>();
	if (!Time)
	{
		return;
	}

	// 6h nasce, 12h pico, 18h põe (mesma convenção do TimeSubsystem).
	const float Pitch = ((Time->GetTimeOfDayHours() - 6.f) / 24.f) * -360.f;
	const FRotator SunRot(Pitch, 40.f, 0.f);
	Sun->SetWorldRotation(SunRot);

	// Altura do sol: 1 = meio-dia, 0 = horizonte, <0 = noite.
	const float Height = -SunRot.Vector().Z;

	if (Height > 0.f)
	{
		// Crepúsculo → dia: laranja vira branco-quente.
		const float DayBlend = FMath::Clamp(Height / 0.3f, 0.f, 1.f);
		Sun->SetLightColor(FMath::Lerp(DuskColor, DayColor, DayBlend));
		Sun->SetIntensity(FMath::Lerp(0.6f, DayIntensity, DayBlend));
		Fog->SetFogDensity(FMath::Lerp(0.03f, 0.013f, DayBlend));
	}
	else
	{
		// Noite: luar azulado fraco, névoa mais presente.
		Sun->SetLightColor(NightColor);
		Sun->SetIntensity(NightIntensity);
		Fog->SetFogDensity(0.035f);
	}
}
