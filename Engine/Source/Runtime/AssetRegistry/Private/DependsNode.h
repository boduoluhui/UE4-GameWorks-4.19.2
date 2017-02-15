// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AssetData.h"
#include "Misc/AssetRegistryInterface.h"

/** Implementation of IDependsNode */
class FDependsNode
{
public:
	FDependsNode() {}
	FDependsNode(const FAssetIdentifier& InIdentifier) : Identifier(InIdentifier) {}

	/** Prints the dependencies and referencers for this node to the log */
	void PrintNode() const;
	/** Prints the dependencies for this node to the log */
	void PrintDependencies() const;
	/** Prints the referencers to this node to the log */
	void PrintReferencers() const;
	/** Gets the list of dependencies for this node */
	void GetDependencies(TArray<FDependsNode*>& OutDependencies, EAssetRegistryDependencyType::Type InDependencyType = EAssetRegistryDependencyType::All) const;
	/** Gets the list of dependency names for this node */
	void GetDependencies(TArray<FAssetIdentifier>& OutDependencies, EAssetRegistryDependencyType::Type InDependencyType = EAssetRegistryDependencyType::All) const;
	/** Gets the list of referencers to this node */
	void GetReferencers(TArray<FDependsNode*>& OutReferencers) const;
	/** Gets the name of the package that this node represents */
	FName GetPackageName() const { return Identifier.PackageName; }
	/** Sets the name of the package that this node represents */
	void SetPackageName(FName InName) { Identifier = FAssetIdentifier(InName); }
	/** Returns the entire identifier */
	FAssetIdentifier GetIdentifier() const { return Identifier; }
	/** Sets the entire identifier */
	void SetIdentifier(const FAssetIdentifier& InIdentifier) { Identifier = InIdentifier;  }
	/** Add a dependency to this node */
	void AddDependency(FDependsNode* InDependency, EAssetRegistryDependencyType::Type InDependencyType = EAssetRegistryDependencyType::Hard)
	{ 
		if (InDependencyType == EAssetRegistryDependencyType::Hard)
		{
			HardDependencies.AddUnique(InDependency);
		}
		if (InDependencyType == EAssetRegistryDependencyType::Soft)
		{
			SoftDependencies.AddUnique(InDependency);
		}
		if (InDependencyType == EAssetRegistryDependencyType::SearchableName)
		{
			NameDependencies.AddUnique(InDependency);
		}
	}
	/** Add a referencer to this node */
	void AddReferencer(FDependsNode* InReferencer) { Referencers.AddUnique(InReferencer); }
	/** Remove a dependency from this node */
	void RemoveDependency(FDependsNode* InDependency) { HardDependencies.Remove(InDependency); SoftDependencies.Remove(InDependency); NameDependencies.Remove(InDependency); }
	/** Remove a referencer from this node */
	void RemoveReferencer(FDependsNode* InReferencer) { Referencers.Remove(InReferencer); }
	/** Clear all dependency records from this node */
	void ClearDependencies() { HardDependencies.Empty(); SoftDependencies.Empty(); NameDependencies.Empty(); }

	/** Iterate over all the dependencies of this node, filtered by the supplied type parameter, and call the supplied lambda parameter on the record */
	template <class T>
	void IterateOverDependencies(T InCallback, EAssetRegistryDependencyType::Type InDependencyType = EAssetRegistryDependencyType::All)
	{
		if (InDependencyType & EAssetRegistryDependencyType::Hard)
		{
			for (FDependsNode* Dependency : HardDependencies)
			{
				InCallback(Dependency, EAssetRegistryDependencyType::Hard);
			}
		}

		if (InDependencyType & EAssetRegistryDependencyType::Soft)
		{
			for (FDependsNode* Dependency : SoftDependencies)
			{
				InCallback(Dependency, EAssetRegistryDependencyType::Soft);
			}
		}

		if (InDependencyType & EAssetRegistryDependencyType::SearchableName)
		{
			for (FDependsNode* Dependency : NameDependencies)
			{
				InCallback(Dependency, EAssetRegistryDependencyType::SearchableName);
			}
		}
	}

	/** Iterate over all the dependencies of this node, filtered by the supplied type parameter, and call the supplied lambda parameter on the record */
	template <class T>
	void IterateOverDependencies(T InCallback, EAssetRegistryDependencyType::Type InDependencyType = EAssetRegistryDependencyType::All) const
	{
		if (InDependencyType & EAssetRegistryDependencyType::Hard)
		{
			for (FDependsNode* Dependency : HardDependencies)
			{
				InCallback(Dependency, EAssetRegistryDependencyType::Hard);
			}
		}

		if (InDependencyType & EAssetRegistryDependencyType::Soft)
		{
			for (FDependsNode* Dependency : SoftDependencies)
			{
				InCallback(Dependency, EAssetRegistryDependencyType::Soft);
			}
		}

		if (InDependencyType & EAssetRegistryDependencyType::SearchableName)
		{
			for (FDependsNode* Dependency : NameDependencies)
			{
				InCallback(Dependency, EAssetRegistryDependencyType::SearchableName);
			}
		}
	}

	/** Iterate over all the referencers of this node and call the supplied lambda
		parameter on the record */
	template <class T>
	void IterateOverReferencers(T InCallback)
	{
		for (FDependsNode* Referencer : Referencers)
		{
			InCallback(Referencer);
		}
	}

	void Reserve(int32 InNumHardDependencies, int32 InNumSoftDependencies, int32 InNumReferencers)
	{
		HardDependencies.Reserve(InNumHardDependencies);
		SoftDependencies.Reserve(InNumSoftDependencies);
		Referencers.Reserve(InNumReferencers);
	}

private:
	/** Recursively prints dependencies of the node starting with the specified indent. VisitedNodes should be an empty set at first which is populated recursively. */
	void PrintDependenciesRecursive(const FString& Indent, TSet<const FDependsNode*>& VisitedNodes) const;
	/** Recursively prints referencers to the node starting with the specified indent. VisitedNodes should be an empty set at first which is populated recursively. */
	void PrintReferencersRecursive(const FString& Indent, TSet<const FDependsNode*>& VisitedNodes) const;

	/** The name of the package/object this node represents */
	FAssetIdentifier Identifier;
	/** The list of hard dependencies for this node */
	TArray<FDependsNode*> HardDependencies;
	/** The list of soft dependencies for this node */
	TArray<FDependsNode*> SoftDependencies;
	/** The list of searchable name dependencies for this node */
	TArray<FDependsNode*> NameDependencies;
	/** The list of referencers to this node */
	TArray<FDependsNode*> Referencers;
};
