/************************************************************
 *   Copyright (C) Ryan Sweeney 2025, All Rights Reserved   *
 ************************************************************/


#include "RenderPasses_v2/ShaderPass.h"

// NOTE: Covariant return types

/**
 * Sets the pass to never cull
 * @param bInNeverCull 
 * @return 
 */
FShaderPass& FShaderPass::SetNeverCull(const bool bInNeverCull)
{
	FShaderPassBase::SetNeverCull(bInNeverCull);
	return *this;
}

/**
 * Sets the pass to run as an async compute pass
 * @param bInAsyncCompute 
 * @return 
 */
FShaderPass& FShaderPass::SetAsyncCompute(const bool bInAsyncCompute)
{
	FShaderPassBase::SetAsyncCompute(bInAsyncCompute);
	return *this;
}
