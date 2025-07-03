#pragma once

#include "CoreMinimal.h"
#include "GOAPAction.h"
#include "SearchAction.generated.h"

UCLASS()
class GOAP_AI_DEMO_API USearchAction : public UGOAPAction
{
    GENERATED_BODY()

public:
    USearchAction();

    virtual bool CheckProceduralPrecondition(const TMap<FName, bool>& WorldState) const override;
    virtual void PerformAction() override;
};
