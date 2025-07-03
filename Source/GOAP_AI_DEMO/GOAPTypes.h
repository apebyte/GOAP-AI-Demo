#pragma once

#include "CoreMinimal.h"
#include "GOAPTypes.generated.h"

USTRUCT(BlueprintType)
struct FGOAPState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName Key;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool Value;

    FGOAPState() {}
    FGOAPState(FName InKey, bool InValue) : Key(InKey), Value(InValue) {}
};

USTRUCT(BlueprintType)
struct FGOAPGoal
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FGOAPState> DesiredStates;
};
