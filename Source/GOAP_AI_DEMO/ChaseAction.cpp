#include "ChaseAction.h"
#include "AI_Character.h"
#include "GOAPAgentComponent.h"

UChaseAction::UChaseAction()
{
    // Preconditions: must see the enemy
    Preconditions.Add(FGOAPState("EnemyVisible", true));
}

bool UChaseAction::CheckProceduralPrecondition(const TMap<FName, bool>& WorldState) const
{
    // Only chase if enemy is visible
    bool bEnemyVisible = WorldState.Contains("EnemyVisible") && WorldState["EnemyVisible"];
    return bEnemyVisible;
}

void UChaseAction::PerformAction()
{
    UGOAPAgentComponent* AgentComp = Cast<UGOAPAgentComponent>(GetOuter());
    if (!AgentComp) return;

    AAI_Character* AIChar = Cast<AAI_Character>(AgentComp->GetOwner());
    if (AIChar)
    {
        // Implement your chase logic here, e.g., move toward the player
        UE_LOG(LogTemp, Warning, TEXT("AI_Character is chasing the player!"));
        // Example: AIChar->ChasePlayer();
    }
}
