/************************************************************
*   Copyright (C) Ryan Sweeney 2025, All Rights Reserved   *
 ************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "PixelShaderUtils.h"
#include "ShaderPassCommon.h"
#include "Runtime/Launch/Resources/Version.h"


/**
 * Utility class for creating render graph passes
 * Allows the option to never cull and run async compute passes
 */
class FShaderPass : public FShaderPassBase
{
public:
	/**
	 * Optional parameters for the shader pass
	 * This allows for setting blend states, rasterizer states, depth stencil states and stencil reference values
	 * These are used to set the render states for the pass
	 * @tparam TShaderClass The shader class that is being used for the pass
	 */
	struct FOptionalParams
	{
		FRHIBlendState* BlendState = nullptr;
		FRHIRasterizerState* RasterizerState = nullptr;
		FRHIDepthStencilState* DepthStencilState = nullptr;
		uint32 StencilRef = 0;
	};
private:
	BEGIN_SHADER_PARAMETER_STRUCT(FShaderPassCopyTextureParameters, )
		RDG_TEXTURE_ACCESS(Input,  ERHIAccess::CopySrc)
		RDG_TEXTURE_ACCESS(Output, ERHIAccess::CopyDest)
	END_SHADER_PARAMETER_STRUCT()
protected:
	const FGlobalShaderMap* GlobalShaderMap;
public:
	FShaderPass(FRDGBuilder& InGraphBuilder, const FGlobalShaderMap* InGlobalShaderMap) : FShaderPassBase(InGraphBuilder),
		GlobalShaderMap(InGlobalShaderMap)
	{}
	FShaderPass(FRDGBuilder& InGraphBuilder, const FGlobalShaderMap* InGlobalShaderMap, const bool bNeverCull) : FShaderPassBase(InGraphBuilder, bNeverCull), GlobalShaderMap(InGlobalShaderMap)
	{}
	FShaderPass(const FShaderPass& InPass) : FShaderPassBase(InPass), GlobalShaderMap(InPass.GlobalShaderMap)
	{}

	virtual FShaderPass& SetNeverCull(const bool bInNeverCull) override;
	virtual FShaderPass& SetAsyncCompute(const bool bInAsyncCompute) override;
	
	/**
	 * Adds a pass to the render graph with the given parameters and shader
	 * Returns a copy of the current pass object so that multiple passes can be chained
	 * whilst keeping the same settings or being able to change them
	 * @tparam TShaderClass Class used for the shader & params
	 * @param PassName Event name for the pass
	 * @param Parameters 
	 * @param Viewport
	 * @param OptionalParams Optional parameters for the pass
	 * @param OptionalParams Optional parameters for the pass
	 * @return A copy of the current pass object
	 */
	template<typename TShaderClass>
	FShaderPass AddRenderPass(
		const FIntPoint& Viewport,
		FRDGEventName&& PassName,
		typename TShaderClass::FParameters* Parameters,
		const FOptionalParams OptionalParams = FOptionalParams())
	{

		checkf(IsInRenderingThread() || IsInRHIThread(), TEXT("This method cannot be run outside of the render thread"));

		const TShaderRef<TShaderClass> PixelShader = TShaderMapRef<TShaderClass>(GlobalShaderMap);
		checkf(PixelShader.IsValid(), TEXT("Invalid Pixel Shader"));

		ClearUnusedGraphResources(PixelShader, Parameters);

		ERDGPassFlags PassFlags = ERDGPassFlags::Raster | (bNeverCull ? ERDGPassFlags::NeverCull : ERDGPassFlags::None);

		const FIntRect ViewportRect = FIntRect(FIntPoint::ZeroValue, Viewport.Size());
		
		GraphBuilder.AddPass(
			Forward<FRDGEventName>(PassName),
			Parameters,
			PassFlags,
			[Parameters, GlobalShaderMap = this->GlobalShaderMap, PixelShader, ViewportRect, OptionalParams](FRHICommandList& RHICmdList)
		{;
			FPixelShaderUtils::DrawFullscreenPixelShader(RHICmdList, GlobalShaderMap, PixelShader, *Parameters, ViewportRect, 
				OptionalParams.BlendState, OptionalParams.RasterizerState, OptionalParams.DepthStencilState, OptionalParams.StencilRef);
		});

		// Return a copy with the same values
		return FShaderPass(*this);
	}

	template<typename TShaderClass>
	FShaderPass AddRenderPass(
		const FIntPoint& Viewport,
		FRDGEventName&& PassName,
		typename TShaderClass::FParameters* Parameters,
		const typename TShaderClass::FPermutationDomain PermutationVector,
		const FOptionalParams OptionalParams = FOptionalParams())
	{
		checkf(IsInRenderingThread() || IsInRHIThread(), TEXT("This method cannot be run outside of the render thread"));

		const TShaderRef<TShaderClass> PixelShader = GlobalShaderMap->GetShader<TShaderClass>(PermutationVector);
		checkf(PixelShader.IsValid(), TEXT("Invalid Pixel Shader"));

		ClearUnusedGraphResources(PixelShader, Parameters);

		ERDGPassFlags PassFlags = GetRasterFlags();

		const FIntRect ViewportRect = FIntRect(FIntPoint::ZeroValue, Viewport.Size());

		// FRDGAsyncTask
		
		GraphBuilder.AddPass(
			Forward<FRDGEventName&&>(PassName),
			Parameters,
			PassFlags,
			[Parameters, GlobalShaderMap = this->GlobalShaderMap, PixelShader, ViewportRect, OptionalParams](FRHICommandList& RHICmdList)
		{;
			FPixelShaderUtils::DrawFullscreenPixelShader(RHICmdList, GlobalShaderMap, PixelShader, *Parameters, ViewportRect, 
				OptionalParams.BlendState, OptionalParams.RasterizerState, OptionalParams.DepthStencilState, OptionalParams.StencilRef);
		});

		// Return a copy with the same values
		return FShaderPass(*this);
	}

	template<typename TShaderClass>
		FShaderPass AddComputePass(
			const FIntVector& GroupCount,
			FRDGEventName&& PassName,
			typename TShaderClass::FParameters* Parameters)
	{
		checkf(IsInRenderingThread() || IsInRHIThread(), TEXT("This method cannot be run outside of the render thread"));
		
		ERDGPassFlags PassFlags = GetComputeFlags();

		// Metadata for the shader parameters required for compute dispatch
		const FShaderParametersMetadata* ParametersMetadata = TShaderClass::FParameters::FTypeInfo::GetStructMetadata();

		const TShaderRef<TShaderClass> ComputeShader = TShaderMapRef<TShaderClass>(GlobalShaderMap);
		checkf(ComputeShader.IsValid(), TEXT("Invalid compute shader"));
		
		ClearUnusedGraphResources(ComputeShader, Parameters);
	
		GraphBuilder.AddPass(
			Forward<FRDGEventName&&>(PassName),
			Parameters,
			PassFlags,
			[Parameters, ComputeShader, ParametersMetadata, GroupCount](FRHIComputeCommandList& RHICmdList)
		{
			ensure(GroupCount.X <= GRHIMaxDispatchThreadGroupsPerDimension.X);
			ensure(GroupCount.Y <= GRHIMaxDispatchThreadGroupsPerDimension.Y);
			ensure(GroupCount.Z <= GRHIMaxDispatchThreadGroupsPerDimension.Z);
				
			FComputeShaderUtils::Dispatch(RHICmdList, ComputeShader, ParametersMetadata, *Parameters, GroupCount);
		});

		// Return a copy with the same values
		return FShaderPass(*this);
	}
	
	template<typename TShaderClass>
    FShaderPass AddComputePass(
		const FIntVector& GroupCount,
		FRDGEventName&& PassName,
		typename TShaderClass::FParameters* Parameters,
		typename TShaderClass::FPermutationDomain PermutationVector)
	{
		checkf(IsInRenderingThread() || IsInRHIThread(), TEXT("This method cannot be run outside of the render thread"));
		
		ERDGPassFlags PassFlags = GetComputeFlags();

		// Metadata for the shader parameters required for compute dispatch
		const FShaderParametersMetadata* ParametersMetadata = TShaderClass::FParameters::FTypeInfo::GetStructMetadata();

		TShaderRef<TShaderClass> ComputeShader = GlobalShaderMap->GetShader<TShaderClass>(PermutationVector);
		checkf(ComputeShader.IsValid(), TEXT("Invalid compute shader"));
		
		ClearUnusedGraphResources(ComputeShader, Parameters);
	
		GraphBuilder.AddPass(
			Forward<FRDGEventName&&>(PassName),
			Parameters,
			PassFlags,
			[Parameters, ComputeShader, ParametersMetadata, GroupCount](FRHIComputeCommandList& RHICmdList)
		{
			ensure(GroupCount.X <= GRHIMaxDispatchThreadGroupsPerDimension.X);
			ensure(GroupCount.Y <= GRHIMaxDispatchThreadGroupsPerDimension.Y);
			ensure(GroupCount.Z <= GRHIMaxDispatchThreadGroupsPerDimension.Z);
				
			FComputeShaderUtils::Dispatch(RHICmdList, ComputeShader, ParametersMetadata, *Parameters, GroupCount);
		});

		// Return a copy with the same values
		return FShaderPass(*this);
	}

	template<typename TShaderClass>
	FShaderPass AddIndirectComputePass(
		FRDGEventName&& PassName,
		typename TShaderClass::FParameters* Parameters,
		FRDGBufferRef IndirectArgsBuffer,
		uint32 IndirectArgsOffset)
	{
		ERDGPassFlags PassFlags = GetComputeFlags();
		
		const TShaderRef<TShaderClass> ComputeShader = TShaderMapRef<TShaderClass>(GlobalShaderMap);
		checkf(ComputeShader.IsValid(), TEXT("Invalid compute shader"));
		
		ClearUnusedGraphResources(ComputeShader, Parameters);
		
		GraphBuilder.AddPass(
			Forward<FRDGEventName&&>(PassName),
			Parameters,
			PassFlags,
			[Parameters, ComputeShader, IndirectArgsBuffer, IndirectArgsOffset](FRHIComputeCommandList& RHICmdList)
		{
			FComputeShaderUtils::DispatchIndirect(RHICmdList, ComputeShader, *Parameters, IndirectArgsBuffer, IndirectArgsOffset);
		});

		// Return a copy with the same values
		return FShaderPass(*this);
	}
	
	template<typename TShaderClass>
	FShaderPass AddIndirectComputePass(
		FRDGEventName&& PassName,
		typename TShaderClass::FParameters* Parameters,
		FRDGBufferRef IndirectArgsBuffer,
		uint32 IndirectArgsOffset,
		typename TShaderClass::FPermutationDomain PermutationVector)
	{
		ERDGPassFlags PassFlags = GetComputeFlags();
		
		TShaderRef<TShaderClass> ComputeShader = GlobalShaderMap->GetShader<TShaderClass>(PermutationVector);
		checkf(ComputeShader.IsValid(), TEXT("Invalid compute shader"));
		
		ClearUnusedGraphResources(ComputeShader, Parameters);
		
		GraphBuilder.AddPass(
			Forward<FRDGEventName&&>(PassName),
			Parameters,
			PassFlags,
			[Parameters, ComputeShader, IndirectArgsBuffer, IndirectArgsOffset](FRHIComputeCommandList& RHICmdList)
		{
			FComputeShaderUtils::DispatchIndirect(RHICmdList, ComputeShader, *Parameters, IndirectArgsBuffer, IndirectArgsOffset);
		});

		// Return a copy with the same values
		return FShaderPass(*this);
	}

	/**
	 * Adds a copy texture pass to the render graph
	 * This is used to copy a texture from one to another
	 * Custom method to allow never culling
	 * Performs a full copy of the texture
	 * @param InputTexture The texture to copy from
	 * @param OutputTexture The texture to copy to
	 * @return A copy of the current pass object
	 */
	FShaderPass AddFullCopyTexturePass(const FRDGTextureRef InputTexture, const FRDGTextureRef OutputTexture)
	{
		if (InputTexture == OutputTexture)
		{
			return FShaderPass(*this);
		}

		const FRDGTextureDesc& InputDesc = InputTexture->Desc;
		const FRDGTextureDesc& OutputDesc = OutputTexture->Desc;
		checkf(InputDesc.Format == OutputDesc.Format, TEXT("This method does not support format conversion."));
		
		FRHICopyTextureInfo CopyInfo;
		CopyInfo.SourcePosition.X = 0;
		CopyInfo.SourcePosition.Y = 0;
		CopyInfo.DestPosition.X = 0;
		CopyInfo.DestPosition.Y = 0;
		CopyInfo.Size = FIntVector(InputDesc.Extent.X, InputDesc.Extent.Y, 1);
		
		FShaderPassCopyTextureParameters* Parameters = GraphBuilder.AllocParameters<FShaderPassCopyTextureParameters>();
		Parameters->Input = InputTexture;
		Parameters->Output = OutputTexture;

		ERDGPassFlags PassFlags = ERDGPassFlags::Copy;
		PassFlags |= GetRasterFlags();
		
		GraphBuilder.AddPass(
		RDG_EVENT_NAME("Full CopyTexture(%s -> %s)", InputTexture->Name, OutputTexture->Name),
		Parameters,
		PassFlags,
		[InputTexture, OutputTexture, CopyInfo](
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 5
			FRDGAsyncTask,
#endif
			FRHICommandList& RHICmdList)
		{
			RHICmdList.CopyTexture(InputTexture->GetRHI(), OutputTexture->GetRHI(), CopyInfo);
		});

		return FShaderPass(*this);
	}
};


