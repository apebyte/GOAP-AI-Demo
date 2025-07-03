#include "AIManager.h"
#include "Engine/World.h"

UAIManager* UAIManager::Get(UWorld* World)
{
    // Store the manager in the world as a singleton
    static UAIManager* Manager = nullptr;
    if (!Manager)
    {
        Manager = NewObject<UAIManager>(World, UAIManager::StaticClass());
        Manager->AddToRoot(); // Prevent garbage collection
    }
    return Manager;
}

void UAIManager::AddVisibleCharacter(AActor* Actor)
{
    if (Actor)
    {
        VisibleCharacters.Add(Actor);
    }
}

void UAIManager::RemoveVisibleCharacter(AActor* Actor)
{
    if (Actor)
    {
        VisibleCharacters.Remove(Actor);
    }
}

const TSet<TWeakObjectPtr<AActor>>& UAIManager::GetVisibleCharacters() const
{
    return VisibleCharacters;
}

void UAIManager::Clear()
{
    VisibleCharacters.Empty();
}