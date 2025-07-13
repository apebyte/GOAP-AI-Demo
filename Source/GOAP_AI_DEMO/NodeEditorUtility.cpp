/**
 * @file NodeEditorUtility.cpp
 * @brief Implements static editor utility functions for linking and unlinking navigation nodes in the Unreal Editor.
 *
 * This file provides implementations for functions that allow users to link, unlink, auto-link, auto-unlink, and clear links
 * between selected nodes or all nodes in the editor. These utilities are intended to assist with navigation graph editing.
 */

#include "NodeEditorUtility.h"
#include "Node.h"
#include "Editor.h"
#include "Engine/Selection.h"
#include "EngineUtils.h"

/**
 * @brief Links all selected nodes to each other (bidirectional).
 *
 * For each selected node, creates links to all other selected nodes if not already linked.
 * Updates link splines for visualization.
 */
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

/**
 * @brief Unlinks selected nodes from each other (bidirectional).
 *
 * For each pair of selected nodes, removes the link between them if it exists.
 * Updates link splines for visualization.
 */
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

    // Only unlink between selected nodes, not all links
    for (int32 i = 0; i < SelectedNodes.Num(); ++i)
    {
        for (int32 j = 0; j < SelectedNodes.Num(); ++j)
        {
            if (i == j) continue;
            ANode* NodeA = SelectedNodes[i];
            ANode* NodeB = SelectedNodes[j];

            if (NodeA->LinkedNodes.Contains(NodeB))
            {
                NodeA->LinkedNodes.Remove(NodeB);
                NodeA->Modify();
            }
        }
    }

    // Update splines for all affected nodes
    for (ANode* Node : SelectedNodes)
    {
        Node->UpdateLinkSplines();
    }
#endif
}

/**
 * @brief Automatically links selected nodes that are within a specified distance of each other.
 *
 * Clears existing links for selected nodes, then links nodes that are within the given distance.
 * Updates link splines for visualization.
 * @param Distance The maximum distance between nodes to be linked.
 */
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

/**
 * @brief Automatically unlinks selected nodes that are within a specified distance of each other.
 *
 * For each selected node, removes links to other selected nodes that are within the given distance.
 * Updates link splines for visualization.
 * @param Distance The maximum distance between nodes to be unlinked.
 */
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

/**
 * @brief Unlinks all nodes in the world from each other.
 *
 * Clears the LinkedNodes array for every node in the world and updates link splines.
 */
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

/**
 * @brief Clears all links for each selected node.
 *
 * For every selected node, removes all links (LinkedNodes) and updates link splines.
 */
void UNodeEditorUtility::ClearSelectedNodeLinks()
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

    for (ANode* Node : SelectedNodes)
    {
        Node->LinkedNodes.Empty();
        Node->UpdateLinkSplines();
        Node->Modify();
    }
#endif
}
