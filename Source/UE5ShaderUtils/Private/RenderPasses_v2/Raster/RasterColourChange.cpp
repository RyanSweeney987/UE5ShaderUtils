/*
Copyright 2025 Ryan Sweeney

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include "RenderPasses_v2/Raster/RasterColourChange.h"

#include "RenderPasses_v2/ShaderPass.h"

#define COLOUR_CHANGE TEXT("/ExampleShadersVirtualLocation/Private/ColourChange.usf")

IMPLEMENT_SHADER_TYPE(, FRasterColourChangePS, COLOUR_CHANGE, TEXT("ColourChangePS"), SF_Pixel);

#undef COLOUR_CHANGE

namespace ShaderPasses::Raster::ColourChange
{
	/**
	 * For simple colour changes
	 * @param GraphBuilder 
	 * @param GlobalShaderMap 
	 * @param InputTexture 
	 * @param OutputTexture
	 */
	void AddPass(FRDGBuilder& GraphBuilder, const FGlobalShaderMap* GlobalShaderMap, const FRDGTextureRef InputTexture, FRDGTextureRef& OutputTexture)
	{
		FRasterColourChangeParameters* Parameters = GraphBuilder.AllocParameters<FRasterColourChangeParameters>();
		Parameters->InputTexture = InputTexture;
		Parameters->RenderTargets[0] = FRenderTargetBinding(OutputTexture, ERenderTargetLoadAction::ENoAction);

		FShaderPass(GraphBuilder, GlobalShaderMap)
		.SetNeverCull(true)
		.AddRenderPass<FRasterColourChangePS>(InputTexture->Desc.Extent, FRDGEventName(TEXT("Raster Colour Change Pass")), Parameters);
	}
}