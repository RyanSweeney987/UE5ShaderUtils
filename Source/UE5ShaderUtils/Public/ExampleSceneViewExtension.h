// Copyright Ryan Sweeney 2025, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "SceneViewExtension.h"

/**
 * 
 */
class UE5SHADERUTILS_API FExampleSceneViewExtension : public FSceneViewExtensionBase
{
public:
	FExampleSceneViewExtension(const FAutoRegister& AutoRegister);
	virtual ~FExampleSceneViewExtension() override;

	//-----------------------------------------------------------------------------------
	// Scene View Extension Implementation
	//-----------------------------------------------------------------------------------
	virtual int32 GetPriority() const override { return 1; }
	
	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override {}
	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override {}
	// virtual void SetupViewPoint(APlayerController* Player, FMinimalViewInfo& InViewInfo) {}
	// virtual void SetupViewProjectionMatrix(FSceneViewProjectionData& InOutProjectionData) {}
	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override {}
	// virtual void PostCreateSceneRenderer(const FSceneViewFamily& InViewFamily, ISceneRenderer* Renderer) {}
	// virtual void PreRenderViewFamily_RenderThread(FRDGBuilder& GraphBuilder, FSceneViewFamily& InViewFamily) {}
	virtual void PreRenderView_RenderThread(FRDGBuilder& GraphBuilder, FSceneView& InView) override {}
	// virtual void PreInitViews_RenderThread(FRDGBuilder& GraphBuilder) {}
	// virtual void PreRenderBasePass_RenderThread(FRDGBuilder& GraphBuilder, bool bDepthBufferIsPopulated) {}
	// virtual void PostRenderBasePassDeferred_RenderThread(FRDGBuilder& GraphBuilder, FSceneView& InView, const FRenderTargetBindingSlots& RenderTargets, TRDGUniformBufferRef<FSceneTextureUniformParameters> SceneTextures) {}
	// virtual void PostRenderBasePassMobile_RenderThread(FRHICommandList& RHICmdList, FSceneView& InView) {}
	virtual void PrePostProcessPass_RenderThread(FRDGBuilder& GraphBuilder, const FSceneView& InView, const FPostProcessingInputs& Inputs) override;

	// virtual void SubscribeToPostProcessingPass(EPostProcessingPass Pass, const FSceneView& InView, FAfterPassCallbackDelegateArray& InOutPassCallbacks, bool bIsPassEnabled) override;

	//-----------------------------------------------------------------------------------
};
