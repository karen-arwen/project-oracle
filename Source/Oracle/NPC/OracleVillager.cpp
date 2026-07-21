// Project Oracle - Cozy Open World AAA

#include "NPC/OracleVillager.h"

#include "Character/OracleCharacter.h"
#include "Collections/OracleCollectionSubsystem.h"
#include "Components/CapsuleComponent.h"
#include "Core/OracleTimeSubsystem.h"
#include "Engine/SkeletalMesh.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"

AOracleVillager::AOracleVillager()
{
	PrimaryActorTick.bCanEverTick = true;

	// Um AIController padrão possui o NPC para processar movimento/rotina.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->InitCapsuleSize(35.f, 90.f);
	GetCharacterMovement()->MaxWalkSpeed = 160.f;  // passo tranquilo de vila
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	// Mesh padrão: mannequin (null-safe). Design troca por NPC anime depois.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MannMesh(
		TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple"));
	if (MannMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MannMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, -90.f, 0.f));
	}
}

void AOracleVillager::Setup(const FText& InName, const TArray<FText>& InLines,
                            const FVector& InHomeSpot, const FVector& InDaySpot)
{
	VillagerName = InName;
	DialogueLines = InLines;
	HomeSpot = InHomeSpot;
	DaySpot = InDaySpot;
	TargetSpot = InDaySpot;
}

void AOracleVillager::BeginPlay()
{
	Super::BeginPlay();

	if (UOracleTimeSubsystem* Time = GetWorld()->GetSubsystem<UOracleTimeSubsystem>())
	{
		Time->OnHourChanged.AddDynamic(this, &AOracleVillager::HandleHourChanged);
		HandleHourChanged(FMath::FloorToInt32(Time->GetTimeOfDayHours()));
	}
}

void AOracleVillager::HandleHourChanged(int32 NewHour)
{
	// Rotina simples: 7h–19h na praça; resto em casa.
	const bool bDaytime = NewHour >= 7 && NewHour < 19;
	TargetSpot = bDaytime ? DaySpot : HomeSpot;
}

void AOracleVillager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Caminha suavemente até o destino da rotina.
	const FVector ToTarget = TargetSpot - GetActorLocation();
	if (ToTarget.SizeSquared2D() > 100.f * 100.f)
	{
		AddMovementInput(ToTarget.GetSafeNormal2D(), 1.f);
	}
}

FText AOracleVillager::GetInteractionText_Implementation() const
{
	return FText::Format(NSLOCTEXT("Oracle", "TalkTo", "Conversar com {0}"), VillagerName);
}

bool AOracleVillager::CanInteract_Implementation(AOracleCharacter* Interactor) const
{
	return Interactor != nullptr && DialogueLines.Num() > 0;
}

void AOracleVillager::Interact_Implementation(AOracleCharacter* Interactor)
{
	if (DialogueLines.Num() == 0)
	{
		return;
	}

	// Fala rotativa.
	const FText Line = DialogueLines[NextLine % DialogueLines.Num()];
	++NextLine;
	Toast(FText::Format(NSLOCTEXT("Oracle", "NpcSays", "{0}: \"{1}\""), VillagerName, Line), false);

	// Amizade sobe; marcos (a cada 5) celebram.
	++Friendship;
	if (Friendship % 5 == 0)
	{
		Toast(FText::Format(NSLOCTEXT("Oracle", "Friend", "♥ Sua amizade com {0} cresceu! (nível {1})"),
			VillagerName, Friendship / 5), /*bGolden=*/true);
	}
}

void AOracleVillager::Toast(const FText& Text, const bool bGolden) const
{
	if (UOracleCollectionSubsystem* Toasts =
			GetGameInstance()->GetSubsystem<UOracleCollectionSubsystem>())
	{
		Toasts->AddToast(Text, bGolden);
	}
}
