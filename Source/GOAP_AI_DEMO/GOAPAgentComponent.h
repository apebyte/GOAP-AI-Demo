#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GOAPTypes.h"
#include "GOAPAction.h"
#include "GOAPAgentComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GOAP_AI_DEMO_API UGOAPAgentComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UGOAPAgentComponent();

    // Available actions this agent can perform
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
    TArray<TSubclassOf<UGOAPAction>> AvailableActionTypes;

    // The current list of action instances (spawned)
    UPROPERTY()
    TArray<UGOAPAction*> ActionInstances;

    // Current plan of actions
    UPROPERTY()
    TArray<UGOAPAction*> CurrentPlan;

    // World state of this agent
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
    TMap<FName, bool> WorldState;

    // Desired goal state
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
    FGOAPGoal CurrentGoal;

    // Called on BeginPlay
    virtual void BeginPlay() override;

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Starts building a plan
    void BuildPlan();

    // Executes the next action in the current plan
    void ExecutePlan();
};
