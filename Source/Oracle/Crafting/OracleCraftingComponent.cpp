// Project Oracle - Cozy Open World AAA

#include "Crafting/OracleCraftingComponent.h"

#include "Character/OracleCharacter.h"
#include "Crafting/OracleRecipeDefinition.h"
#include "Inventory/OracleInventoryComponent.h"
#include "Inventory/OracleItemDefinition.h"
#include "Skills/OracleSkillsComponent.h"

void UOracleCraftingComponent::LearnRecipe(UOracleRecipeDefinition* Recipe)
{
	if (Recipe && !KnownRecipes.Contains(Recipe))
	{
		KnownRecipes.Add(Recipe);
	}
}

UOracleInventoryComponent* UOracleCraftingComponent::GetInventory() const
{
	const AOracleCharacter* Character = Cast<AOracleCharacter>(GetOwner());
	return Character ? Character->GetInventory() : nullptr;
}

bool UOracleCraftingComponent::CanCraft(const UOracleRecipeDefinition* Recipe) const
{
	const UOracleInventoryComponent* Inventory = GetInventory();
	if (!Recipe || !Recipe->Result || !Inventory)
	{
		return false;
	}
	for (const FOracleIngredient& Ingredient : Recipe->Ingredients)
	{
		if (!Ingredient.Item || Inventory->CountOf(Ingredient.Item) < Ingredient.Count)
		{
			return false;
		}
	}
	return true;
}

UOracleRecipeDefinition* UOracleCraftingComponent::GetFirstCraftable() const
{
	for (UOracleRecipeDefinition* Recipe : KnownRecipes)
	{
		if (CanCraft(Recipe))
		{
			return Recipe;
		}
	}
	return nullptr;
}

bool UOracleCraftingComponent::Craft(UOracleRecipeDefinition* Recipe)
{
	UOracleInventoryComponent* Inventory = GetInventory();
	if (!Inventory || !CanCraft(Recipe))
	{
		return false;
	}

	for (const FOracleIngredient& Ingredient : Recipe->Ingredients)
	{
		Inventory->RemoveItem(Ingredient.Item, Ingredient.Count);
	}
	// AddItem dispara o compêndio: toast "+1 {resultado}" (e descoberta).
	Inventory->AddItem(Recipe->Result, Recipe->ResultCount);

	if (const AOracleCharacter* Character = Cast<AOracleCharacter>(GetOwner()))
	{
		Character->GetSkills()->AddXP(EOracleSkill::Artesanato, 10);
	}
	return true;
}

void UOracleCraftingComponent::CraftBest()
{
	Craft(GetFirstCraftable());
}
