// Project Oracle - Cozy Open World AAA

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OracleWalletComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOracleCoinsChanged, int32, NewTotal);

/**
 * Carteira do jogador. Moeda única (Folhas) na v1; moedas premium/
 * evento entram como novos campos sem quebrar nada.
 */
UCLASS(ClassGroup = (Oracle), meta = (BlueprintSpawnableComponent))
class ORACLE_API UOracleWalletComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Oracle|Economy")
	void AddCoins(int32 Amount);

	/** Tenta gastar; false se não houver saldo. */
	UFUNCTION(BlueprintCallable, Category = "Oracle|Economy")
	bool SpendCoins(int32 Amount);

	UFUNCTION(BlueprintPure, Category = "Oracle|Economy")
	int32 GetCoins() const { return Coins; }

	void RestoreFromSave(int32 InCoins) { Coins = InCoins; }

	UPROPERTY(BlueprintAssignable, Category = "Oracle|Economy")
	FOracleCoinsChanged OnCoinsChanged;

private:
	UPROPERTY()
	int32 Coins = 0;
};
