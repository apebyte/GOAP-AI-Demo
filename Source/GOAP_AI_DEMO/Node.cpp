#include "Node.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

ANode::ANode()
{
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
}

void ANode::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    // Always lift the node by 0.1 units on Z in the editor
    FVector Location = GetActorLocation();
    Location.Z += 0.01f;
    SetActorLocation(Location);
}

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node")
UStaticMeshComponent* MeshComponent;

void ANode::UpdateNodeColor()
{
    if (!MeshComponent) return;

    UMaterialInstanceDynamic* DynMat = MeshComponent->CreateAndSetMaterialInstanceDynamic(0);
    if (!DynMat) return;

    FLinearColor Color = FLinearColor::Blue; // Default for None

    switch (NodeType)
    {
        case ENodeType::None:
            Color = FLinearColor::Blue;
            break;
        case ENodeType::Cover:
            Color = FLinearColor::Red;
            break;
        case ENodeType::Flank:
            Color = FLinearColor::Yellow;
            break;
        case ENodeType::Turret:
            Color = FLinearColor::Green;
            break;
        case ENodeType::Danger:
            Color = FLinearColor::Black;
            break;
        default:
            break;
    }

    DynMat->SetVectorParameterValue("BaseColor", Color);
}

#if WITH_EDITOR
void ANode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    if (PropertyChangedEvent.Property &&
        PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(ANode, NodeType))
    {
        UpdateNodeColor();
    }
}
#endif
