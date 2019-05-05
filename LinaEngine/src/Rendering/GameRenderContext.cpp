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
Timestamp: 4/27/2019 5:54:04 PM

*/

#include "LinaPch.hpp"
#include "Rendering/GameRenderContext.hpp"  
#include "Core/STLImport.hpp"

namespace LinaEngine::Graphics
{
	void GameRenderContext::Flush()
	{
		Texture* currentTexture = nullptr;

		for (LinaMap<LinaPair<VertexArray*, Texture*>, LinaArray<Matrix> >::iterator it = m_MeshRenderBuffer.begin(); it != m_MeshRenderBuffer.end(); ++it)
		{
			VertexArray* vertexArray = it->first.first;
			Texture* texture = it->first.second;
			Matrix* transforms = &it->second[0];
			size_t numTransforms = it->second.size();

			if (numTransforms == 0) continue;
			

			if (texture != currentTexture) 
				m_Shader->SetSampler("diffuse", *texture, *m_Sampler, 0);
			
			// Update the buffer w/ each transform.
			vertexArray->UpdateBuffer(4, transforms, numTransforms * sizeof(Matrix));

			// Draw call.
			this->Draw(*m_Shader, *vertexArray, *m_DrawParams, numTransforms);

			// Clear the buffer, or do not if you want a trail of shadows lol.
			it->second.clear();
		}
	}
}
