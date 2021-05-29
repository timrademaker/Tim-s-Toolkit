// Some copyright should be here...

using UnrealBuildTool;

public class TimsToolkit : ModuleRules
{
	public TimsToolkit(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core"
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"HTTP",
			}
		);

		if (Target.Type == TargetType.Editor)
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
