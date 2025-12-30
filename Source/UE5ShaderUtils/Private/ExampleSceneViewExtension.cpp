/*
Copyright 2025 Ryan Sweeney

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include "ExampleSceneViewExtension.h"

#include "RenderPasses_v2/Raster/RasterColourChange.h"
#include "PostProcess/PostProcessing.h"
#include "RenderPasses_v2/Compute/ComputeColourChange.h"
#include "Runtime/Launch/Resources/Version.h"

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3
#include "PostProcess/PostProcessMaterialInputs.h"
#else
#include "PostProcess/PostProcessMaterial.h"
#endif

FExampleSceneViewExtension::FExampleSceneViewExtension(const FAutoRegister& AutoRegister) 
	: FSceneViewExtensionBase(AutoRegister)
{
	
}

FExampleSceneViewExtension::~FExampleSceneViewExtension()
{
}

void FExampleSceneViewExtension::PrePostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& InView,
	const FPostProcessingInputs& Inputs)
{
	FSceneViewExtensionBase::PrePostProcessPass_RenderThread(GraphBuilder, InView, Inputs);
		
	checkSlow(InView.bIsViewInfo);
	
	FGlobalShaderMap* GlobalShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);
	
	const FRDGTextureRef SceneColorTexture = (*Inputs.SceneTextures)->SceneColorTexture;
		
	// Create output texture based on input texture description
	// This doesn't save the output anywhere, just for demonstration purposes
	// In a real scenario, you would want to output to the scene color or another render target
	// You might choose to create the output texture in the passes themselves and output the result, return the texture etc
	FRDGTextureRef OutputTexture = GraphBuilder.CreateTexture(
		SceneColorTexture->Desc,
		TEXT("ExampleSceneViewExtension_OutputTexture"));
	
	ShaderPasses::Raster::ColourChange::AddPass(GraphBuilder, GlobalShaderMap, SceneColorTexture, OutputTexture);
	ShaderPasses::Compute::ColourChange::AddPass(GraphBuilder, GlobalShaderMap, SceneColorTexture, OutputTexture);
}
