/*
Copyright 2025 Ryan Sweeney

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include "CoreMinimal.h"
#include "ShaderParameterStruct.h"

BEGIN_SHADER_PARAMETER_STRUCT(FRasterColourChangeParameters,)
	SHADER_PARAMETER_RDG_TEXTURE(Texture2D<float4>, InputTexture)

	RENDER_TARGET_BINDING_SLOTS()
END_SHADER_PARAMETER_STRUCT()

class FRasterColourChangePS : public FGlobalShader
{	
	DECLARE_EXPORTED_SHADER_TYPE(FRasterColourChangePS, Global, );
	using FParameters = FRasterColourChangeParameters;
	SHADER_USE_PARAMETER_STRUCT(FRasterColourChangePS, FGlobalShader);
};

namespace ShaderPasses::Raster::ColourChange
{
	void AddPass(FRDGBuilder& GraphBuilder, const FGlobalShaderMap* GlobalShaderMap, const FRDGTextureRef InputTexture, FRDGTextureRef& OutputTexture);
}