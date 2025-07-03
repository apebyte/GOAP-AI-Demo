#pragma once

#include "CoreMinimal.h"
#include "GOAPAction.h"
#include "ChaseAction.generated.h"

UCLASS()
class GOAP_AI_DEMO_API UChaseAction : public UGOAPAction
{
	GENERATED_BODY()

public:
	UChaseAction();

	virtual bool CheckProceduralPrecondition(const TMap<FName, bool>& WorldState) const override;
	virtual void PerformAction() override;
};
