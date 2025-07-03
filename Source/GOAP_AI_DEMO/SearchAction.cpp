#include "SearchAction.h"
#include "GameFramework/Actor.h"

USearchAction::USearchAction()
{
    // Must have seen an enemy to start searching
    Preconditions.Add(FGOAPState("EnemyVisible", true));

    // Searching gives us targeting ability, or readiness to attack
    Effects.Add(FGOAPState("Alert", true));

    Cost = 1.0f;
}

bool USearchAction::CheckProceduralPrecondition(const TMap<FName, bool>& WorldState) const
{
    // Optionally add logic like checking line of sight or noise here
    return true;
}

void USearchAction::PerformAction()
{
    UE_LOG(LogTemp, Warning, TEXT("Performing: SearchAction"));
}
