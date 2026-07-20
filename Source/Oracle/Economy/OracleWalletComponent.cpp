// Project Oracle - Cozy Open World AAA

#include "Economy/OracleWalletComponent.h"

void UOracleWalletComponent::AddCoins(const int32 Amount)
{
	if (Amount == 0)
	{
		return;
	}
	Coins = FMath::Max(0, Coins + Amount);
	OnCoinsChanged.Broadcast(Coins);
}

bool UOracleWalletComponent::SpendCoins(const int32 Amount)
{
	if (Amount <= 0 || Coins < Amount)
	{
		return false;
	}
	Coins -= Amount;
	OnCoinsChanged.Broadcast(Coins);
	return true;
}
