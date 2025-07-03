#include "AI_Character_Controller.h"
#include "AI_Character.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "AIManager.h"

AAI_Character_Controller::AAI_Character_Controller()
{
    // Ensure the perception component exists; create if not present
    if (!GetPerceptionComponent())
    {
        // Create a new perception component if one doesn't exist
        SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent")));
    }
    // Get the perception component for configuration
    UAIPerceptionComponent* PerceptionComp = GetPerceptionComponent();

    if (PerceptionComp)
    {
        // Create and configure the sight sense
        SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

        // Set the maximum distance the AI can see
        SightConfig->SightRadius = 1500.f;
        // Set the distance at which the AI loses sight of a target
        SightConfig->LoseSightRadius = 1600.f;
        // Set the peripheral vision angle (field of view)
        SightConfig->PeripheralVisionAngleDegrees = 90.f;
        // Set how long a stimulus is remembered
        SightConfig->SetMaxAge(5.f);
        // Configure detection to only see friendlies (e.g., the player)
        SightConfig->DetectionByAffiliation.bDetectEnemies = false;
        SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
        SightConfig->DetectionByAffiliation.bDetectNeutrals = false;

        // Register the sight sense with the perception component
        PerceptionComp->ConfigureSense(*SightConfig);
        // Set sight as the dominant sense
        PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());

        // Bind the perception updated event to our handler
        PerceptionComp->OnPerceptionUpdated.AddDynamic(this, &AAI_Character_Controller::OnPerceptionUpdated);
    }
}

void AAI_Character_Controller::OnMoveCompleted(FAIRequestID RequestID,
    const FPathFollowingResult& Result)
{
    // Call the base class implementation
    Super::OnMoveCompleted(RequestID, Result);

    // If the controlled pawn is an AI_Character, notify it of move completion
    if (AAI_Character* AIChar = Cast<AAI_Character>(GetPawn()))
    {
        AIChar->HandleMoveCompleted(RequestID, Result);
    }
}

void AAI_Character_Controller::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
    bool bPlayerSeen = false;
    UWorld* World = GetWorld();
    UAIManager* Manager = UAIManager::Get(World);

    for (AActor* Actor : UpdatedActors)
    {
        if (!Actor) continue;

        ETeamAttitude::Type Attitude = GetTeamAttitudeTowards(*Actor);
        if (Attitude == ETeamAttitude::Friendly)
        {
            bPlayerSeen = true;
            Manager->AddVisibleCharacter(Actor); // Only add friendlies (the player)
        }
    }

    if (AAI_Character* AIChar = Cast<AAI_Character>(GetPawn()))
    {
        if (AIChar->GOAPAgentComponent)
        {
            AIChar->GOAPAgentComponent->WorldState.Add("EnemyVisible", bPlayerSeen);
            AIChar->GOAPAgentComponent->WorldState.Add("Alert", bPlayerSeen);
        }
    }
}

FGenericTeamId AAI_Character_Controller::GetGenericTeamId() const
{
    return TeamId; // TeamId should be defined in the header and set appropriately
}
