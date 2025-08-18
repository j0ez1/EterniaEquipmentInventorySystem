using UnrealBuildTool;

public class EterniaItems : ModuleRules
{
    public EterniaItems(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "EnhancedInput", 
                "GameplayAbilities"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "GameplayAbilities",
                "EnhancedInput",
                "DeveloperSettings"
            }
        );
    }
}