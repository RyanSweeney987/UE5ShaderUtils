/*
Copyright 2025 Ryan Sweeney

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


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
