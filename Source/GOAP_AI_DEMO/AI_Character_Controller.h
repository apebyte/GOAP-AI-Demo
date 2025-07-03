#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GenericTeamAgentInterface.h"
#include "AI_Character_Controller.generated.h"

UCLASS()
class GOAP_AI_DEMO_API AAI_Character_Controller : public AAIController
{
    GENERATED_BODY()
public:
    AAI_Character_Controller();

protected:
    virtual void OnMoveCompleted(FAIRequestID RequestID,
        const FPathFollowingResult& Result) override;

    // Sight config
    UPROPERTY()
    UAISenseConfig_Sight* SightConfig;

    // Perception updated callback
    UFUNCTION()
    void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

    // Do NOT re-inherit the interface here
    // Just override the function
    virtual FGenericTeamId GetGenericTeamId() const override;

public:
    FGenericTeamId TeamId = FGenericTeamId(1); // 1 = Enemy team, 0 = Friendly team
};
