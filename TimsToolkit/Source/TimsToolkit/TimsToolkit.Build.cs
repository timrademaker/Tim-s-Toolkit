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
				"Core",
				// ... add other public dependencies that you statically link with here ...
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
				// ... add private dependencies that you statically link with here ...	
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
