// Some copyright should be here...

using UnrealBuildTool;

public class TimsToolkitNodes : ModuleRules
{
	public TimsToolkitNodes(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"BlueprintGraph",
				"GraphEditor",
				"UnrealEd",
				"KismetCompiler",
				"TimsToolkit"
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore"
			}
		);
	}
}
