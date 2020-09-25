// Some copyright should be here...

using UnrealBuildTool;

public class TimsToolkitLoadingScreen : ModuleRules
{
	public TimsToolkitLoadingScreen(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;


		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);


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
				"MoviePlayer", 
				"UMG"
				// ... add private dependencies that you statically link with here ...	
			}
			);


		PublicIncludePathModuleNames.AddRange(
		new string[] {
				"Settings"
		}
		);

		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				"Settings"
			}
			);
	}
}
