#include "GOAPAgentComponent.h"

// Constructor
UGOAPAgentComponent::UGOAPAgentComponent()
{
    // Enable ticking for this component
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGOAPAgentComponent::BeginPlay()
{
    Super::BeginPlay();


    // Create instances of all available action classes
    for (TSubclassOf<UGOAPAction> ActionClass : AvailableActionTypes)
    {
        if (ActionClass)
        {
            UGOAPAction* Action = NewObject<UGOAPAction>(this, ActionClass);
            ActionInstances.Add(Action);
        }
    }

    // Try building a plan toward the current goal
    BuildPlan();
}

// Called every frame
void UGOAPAgentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // 1. Check if the current goal is already satisfied
    bool bGoalSatisfied = true;
    for (const FGOAPState& GoalState : CurrentGoal.DesiredStates)
    {
        if (!WorldState.Contains(GoalState.Key) || WorldState[GoalState.Key] != GoalState.Value)
        {
            bGoalSatisfied = false;
            break;
        }
    }

    if (bGoalSatisfied)
    {
        // Goal is already met; no further action needed
        UE_LOG(LogTemp, Warning, TEXT("AI_Character has reached its GOAP goal state(s)!"));
        return;
    }

    // 2. If no plan exists, try to build one
    if (CurrentPlan.Num() == 0)
    {
        BuildPlan();
    }

    // 3. If a plan exists, execute the next action in the sequence
    if (CurrentPlan.Num() > 0)
    {
        ExecutePlan();  // Assumes one action per tick
    }
}

// Attempts to create a plan from current world state to desired goal
void UGOAPAgentComponent::BuildPlan()
{
    CurrentPlan.Empty();  // Clear any previous plan

    TArray<UGOAPAction*> OpenActions;
    TMap<FName, bool> SimulatedState = WorldState;

    // Step 1: Find actions whose preconditions match the simulated state
    for (UGOAPAction* Action : ActionInstances)
    {
        if (Action && Action->CheckProceduralPrecondition(SimulatedState))
        {
            bool bMatches = true;

            // Check all preconditions
            for (const FGOAPState& Pre : Action->Preconditions)
            {
                if (!SimulatedState.Contains(Pre.Key) || SimulatedState[Pre.Key] != Pre.Value)
                {
                    bMatches = false;
                    break;
                }
            }

            // Add valid actions to the open set
            if (bMatches)
            {
                OpenActions.Add(Action);
            }
        }
    }

    // Step 2: Simulate applying action effects to reach the goal
    for (UGOAPAction* Action : OpenActions)
    {
        // Apply each effect to the simulated state
        for (const FGOAPState& Effect : Action->Effects)
        {
            SimulatedState.Add(Effect.Key, Effect.Value);
        }

        // Add action to the current plan
        CurrentPlan.Add(Action);

        // Check if goal is achieved in the simulated state
        bool bGoalMet = true;
        for (const FGOAPState& GoalState : CurrentGoal.DesiredStates)
        {
            if (!SimulatedState.Contains(GoalState.Key) || SimulatedState[GoalState.Key] != GoalState.Value)
            {
                bGoalMet = false;
                break;
            }
        }

        // Stop planning if goal is reached
        if (bGoalMet)
        {
            break;
        }
    }

    // Debug log: print number of actions in the plan
    UE_LOG(LogTemp, Warning, TEXT("Plan built with %d actions."), CurrentPlan.Num());
}

// Executes the next action in the plan
void UGOAPAgentComponent::ExecutePlan()
{
    if (CurrentPlan.Num() > 0)
    {
        // Get and remove the next action
        UGOAPAction* NextAction = CurrentPlan[0];
        CurrentPlan.RemoveAt(0);

        if (NextAction)
        {
            // Perform the action behavior
            NextAction->PerformAction();

            // Apply action's effects to the actual world state
            for (const FGOAPState& Effect : NextAction->Effects)
            {
                WorldState.Add(Effect.Key, Effect.Value);
            }
        }
    }
}
