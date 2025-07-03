// GOAPAction.cpp
#include "GOAPAction.h"

UGOAPAction::UGOAPAction()
{
    Cost = 1.f;            // or any defaults you like
}

bool UGOAPAction::CheckProceduralPrecondition(const TMap<FName, bool>& /*WorldState*/) const
{
    // Default behaviour: assume it’s always allowed.
    // Child classes can odverride with real logic.
    return true;
}

void UGOAPAction::PerformAction()
{
    // Base class does nothing.
    // Child classes will override this.
}
