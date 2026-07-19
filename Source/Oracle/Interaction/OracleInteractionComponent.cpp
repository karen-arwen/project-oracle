// Project Oracle - Cozy Open World AAA

#include "Interaction/OracleInteractionComponent.h"

#include "Character/OracleCharacter.h"
#include "Interaction/OracleInteractable.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"

UOracleInteractionComponent::UOracleInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UOracleInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(SearchTimer, this,
		&UOracleInteractionComponent::UpdateFocus, SearchInterval, true);
}

void UOracleInteractionComponent::UpdateFocus()
{
	AOracleCharacter* Character = Cast<AOracleCharacter>(GetOwner());
	if (!Character)
	{
		FocusedActor = nullptr;
		return;
	}

	const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = {
		UEngineTypes::ConvertToObjectType(ECC_WorldStatic),
		UEngineTypes::ConvertToObjectType(ECC_WorldDynamic)
	};

	TArray<AActor*> Found;
	UKismetSystemLibrary::SphereOverlapActors(this, Character->GetActorLocation(),
		SearchRadius, ObjectTypes, nullptr, {Character}, Found);

	AActor* Best = nullptr;
	float BestDistSq = TNumericLimits<float>::Max();

	for (AActor* Actor : Found)
	{
		if (!Actor->GetClass()->ImplementsInterface(UOracleInteractable::StaticClass()))
		{
			continue;
		}
		if (!IOracleInteractable::Execute_CanInteract(Actor, Character))
		{
			continue;
		}
		const float DistSq = FVector::DistSquared(Actor->GetActorLocation(), Character->GetActorLocation());
		if (DistSq < BestDistSq)
		{
			BestDistSq = DistSq;
			Best = Actor;
		}
	}

	FocusedActor = Best;
}

void UOracleInteractionComponent::TryInteract()
{
	AOracleCharacter* Character = Cast<AOracleCharacter>(GetOwner());
	AActor* Target = FocusedActor.Get();

	if (Character && Target &&
		Target->GetClass()->ImplementsInterface(UOracleInteractable::StaticClass()) &&
		IOracleInteractable::Execute_CanInteract(Target, Character))
	{
		IOracleInteractable::Execute_Interact(Target, Character);
		UpdateFocus();  // o alvo pode ter sumido (ex.: coletado)
	}
}

FText UOracleInteractionComponent::GetFocusedText() const
{
	if (AActor* Target = FocusedActor.Get())
	{
		return IOracleInteractable::Execute_GetInteractionText(Target);
	}
	return FText::GetEmpty();
}
