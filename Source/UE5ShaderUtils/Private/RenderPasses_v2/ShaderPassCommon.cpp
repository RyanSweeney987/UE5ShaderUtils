/************************************************************
 *   Copyright (C) Ryan Sweeney 2025, All Rights Reserved   *
 ************************************************************/


#include "RenderPasses_v2/ShaderPassCommon.h"
#include "RHIStaticStates.h"
#include "RenderGraphDefinitions.h"

/**
 * Sets the pass to never cull
 * @param bInNeverCull 
 * @return The current pass object
 */
FShaderPassBase& FShaderPassBase::SetNeverCull(const bool bInNeverCull)
{
	// Only enable the ability to never cull in the editor
#if !UE_EDITOR
	bNeverCull = false;
#else
	bNeverCull = bInNeverCull;
#endif
	return *this;
}

/**
 * Sets the pass to be async compute
 * @param bInAsyncCompute 
 * @return The current pass object
 */
FShaderPassBase& FShaderPassBase::SetAsyncCompute(const bool bInAsyncCompute)
{
	bAsyncCompute = bInAsyncCompute;
	return *this;
}

ERDGPassFlags FShaderPassBase::GetRasterFlags() const
{
	const ERDGPassFlags Culling = (bNeverCull ? ERDGPassFlags::NeverCull : ERDGPassFlags::None);
	return ERDGPassFlags::Raster | Culling;
}

ERDGPassFlags FShaderPassBase::GetComputeFlags() const
{
	const ERDGPassFlags Culling = (bNeverCull ? ERDGPassFlags::NeverCull : ERDGPassFlags::None);
	const ERDGPassFlags Compute = (bAsyncCompute ? ERDGPassFlags::AsyncCompute : ERDGPassFlags::Compute);
	return Culling | Compute;
}
