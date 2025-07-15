#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Node.h"
#include "GOAPAgentComponent.h"
#include "AI_Character.generated.h"

struct FAIRequestID;
struct FPathFollowingResult;

UCLASS()
class GOAP_AI_DEMO_API AAI_Character : public ACharacter
{
    GENERATED_BODY()

public:
    AAI_Character();

    virtual void BeginPlay() override;

    /**  Patrol path set in the editor */
    UPROPERTY(EditAnywhere, Category = "Patrol")
    TArray<ANode*> PatrolPath;

    /** Called by our AI-controller when a move finishes */
    void HandleMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);

    void Patrol();

    /** GOAP Agent Component */
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "GOAP")
    UGOAPAgentComponent* GOAPAgentComponent;

private:
    void MoveToNode(ANode* Target); // Issues the MoveTo request

    UPROPERTY(EditAnywhere, Category = "Patrol", meta = (AllowPrivateAccess = "true"))
    float AcceptanceRadius = 100.f;

    int32 CurrentPatrolIndex = 0;
};
