
/*
 * Copyright (C) 2019 Inan Evin
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#if defined(VS_BUILD)
#include <../UniformBuffers.glh>
layout (location = 0) in vec3 position;
out vec3 TexCoords;

void main()
{
  mat4 rotView = mat4(mat3(view));
  vec4 clipPos = projection * rotView * vec4(position, 1.0);
  gl_Position = clipPos.xyww;
	TexCoords = position;
}

#elif defined(FS_BUILD)
out vec4 fragColor;
in vec3 TexCoords;
struct Material
{
  vec3 startColor;
  vec3 endColor;
  vec3 sunDirection;
};
uniform Material material;

void main()
{
	float f = dot(normalize(TexCoords), normalize(-material.sunDirection)) * 0.5f + 0.5f;
  fragColor = mix(vec4(material.startColor, 1.0), vec4(material.endColor, 1.0), pow(f,2)) * 1;
}

#endif
