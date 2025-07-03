#include "PatrolAreaAction.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AI_Character.h"

UPatrolAreaAction::UPatrolAreaAction()
{
    // What must be true to patrol?
    Preconditions.Add(FGOAPState("IsIdle", true));

    // What does this action do to the world?
    Effects.Add(FGOAPState("EnemyVisible", true));
    Effects.Add(FGOAPState("Alert", true)); // Add Alert state when player is spotted

    Cost = 1.0f;
}

bool UPatrolAreaAction::CheckProceduralPrecondition(const TMap<FName, bool>& WorldState) const
{
    bool bEnemyVisible = WorldState.Contains("EnemyVisible") && WorldState["EnemyVisible"];
    return !bEnemyVisible;
}

void UPatrolAreaAction::PerformAction()
{
    // Get the owning component
    UGOAPAgentComponent* AgentComp = Cast<UGOAPAgentComponent>(GetOuter());
    if (!AgentComp) return;

    // Get the actor that owns the component
    AAI_Character* AIChar = Cast<AAI_Character>(AgentComp->GetOwner());
    if (AIChar)
    {
        AIChar->Patrol();
    }
}
