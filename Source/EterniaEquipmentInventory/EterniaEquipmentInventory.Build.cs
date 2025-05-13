using UnrealBuildTool;

public class EterniaEquipmentInventory : ModuleRules
{
    public EterniaEquipmentInventory(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "EnhancedInput", "GameplayAbilities"
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