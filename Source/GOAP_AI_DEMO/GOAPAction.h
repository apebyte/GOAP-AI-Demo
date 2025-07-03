#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GOAPTypes.h"
#include "GOAPAction.generated.h"

UCLASS(Blueprintable)
class GOAP_AI_DEMO_API UGOAPAction : public UObject
{
    GENERATED_BODY()

public:
    UGOAPAction();

    // Preconditions that must be true to consider this action
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
    TArray<FGOAPState> Preconditions;

    // Effects that become true after the action completes
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
    TArray<FGOAPState> Effects;

    // Optional cost for pathfinding/planning
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GOAP")
    float Cost = 1.0f;

    // Optional check (e.g. is enemy in sight?)
    UFUNCTION(BlueprintCallable, Category = "GOAP")
    virtual bool CheckProceduralPrecondition(const TMap<FName, bool>& WorldState) const;

    // The actual behavior to perform (to override in subclasses)
    UFUNCTION(BlueprintCallable, Category = "GOAP")
    virtual void PerformAction();
};
