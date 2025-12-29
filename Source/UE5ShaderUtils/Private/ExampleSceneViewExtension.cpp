// Copyright Ryan Sweeney 2025, All Rights Reserved


#include "ExampleSceneViewExtension.h"


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
}
