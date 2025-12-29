// Fill out your copyright notice in the Description page of Project Settings.


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
