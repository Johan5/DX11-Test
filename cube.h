#pragma once

#include <d3d11.h>
#include <vector>

#include "game_object.h"
#include "vector.h"
#include "matrix.h"
#include "graphics.h"
#include "vertex_shader.h"
#include "pixel_shader.h"


class CCube : public CGameObject
{
public:
	struct SCubeConstantBuffer
	{
		CMatrix4x4f _ModelToWorld;
		// Transforms normals from model to world (it is the inverse transform of _ModelToWorld)
		CMatrix4x4f _NormalModelToWorld;
		SMaterial _Material;
	};
	struct SVertex
	{
		CVector3f _Position;
		CVector3f _Normal;
		CVector4f _Color;
	};
	~CCube() override;

	void Initialize( CGraphics& Graphics ) override;
	void Shutdown() override;
	bool IsInitialized() const override;

	void Render( CRenderContext& RenderContext, const CCameraBase& Camera ) override;

	void SetPosition( const CVector3f& NewPosition );
	void SetScale( const CVector3f& NewScale );

	CMatrix4x4f GetLocalToWorldTransform() const;
	CMatrix4x4f GetNormalLocalToWorldTransform() const;

private:
	void UpdateTransforms() const;
private:
	CVector3f _Position = CVector3f{ 0.0f, 0.0f, 0.0f };
	CVector3f _Scale = CVector3f{ 1.0f, 1.0f, 1.0f };
	// Normalized, in world coords
	//CVector3f _Forward = CVector3f::Forward();
	CVector3f _Forward = CVector3f{ 1.0f, 0.0f, 0.0f }.CalcNormalized();
	// Normalized, in world coords
	// CVector3f _Up = CVector3f::Up();
	CVector3f _Up = CVector3f{ 0.0f, 1.0f, 0.0f }.CalcNormalized();

	// these are just cached values
	mutable bool _TransformsAreStale = true;
	mutable CMatrix4x4f _LocalToWorldTransform;
	// transforms normal vectors to world space
	mutable CMatrix4x4f _NormalLocalToWorldTransform;

	std::vector<SVertex> _Vertices;
	CVertexBuffer _VertexBuffer;
	CConstantBuffer _ConstantBuffer;
	CVertexShader _VertexShader;
	CPixelShader _PixelShader;

	bool _IsInitialized = false;
};


