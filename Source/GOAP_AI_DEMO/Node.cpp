#include "Node.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

ANode::ANode()
{
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
    UpdateLinkSplines();
}

void ANode::BeginPlay()
{
    Super::BeginPlay();
}

void ANode::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    FVector Location = GetActorLocation();
    Location.Z += 0.01f;
    SetActorLocation(Location);

    UpdateLinkSplines();
}

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

void ANode::UpdateLinkSplines()
{
#if WITH_EDITOR
    // Destroy old splines
    for (USplineComponent* Spline : DebugSplines)
    {
        if (Spline) Spline->DestroyComponent();
    }
    DebugSplines.Empty();

    // Create a spline for each connection
    for (ANode* LinkedNode : LinkedNodes)
    {
        if (LinkedNode)
        {
            USplineComponent* Spline = NewObject<USplineComponent>(this, NAME_None, RF_Transient);
            Spline->bIsEditorOnly = true;
            Spline->RegisterComponent();
            Spline->SetMobility(EComponentMobility::Movable);
            Spline->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

            Spline->ClearSplinePoints(false);
            Spline->AddSplinePoint(GetActorLocation(), ESplineCoordinateSpace::World, false);
            Spline->AddSplinePoint(LinkedNode->GetActorLocation(), ESplineCoordinateSpace::World, false);
            Spline->SetClosedLoop(false);
            Spline->UpdateSpline();

            DebugSplines.Add(Spline);
        }
    }
#endif
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