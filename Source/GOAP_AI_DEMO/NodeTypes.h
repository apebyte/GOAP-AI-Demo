#pragma once

#include "NodeTypes.generated.h"

UENUM(BlueprintType)
enum class ENodeType : uint8
{
    None        UMETA(DisplayName = "None"),
    Cover       UMETA(DisplayName = "Cover"),
    Flank       UMETA(DisplayName = "Flank"),
    Turret      UMETA(DisplayName = "Turret"),
    Danger      UMETA(DisplayName = "Danger")
};
