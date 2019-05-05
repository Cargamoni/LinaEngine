/*
Author: Inan Evin
www.inanevin.com

Copyright 2018 Inan Evin

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, 
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions 
and limitations under the License.

Class: GameRenderContext
Timestamp: 4/27/2019 5:53:43 PM

*/

#pragma once

#ifndef GameRenderContext_HPP
#define GameRenderContext_HPP

#include "RenderContext.hpp"

namespace LinaEngine::Graphics
{
	class GameRenderContext : public RenderContext
	{
	public:

		GameRenderContext() : RenderContext() {};
		virtual ~GameRenderContext() {};

		FORCEINLINE void Construct(PAMRenderDevice& deviceIn, RenderTarget& targetIn, DrawParams& drawParamsIn, Shader& shaderIn, Sampler& samplerIn, const Matrix& perspectiveIn)
		{
			RenderContext::Construct(deviceIn, targetIn);
			m_DrawParams = &drawParamsIn;
			m_Shader = &shaderIn;
			m_Sampler = &samplerIn;
			m_Perspective = perspectiveIn;
		}

		FORCEINLINE void RenderMesh(VertexArray& vertexArray, Texture& texture, const Matrix& transformIn)
		{
			// Add the new matrix to the same pairs, each pair will be drawn once.
			m_MeshRenderBuffer[LinaMakePair(&vertexArray, &texture)].push_back(m_Perspective * transformIn);
		}

		FORCEINLINE void UpdatePerspective(const Matrix& newPerspective)
		{
			m_Perspective = newPerspective;
		}

		void Flush();

	private:

		DrawParams* m_DrawParams;
		Shader* m_Shader;
		Sampler *m_Sampler;
		Matrix m_Perspective;

		// Map to see the list of same vertex array & textures to compress them into single draw call.
		LinaMap<LinaPair<VertexArray*, Texture*>, LinaArray<Matrix> > m_MeshRenderBuffer;

	};
}


#endif