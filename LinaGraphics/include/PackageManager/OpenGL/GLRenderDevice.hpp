/* 
This file is a part of: Lina Engine
https://github.com/inanevin/LinaEngine

Author: Inan Evin
http://www.inanevin.com

Copyright (c) [2018-2020] [Inan Evin]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*
Class: GLRenderDevice

Responsible for handling all Open GL related drawing functionalities. All GL commands are only stored and used
within an instance of this class.

Timestamp: 4/27/2019 10:12:16 PM
*/

#pragma once

#ifndef GLRenderDevice_HPP
#define GLRenderDevice_HPP


#include "Utility/Math/Matrix.hpp"
#include "Utility/Math/Color.hpp"
#include "Rendering/RenderingCommon.hpp"
#include <map>

using namespace LinaEngine;

namespace LinaEngine::Graphics
{

	// Vertex array struct for storage & vertex array data transportation.
	struct VertexArrayData
	{
		uint32* buffers;
		uintptr* bufferSizes;
		uint32  numBuffers;
		uint32  numElements;
		uint32  instanceComponentsStartIndex;
		BufferUsage bufferUsage;
	};

	// Shader program struct for storage.
	struct ShaderProgram
	{
		std::vector<uint32> shaders;
		std::map<std::string, int32> uniformBlockMap;
		std::map<std::string, int32> samplerMap;
		std::map<std::string, int32> uniformMap;
	};


	class GLRenderDevice
	{
	public:

		GLRenderDevice();
		~GLRenderDevice();

		// Initializes the devices & params.
		void Initialize(int width, int height, DrawParams& defaultParams);

		// Creates a texture on GL.
		uint32 CreateTexture2D(Vector2 size, const void* data,  SamplerParameters samplerParams ,bool compress, bool useBorder = false, Color borderColor = Color::White);

		// Creates an HDRI texture
		uint32 CreateTextureHDRI(Vector2 size, float* data, SamplerParameters samplerParams);

		// Creates a Cubemap texture on GL.
		uint32 CreateCubemapTexture(Vector2 size, SamplerParameters samplerParams, const std::vector<int32*>& data, uint32 dataSize = 6);
		
		// Creates an empty cubemap texture.
		uint32 CreateCubemapTextureEmpty(Vector2 size, SamplerParameters samplerParams);

		// Creates a multisampled texture
		uint32 CreateTexture2DMSAA(Vector2 size, SamplerParameters samplerParams, int sampleCount);

		// Creates an empty texture 2d
		uint32 CreateTexture2DEmpty(Vector2 size, SamplerParameters samplerParams);

		// Sets up texture parameters
		void SetupTextureParameters(uint32 textureTarget, SamplerParameters samplerParams, bool useBorder = false, float* borderColor = NULL);

		// Updates texture parameters
		void UpdateTextureParameters(uint32 bindMode, uint32 id, SamplerParameters samplerParmas);

		// Releases a previously created texture from GL.
		uint32 ReleaseTexture2D(uint32 texture2D);

		// Creates a vertex array on GL for mesh & model data.
		uint32 CreateVertexArray(const float** vertexData, const uint32* vertexElementSizes, const uint32* vertexElementTypes, uint32 numVertexComponents, uint32 numInstanceComponents, uint32 numVertices, const uint32* indices, uint32 numIndices, BufferUsage bufferUsage);

		// Creates a skybox vertex array.
		uint32 CreateSkyboxVertexArray();

		// Creates a vertex array for a screen quad
		uint32 CreateScreenQuadVertexArray();

		// Creates a vertex array for line drawing.
		uint32 CreateLineVertexArray();

		// Creates a vertex array for an hdri skybox cube to capture lighting data
		uint32 CreateHDRICubeVertexArray();

		// Releases a previously created vertex array by id from GL.
		uint32 ReleaseVertexArray(uint32 vao, bool checkMap = true);

		// Creates a texture sampler on GL.
		uint32 CreateSampler(SamplerParameters samplerParams);

		// Releases a previously created texture sampler from GL.
		uint32 ReleaseSampler(uint32 sampler);

		// Creates a uniform buffer on GL.
		uint32 CreateUniformBuffer(const void* data, uintptr dataSize, BufferUsage usage);

		// Releases a previously created buffer from GL.
		uint32 ReleaseUniformBuffer(uint32 buffer);

		// Creates a shader program based on shader text on GL.
		uint32 CreateShaderProgram(const std::string& shaderText, bool usesGeometryShader);

		// Runs glValidate on the target program.
		bool ValidateShaderProgram(uint32 shader);

		// Releases a previously created shader program from GL.
		uint32 ReleaseShaderProgram(uint32 shader);

		// Creates a render target based on window props & attachments on GL.
		uint32 CreateRenderTarget(uint32 texture, int32 width, int32 height, TextureBindMode bindTextureMode, FrameBufferAttachment attachment, uint32 attachmentNumber, uint32 mipLevel, bool noReadWrite, bool bindRBO = false, FrameBufferAttachment rboAtt = FrameBufferAttachment::ATTACHMENT_DEPTH_AND_STENCIL, uint32 rbo = 0, bool errorCheck = true);

		// Binds an additional texture to render target
		void BindTextureToRenderTarget(uint32 fbo, uint32 texture,TextureBindMode bindTextureMode, FrameBufferAttachment attachment, uint32 attachmentNumber, uint32 textureAttachmentNumber = 0, int mipLevel = 0, bool bindTexture = true, bool setDefaultFBO = true);

		// Tells open gl drawing into multiple buffers is enabled
		void MultipleDrawBuffersCommand(uint32 fbo, uint32 bufferCount, uint32* attachments);

		// Scale an existing texture
		void ResizeRTTexture(uint32 texture, Vector2 newSize, PixelFormat m_internalPixelFormat, PixelFormat m_pixelFormat, TextureBindMode bindMode = TextureBindMode::BINDTEXTURE_TEXTURE2D, bool compress = false);

		// Scales an existing render buffer
		void ResizeRenderBuffer(uint32 fbo, uint32 rbo, Vector2 newSize, RenderBufferStorage storage);

		// Releases a previously created render target from GL.
		uint32 ReleaseRenderTarget(uint32 target);

		// Creates a render buffer object
		uint32 CreateRenderBufferObject(RenderBufferStorage storage, uint32 width, uint32 height, int sampleCount);

		// Deletes a render buffer object.
		uint32 ReleaseRenderBufferObject(uint32 target);

		// Updates parameter settings for a sampler
		void UpdateSamplerParameters(uint32 sampler, SamplerParameters params);

		// Generate mipmaps for a texture
		void GenerateTextureMipmaps(uint32 texture, TextureBindMode bindMode);

		// Binds a rbo to an existing fbo
		void BlitFrameBuffers(uint32 readFBO, uint32 readWidth, uint32 readHeight, uint32 writeFBO, uint32 writeWidth, uint32 writeHeight, BufferBit mask, SamplerFilter filter);

		// Checks whether a given framebuffer is complete or not.
		bool IsRenderTargetComplete(uint32 fbo);

		// Updates a vertex array object by id.
		void UpdateVertexArray(uint32 vao, uint32 bufferIndex, const void* data, uintptr dataSize);

		// Uses a shader by id.
		void SetShader(uint32 shader);

		// Uses a shader sampler by id.
		void SetTexture(uint32 texture, uint32 sampler, uint32 unit, TextureBindMode bindTextureMode = TextureBindMode::BINDTEXTURE_TEXTURE2D, bool setSampler = false);

		// Creates uniform buffer of a shader by id.
		void SetShaderUniformBuffer(uint32 shader, const std::string& uniformBufferName, uint32 buffer);

		// Binds a buffer object to a binding point on GL buffer, then binds the program uniform block to that points.
		void BindUniformBuffer(uint32 buffer, uint32 bindingPoint);

		// Binds a shader to unifor block binding point.
		void BindShaderBlockToBufferPoint(uint32 shader, uint32 blockPoint, std::string& blockName);

		// Updates a vertex array buffer by id.
		void UpdateVertexArrayBuffer(uint32 vao, uint32 bufferIndex, const void* data, uintptr dataSize);

		// Updates a uniform buffer for a shader by id with offset.
		void UpdateUniformBuffer(uint32 buffer, const void* data, uintptr offset, uintptr dataSize);

		// Updates a uniform buffer for a shader by id without offset.
		void UpdateUniformBuffer(uint32 buffer, const void* data, uintptr dataSize);

		// Sets draw parameters.
		void SetDrawParameters(const DrawParams& drawParams);

		// Actual drawing process for meshes.
		void Draw(uint32 vao, const DrawParams& drawParams, uint32 numInstances, uint32 numElements, bool drawArrays = false);

		// Draws line bw two points
		void DrawLine(float width);
		void DrawLine(uint32 shader, const Matrix& model, const Vector3& from, const Vector3& to, float width = 1.0f);

		// Clears context.
		void Clear(bool shouldClearColor, bool shouldClearDepth, bool shouldClearStencil, const class Color& color, uint32 stencil);

		// Updates a float type uniform on a shader w/ given name.
		void UpdateShaderUniformFloat(uint32 shader, const std::string& uniform, const float f);

		// Updates an integer type uniform on a shader w/ given name.
		void UpdateShaderUniformInt(uint32 shader, const std::string& uniform, const int f);

		// Updates a vec3 type uniform on a shader with given name.
		void UpdateShaderUniformColor(uint32 shader, const std::string& uniform, const Color& color);

		// Updates a vec2 type uniform on a shader with given name.
		void UpdateShaderUniformVector2(uint32 shader, const std::string& uniform, const Vector2& m);

		// Updates a vec3 type uniform on a shader with given name.
		void UpdateShaderUniformVector3(uint32 shader, const std::string& uniform, const Vector3& m);

		// Updates a vec4 type uniform on a shader with given name.
		void UpdateShaderUniformVector4F(uint32 shader, const std::string& uniform, const Vector4& m);

		// Updates a mat4 type uniform on a shader with given name.
		void UpdateShaderUniformMatrix(uint32 shader, const std::string& uniform, const Matrix& m);

		// Updates a mat4 type uniform on a shader with given name.
		void UpdateShaderUniformMatrix(uint32 shader, const std::string& uniform, void* data);

		// Sets stencil mask to specific value
		void SetStencilWriteMask(uint32 mask);

		// Enable/disable depth testing.
		void SetDepthTestEnable(bool enable);

		// Set frame buffer object.
		void SetFBO(uint32 fbo);

		// Set vertex array object.
		void SetVAO(uint32 vao);

		// Captures HDRI lighting data.
		void CaptureHDRILightingData(Matrix& view, Matrix& projection, Vector2 captureSize, uint32 cubeMapTexture, uint32 hdrTexture, uint32 fbo, uint32 rbo, uint32 shader);

		// Sets viewport dimensions
		void SetViewport(Vector2 pos, Vector2 size);


	private:

		std::string GetShaderVersion();
		uint32 GetVersion();
	
		void SetRBO(uint32 rbo);
		void SetFaceCulling(FaceCulling faceCulling);
		void SetDepthTest(bool shouldWrite, DrawFunc depthFunc);
		void SetBlending(BlendFunc sourceBlend, BlendFunc destBlend);
		void SetStencilTest(bool enable, DrawFunc stencilFunc, uint32 stencilTestMask, uint32 stencilWriteMask, int32 stencilComparisonVal, StencilOp stencilFail, StencilOp stencilPassButDepthFail, StencilOp stencilPass);
		void SetScissorTest(bool enable, uint32 startX = 0, uint32 startY = 0, uint32 width = 0, uint32 height = 0);


	private:

		// Currently active shader.
		uint32 m_boundShader = 0;

		// Currently active vertex array object.
		uint32 m_boundVAO = 0;

		// Currently active frame buffer object.
		uint32 m_boundFBO = 0;

		// Currently active read buffer.
		uint32 m_boundReadFBO = 0;

		// Currently active write buffer.
		uint32 m_boundWriteFBO = 0;

		// FBO rep. on viewport.
		uint32 m_viewportFBO = 0;

		// Currently bound render buffer object
		uint32 m_boundRBO = 0;

		// Currently bound uniform buffer
		uint32 m_boundUBO;

		// Currently bound texture unit
		uint32 m_boundTextureUnit;

		// Bound Viewport size
		Vector2 m_boundViewportSize;

		// Bound Viewport position
		Vector2 m_boundViewportPos;

		// Map for bound vertex array objects.
		std::map<uint32, VertexArrayData> m_vaoMap;

		// Shader program map w/ ids.
		std::map<uint32, ShaderProgram> m_shaderProgramMap;

		// Storage for shader version.
		std::string m_ShaderVersion;

		// Storage for gl version data.
		uint32 m_GLVersion;

		// Current drawing parameters.
		FaceCulling m_usedFaceCulling;
		DrawFunc m_usedDepthFunction;
		BlendFunc m_usedSourceBlending;
		BlendFunc m_usedDestinationBlending;
		DrawFunc m_usedStencilFunction;
		StencilOp m_usedStencilFail;
		StencilOp m_usedStencilPassButDepthFail;
		StencilOp m_usedStencilPass;

		// Current operation parameters.
		uint32 m_usedStencilTestMask = 0;
		uint32 m_usedStencilWriteMask = 0;
		int32 m_usedStencilComparisonValue = 0;
		bool m_isBlendingEnabled = true;
		bool m_isStencilTestEnabled = true;
		bool m_isScissorsTestEnabled = false;
		bool m_shouldWriteDepth = true;
		bool m_isDepthTestEnabled = true;
		Color m_currentClearColor = Color::Black;
		
	};
}


#endif