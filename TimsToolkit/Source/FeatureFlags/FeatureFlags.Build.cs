// Some copyright should be here...

using UnrealBuildTool;

public class FeatureFlags : ModuleRules
{
	public FeatureFlags(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"BlueprintGraph",
				"GraphEditor",
				"UnrealEd",
				"KismetCompiler"
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"HTTP"
			}
		);

		if (Target.Type == TargetRules.TargetType.Editor)
		{
			PublicIncludePathModuleNames.AddRange(
				new string[] {
					"Settings"
				}
			);

			DynamicallyLoadedModuleNames.AddRange(
				new string[] {
					"Settings"
				}
			);
		}
	}
}
