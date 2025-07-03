#include "NodeEditorUtility.h"
#include "Node.h"
#include "Editor.h"
#include "Engine/Selection.h"

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
            }
        }
        // Mark the node as changed so the editor saves the update
        Node->Modify();
    }
#endif
}

void UNodeEditorUtility::Unlink()
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

    // Clear the LinkedNodes array for each selected node
    for (ANode* Node : SelectedNodes)
    {
        Node->LinkedNodes.Empty();
        Node->Modify();
    }
#endif
}

void UNodeEditorUtility::AutoLink(float Distance)
{
#if WITH_EDITOR
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

    // Clear existing links
    for (ANode* Node : SelectedNodes)
    {
        Node->LinkedNodes.Empty();
    }

    // Link nodes within distance
    for (int32 i = 0; i < SelectedNodes.Num(); ++i)
    {
        ANode* NodeA = SelectedNodes[i];
        FVector PosA = NodeA->GetActorLocation();

        for (int32 j = i + 1; j < SelectedNodes.Num(); ++j)
        {
            ANode* NodeB = SelectedNodes[j];
            FVector PosB = NodeB->GetActorLocation();

            float Distance = FVector::Dist(PosA, PosB);
            if (Distance <= Distance)
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

    // Mark nodes as modified
    for (ANode* Node : SelectedNodes)
    {
        Node->Modify();
    }
#endif
}

void UNodeEditorUtility::UnlinkWithinDistance(float Distance)
{
#if WITH_EDITOR
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

    // Unlink nodes within distance
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
    }

    // Mark nodes as modified
    for (ANode* Node : SelectedNodes)
    {
        Node->Modify();
    }
#endif
}

