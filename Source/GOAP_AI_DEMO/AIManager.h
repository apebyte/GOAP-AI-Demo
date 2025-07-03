#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "AIManager.generated.h"

UCLASS()
class GOAP_AI_DEMO_API UAIManager : public UObject
{
    GENERATED_BODY()

public:
    // Singleton accessor
    static UAIManager* Get(UWorld* World);

    // Add a visible character
    void AddVisibleCharacter(AActor* Actor);

    // Remove a character from visibility
    void RemoveVisibleCharacter(AActor* Actor);

    // Get all currently visible characters
    const TSet<TWeakObjectPtr<AActor>>& GetVisibleCharacters() const;

    // Clear all (e.g., on level reset)
    void Clear();

private:
    // Set of currently visible characters
    TSet<TWeakObjectPtr<AActor>> VisibleCharacters;
};