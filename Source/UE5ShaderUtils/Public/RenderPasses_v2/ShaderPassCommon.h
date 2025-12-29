// Copyright Ryan Sweeney 2025, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "RenderGraphBuilder.h"
#include "RHIResources.h"

class FShaderPassBase
{
protected:
	FRDGBuilder& GraphBuilder;
	
	// Changeable settings
	bool bNeverCull = false;
	bool bAsyncCompute = false;

public:
	FShaderPassBase(FRDGBuilder& InGraphBuilder) : 
		GraphBuilder(InGraphBuilder)
	{}
	FShaderPassBase(FRDGBuilder& InGraphBuilder, const bool bNeverCull) : GraphBuilder(InGraphBuilder), bNeverCull(bNeverCull)
	{}
	FShaderPassBase(const FShaderPassBase& InPass) : GraphBuilder(InPass.GraphBuilder), bNeverCull(InPass.bNeverCull), bAsyncCompute(InPass.bAsyncCompute)
	{}
	virtual ~FShaderPassBase() = default;

	virtual FShaderPassBase& SetNeverCull(const bool bInNeverCull);
	virtual FShaderPassBase& SetAsyncCompute(const bool bInAsyncCompute);
protected:
	ERDGPassFlags GetRasterFlags() const;
	ERDGPassFlags GetComputeFlags() const;
};