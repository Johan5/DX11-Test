#pragma once

#include "d3dclass.h"
#include "vertex_shader.h"
#include "pixel_shader.h"
#include "constant_buffer.h"
#include "texture.h"
#include "texture_view.h"
#include "render_target_view.h"
#include "geometry_shader.h"
#include "texture_view.h"
#include "sampler_state.h"
#include "depth_stencil_view.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "render_context.h"
#include "mesh.h"

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <optional>
#include <wrl/client.h> // Microsoft ComPtr

class CVertexShader;

namespace NGraphicsDefines
{
	const bool IsFullScreen = false;
	const bool IsVSyncEnabled = true;
	const float ScreenDepth = 1000.0f;
	const float ScreenNear = 1.0f;
	const float ScreenFar = 100.0f;

	// These slots should of course otimally not be hardcoded, but parsed from resource files
	const int32_t ShadowMapTextureSlot = 0;
	const int32_t ShadowMapSamplerSlot = 0;
}

////////////////////////////////////////////////////////////////////////////////


// The CGraphics class' primary usage is creating (and owning) various graphics related resources,
// such as buffers and shaders.
class CGraphics
{
public:
	CGraphics() = default;
	CGraphics( const CGraphics& Other ) = delete;
	CGraphics( const CGraphics&& Other ) = delete;
	~CGraphics();
	CGraphics& operator=( const CGraphics& Other ) = delete;
	CGraphics& operator=( const CGraphics&& Other ) = delete;

	bool Initialize( int ScreenWidth, int ScreenHeight, HWND Wnd );
	void Shutdown();

	CVertexBuffer CreateVertexBuffer( const void* pVertexData, uint32_t VertexDataSizeInBytes, const SVertexBufferProperties& Settings );
	CIndexBuffer CreateIndexBuffer(uint32_t SizeInBytes, ECpuAccessPolicy AccessPolicy);
	CConstantBuffer CreateConstantBuffer( int32_t SizeInBytes, ECpuAccessPolicy AccessPolicy );
	CTexture CreateTexture(uint32_t Width, uint32_t Height, EGfxResourceDataFormat Format, uint32_t BindFlags, ECpuAccessPolicy AccessPolicy);
	CTexture CreateTexture(const D3D11_TEXTURE2D_DESC& TextureDesc);
	CTexture CreateTexture(const D3D11_TEXTURE2D_DESC& TextureDesc, const D3D11_SUBRESOURCE_DATA& pInitialData);
	CTextureView CreateTextureView(CTexture& Texture, const D3D11_SHADER_RESOURCE_VIEW_DESC& Desc);
	CRenderTargetView CreateRenderTargetView(CTexture& Texture, const D3D11_RENDER_TARGET_VIEW_DESC& Desc);
	CDepthStencilView CreateDepthStencilView(CTexture& Texture, const D3D11_DEPTH_STENCIL_VIEW_DESC& Desc);
	CSamplerState CreateSamplerState(const D3D11_SAMPLER_DESC& SamplerDesc);

	int GetScreenWidth() const { return _ScreenWidthInPix; }
	int GetScreenHeight() const { return _ScreenHeightInPix; }
	std::optional<CTexture> GetTextureByName(const std::string& Name);

	CVertexShader* AccessVertexShader(const std::string& fileName);
	CPixelShader* AccessPixelShader(const std::string& fileName);
	CGeometryShader* AccessGeometryShader(const std::string& fileName);

	SMesh* AccessMesh(EMeshType MeshType);

	CRenderContext& StartRenderFrame( const CVector4f& BackgroundColor );
	void EndFrame( CRenderContext& RenderContext );
	CRenderContext& AccessRenderContest() { return _RenderContext; }
	
	ID3D11Device& AccessDevice() { return *_Direct3D->AccessDevice(); }

private:
	bool initShaders();
	bool initTextures();
	bool initMeshes();

	std::optional<CVertexShader> CreateVertexShader(const std::string& ShaderFileName, const std::string& ShaderMainFunction, std::vector<SShaderInputDescription>& ShaderInputLayout);
	std::optional<CGeometryShader> CreateGeometryShader(const std::string& ShaderFileName, const std::string& ShaderMainFunction);
	std::optional<CPixelShader> CreatePixelShader(const std::string& ShaderFileName, const std::string& ShaderMainFunction);

private:
	std::unique_ptr<CDirectX3D> _Direct3D;

	CRenderContext _RenderContext;

	int32_t _ScreenWidthInPix;
	int32_t _ScreenHeightInPix;

	std::unordered_map<EMeshType, SMesh> _Meshes;
	std::unordered_map<std::string, CTexture> _Textures;

	std::unordered_map<std::string, CVertexShader> _VertexShaders;
	std::unordered_map<std::string, CPixelShader> _PixelShaders;
	std::unordered_map<std::string, CGeometryShader> _GeometryShaders;
};
