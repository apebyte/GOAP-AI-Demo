#include "AI_Character.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "AI_Character_Controller.h"
#include "GOAPAgentComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"

AAI_Character::AAI_Character()
{
    PrimaryActorTick.bCanEverTick = true;
    AIControllerClass = AAI_Character_Controller::StaticClass();

    // Initialize the GOAPAgentComponent
    //GOAPAgentComponent = CreateDefaultSubobject<UGOAPAgentComponent>(TEXT("GOAPAgentComponent"));

    // Initialize the NavigationComponent
    NavigationComponent = CreateDefaultSubobject<UAC_NavigationComponent>(TEXT("NavigationComponent"));
}

void AAI_Character::BeginPlay()
{
    Super::BeginPlay();

    // Ensure NavigationComponent is valid
    if (NavigationComponent)
    {
        ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
        if (PlayerCharacter)
        {
            NavigationComponent->FindStartAndEndNodes(PlayerCharacter->GetActorLocation());
        }
    }
}

// Patrol logic: move to each node in the PatrolPath in sequence
void AAI_Character::Patrol()
{
    if (PatrolPath.Num() == 0) return;

    ANode* TargetNode = PatrolPath[CurrentPatrolIndex];
    if (!TargetNode) return;

    MoveToNode(TargetNode);
}

void AAI_Character::MoveToNode(ANode* TargetNode)
{
    if (AAIController* AICon = Cast<AAIController>(GetController()))
    {
        AICon->MoveToLocation(TargetNode->GetActorLocation(), AcceptanceRadius, true);
    }
}

void AAI_Character::HandleMoveCompleted(FAIRequestID, const FPathFollowingResult&)
{
    CurrentPatrolIndex = (CurrentPatrolIndex + 1) % PatrolPath.Num();
    Patrol();
}
