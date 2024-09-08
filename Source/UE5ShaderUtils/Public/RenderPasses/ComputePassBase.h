// Copyright Ryan Sweeney 2024, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "RenderGraphUtils.h"

namespace ComputePassBase
{
	static constexpr ETextureCreateFlags ComputeTextureCreationFlags = TexCreate_ShaderResource | TexCreate_UAV | TexCreate_FastVRAM | TexCreate_RenderTargetable;
}

/**
 *
 */
struct UE5SHADERUTILS_API FComputePassInputParams
{
	FRDGBuilder &GraphBuilder;
	const FGlobalShaderMap *GlobalShaderMap;

	FComputePassInputParams(FRDGBuilder &InGraphBuilder, const FGlobalShaderMap *InGlobalShaderMap)
		: GraphBuilder(InGraphBuilder), GlobalShaderMap(InGlobalShaderMap)
	{
	}
};

/**
 *
 */
struct UE5SHADERUTILS_API FComputePassOutputParams
{
	FRDGTextureRef Texture;

	FComputePassOutputParams(const FRDGTextureRef InTexture = nullptr)
		: Texture(InTexture)
	{
	}
};

/**
 *
 */
class UE5SHADERUTILS_API FComputePassBase
{
public:
	bool bNeverCull = false;
	bool bAsyncCompute = false;

public:
	FComputePassBase() = default;
	virtual ~FComputePassBase() = default;

	virtual FComputePassOutputParams AddPass(FComputePassInputParams &InputParams) = 0;

protected:
	template <typename TShaderClass>
	void AddComputePass(FRDGBuilder &GraphBuilder,
						const FGlobalShaderMap *GlobalShaderMap,
						FRDGEventName &&PassName,
						typename TShaderClass::FParameters *Parameters,
						const FIntVector &GroupCount)
	{
		ERDGPassFlags PassFlags = (bNeverCull ? ERDGPassFlags::NeverCull : ERDGPassFlags::None);
		PassFlags |= bAsyncCompute ? ERDGPassFlags::AsyncCompute : ERDGPassFlags::Compute;

		const FShaderParametersMetadata *ParametersMetadata = TShaderClass::FParameters::FTypeInfo::GetStructMetadata();
		const TShaderRef<TShaderClass> ComputeShader = TShaderMapRef<TShaderClass>(GlobalShaderMap);

		checkf(ComputeShader.IsValid(), TEXT("Invalid compute shader"));

		ClearUnusedGraphResources(ComputeShader, Parameters);

		GraphBuilder.AddPass(
			Forward<FRDGEventName>(PassName),
			Parameters,
			PassFlags,
			[Parameters, ComputeShader, ParametersMetadata, GroupCount](FRHIComputeCommandList &RHICmdList)
			{
				ensure(GroupCount.X <= GRHIMaxDispatchThreadGroupsPerDimension.X);
				ensure(GroupCount.Y <= GRHIMaxDispatchThreadGroupsPerDimension.Y);
				ensure(GroupCount.Z <= GRHIMaxDispatchThreadGroupsPerDimension.Z);

				FComputeShaderUtils::Dispatch(RHICmdList, ComputeShader, ParametersMetadata, *Parameters, GroupCount);
			});
	}

	template <typename TShaderClass>
	void AddIndirectComputePass(FRDGBuilder &GraphBuilder,
								const FGlobalShaderMap *GlobalShaderMap,
								FRDGEventName &&PassName,
								typename TShaderClass::FParameters *Parameters,
								FRDGBufferRef IndirectArgsBuffer,
								uint32 IndirectArgsOffset)
	{
		ERDGPassFlags PassFlags = (bNeverCull ? ERDGPassFlags::NeverCull : ERDGPassFlags::None);
		PassFlags |= bAsyncCompute ? ERDGPassFlags::AsyncCompute : ERDGPassFlags::Compute;

		const TShaderRef<TShaderClass> ComputeShader = TShaderMapRef<TShaderClass>(GlobalShaderMap);
		checkf(ComputeShader.IsValid(), TEXT("Invalid compute shader"));

		GraphBuilder.AddPass(
			Forward<FRDGEventName>(PassName),
			Parameters,
			PassFlags,
			[Parameters, ComputeShader, IndirectArgsBuffer, IndirectArgsOffset](FRHIComputeCommandList &RHICmdList)
			{
				FComputeShaderUtils::DispatchIndirect(RHICmdList, ComputeShader, *Parameters, IndirectArgsBuffer, IndirectArgsOffset);
			});
	}
};
