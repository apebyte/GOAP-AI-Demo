#include "NodeEditorUtility.h"
#include "Node.h"
#include "Editor.h"
#include "Engine/Selection.h"
#include "EngineUtils.h"

void UNodeEditorUtility::Link()
{
#if WITH_EDITOR
    // Get the current editor world
    UWorld* World = GEditor ? GEditor->GetEditorWorldContext().World() : nullptr;
    if (!World) return;

    // Get all selected actors
    TArray<AActor*> SelectedActors;
    for (FSelectionIterator It(GEditor->GetSelectedActorIterator()); It; ++It)
    {
        if (AActor* Actor = Cast<AActor>(*It))
        {
            SelectedActors.Add(Actor);
        }
    }

    // Filter to only ANode actors
    TArray<ANode*> SelectedNodes;
    for (AActor* Actor : SelectedActors)
    {
        if (ANode* Node = Cast<ANode>(Actor))
        {
            SelectedNodes.Add(Node);
        }
    }

    // Link each node to all others (bidirectional)
    for (ANode* Node : SelectedNodes)
    {
        for (ANode* OtherNode : SelectedNodes)
        {
            if (Node != OtherNode && !Node->LinkedNodes.Contains(OtherNode))
            {
                Node->LinkedNodes.Add(OtherNode);
                Node->Modify();
            }
        }
    }

    // After all links are set, update splines for all nodes
    for (ANode* Node : SelectedNodes)
    {
        Node->UpdateLinkSplines();
    }
#endif
}

void UNodeEditorUtility::Unlink()
{
#if WITH_EDITOR
    UWorld* World = GEditor ? GEditor->GetEditorWorldContext().World() : nullptr;
    if (!World) return;

    TArray<AActor*> SelectedActors;
    for (FSelectionIterator It(GEditor->GetSelectedActorIterator()); It; ++It)
    {
        if (AActor* Actor = Cast<AActor>(*It))
        {
            SelectedActors.Add(Actor);
        }
    }

    TArray<ANode*> SelectedNodes;
    for (AActor* Actor : SelectedActors)
    {
        if (ANode* Node = Cast<ANode>(Actor))
        {
            SelectedNodes.Add(Node);
        }
    }

    // Clear the LinkedNodes array and remove splines for each selected node
    for (ANode* Node : SelectedNodes)
    {
        Node->LinkedNodes.Empty();
        Node->UpdateLinkSplines();
        Node->Modify();
    }
#endif
}

void UNodeEditorUtility::AutoLink(float Distance)
{
#if WITH_EDITOR
    UWorld* World = GEditor ? GEditor->GetEditorWorldContext().World() : nullptr;
    if (!World) return;

    TArray<AActor*> SelectedActors;
    for (FSelectionIterator It(GEditor->GetSelectedActorIterator()); It; ++It)
    {
        if (AActor* Actor = Cast<AActor>(*It))
        {
            SelectedActors.Add(Actor);
        }
    }

    TArray<ANode*> SelectedNodes;
    for (AActor* Actor : SelectedActors)
    {
        if (ANode* Node = Cast<ANode>(Actor))
        {
            SelectedNodes.Add(Node);
        }
    }

    // Clear existing links and splines
    for (ANode* Node : SelectedNodes)
    {
        Node->LinkedNodes.Empty();
        Node->UpdateLinkSplines();
    }

    // Link nodes within distance (bidirectional)
    for (int32 i = 0; i < SelectedNodes.Num(); ++i)
    {
        ANode* NodeA = SelectedNodes[i];
        FVector PosA = NodeA->GetActorLocation();

        for (int32 j = i + 1; j < SelectedNodes.Num(); ++j)
        {
            ANode* NodeB = SelectedNodes[j];
            FVector PosB = NodeB->GetActorLocation();

            float Dist = FVector::Dist(PosA, PosB);
            if (Dist <= Distance)
            {
                if (!NodeA->LinkedNodes.Contains(NodeB))
                {
                    NodeA->LinkedNodes.Add(NodeB);
                }
                if (!NodeB->LinkedNodes.Contains(NodeA))
                {
                    NodeB->LinkedNodes.Add(NodeA);
                }
            }
        }
    }

    // Mark nodes as modified and update splines for all nodes
    for (ANode* Node : SelectedNodes)
    {
        Node->Modify();
        Node->UpdateLinkSplines();
    }
#endif
}

void UNodeEditorUtility::AutoUnlink(float Distance)
{
#if WITH_EDITOR
    UWorld* World = GEditor ? GEditor->GetEditorWorldContext().World() : nullptr;
    if (!World) return;

    TArray<AActor*> SelectedActors;
    for (FSelectionIterator It(GEditor->GetSelectedActorIterator()); It; ++It)
    {
        if (AActor* Actor = Cast<AActor>(*It))
        {
            SelectedActors.Add(Actor);
        }
    }

    TArray<ANode*> SelectedNodes;
    for (AActor* Actor : SelectedActors)
    {
        if (ANode* Node = Cast<ANode>(Actor))
        {
            SelectedNodes.Add(Node);
        }
    }

    // Unlink nodes within distance and remove splines
    for (int32 i = 0; i < SelectedNodes.Num(); ++i)
    {
        ANode* NodeA = SelectedNodes[i];
        FVector PosA = NodeA->GetActorLocation();

        for (int32 j = 0; j < SelectedNodes.Num(); ++j)
        {
            if (i == j) continue;
            ANode* NodeB = SelectedNodes[j];
            FVector PosB = NodeB->GetActorLocation();

            float Dist = FVector::Dist(PosA, PosB);
            if (Dist <= Distance)
            {
                NodeA->LinkedNodes.Remove(NodeB);
            }
        }
        NodeA->UpdateLinkSplines();
    }

    // Mark nodes as modified
    for (ANode* Node : SelectedNodes)
    {
        Node->Modify();
    }
#endif
}

void UNodeEditorUtility::UnlinkAll()
{
#if WITH_EDITOR
    UWorld* World = GEditor ? GEditor->GetEditorWorldContext().World() : nullptr;
    if (!World) return;

    for (TActorIterator<ANode> It(World); It; ++It)
    {
        ANode* Node = *It;
        if (Node)
        {
            Node->LinkedNodes.Empty();
            Node->UpdateLinkSplines();
            Node->Modify();
        }
    }
#endif
}
