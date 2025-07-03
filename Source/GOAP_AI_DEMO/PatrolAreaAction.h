#pragma once

#include "CoreMinimal.h"
#include "GOAPAction.h"
#include "PatrolAreaAction.generated.h"

UCLASS()
class GOAP_AI_DEMO_API UPatrolAreaAction : public UGOAPAction
{
    GENERATED_BODY()

public:
    UPatrolAreaAction();

    virtual bool CheckProceduralPrecondition(const TMap<FName, bool>& WorldState) const override;
    virtual void PerformAction() override;
};
