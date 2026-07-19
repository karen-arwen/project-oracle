using UnrealBuildTool;

public class Oracle : ModuleRules
{
	public Oracle(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// Permite includes relativos à raiz do módulo: "Character/OracleCharacter.h" etc.
		PublicIncludePaths.Add(ModuleDirectory);

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"Niagara"
		});

		// Futuro (não adicionar antes do módulo existir de fato):
		// "GameplayAbilities", "GameplayTags", "GameplayTasks"  -> Módulo de Habilidades
		// "CommonUI", "UMG"                                     -> Módulo de UI
		// "Niagara"                                             -> VFX
	}
}
