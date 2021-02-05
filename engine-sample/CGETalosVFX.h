#ifdef TALOSVFX_AVAILABLE
#pragma once

#include "CGEUIComponent.h"
#include "TalosIFactory.h"
#include "render/drawables/TalosTextureRegionDrawable.h"
#include "render/drawables/TalosPolylineDrawable.h"
#include "render/drawables/TalosShadedDrawable.h"

namespace Talos
{
	class ParticleEffectDescriptor;
	class ParticleEffectInstance;
}

namespace CGE
{

	class TalosVFXFactory : public Talos::IFactory
	{
	public:
		Talos::IFileProvider* CreateFileProvider(const char* fileName) override;
		Talos::TextureRegionDrawable* CreateTextureRegionDrawable(const char* region) override;
		Talos::PolylineDrawable* CreatePolylineDrawable(const char* region) override;
		Talos::ShadedDrawable* CreateShadedDrawable(const char* shdrFileName) override;
	};

	class TalosRenderContext : public Talos::RenderContext
	{
	public:
		Thor::Renderer* renderer = nullptr;
		CIwFMat animModelTransform;
	};

	class TalosTextureRegionDrawable : public Talos::TextureRegionDrawable
	{
	public:
		TalosTextureRegionDrawable(const char* region);

		void Draw(Talos::RenderContext* context, float x, float y, float width, float height, float rotation) override;
		void Draw(Talos::RenderContext* context, Talos::Particle* particle, float r, float g, float b, float a) override;	
	//protected:
		Thor::Texture* _texture = nullptr;
	};

	class TalosPolylineDrawable : public Talos::PolylineDrawable
	{
	public:
		TalosPolylineDrawable(const char* region);
		~TalosPolylineDrawable();

		void Draw(Talos::RenderContext* context, float x, float y, float width, float height, float rotation) override;
		void Draw(Talos::RenderContext* context, Talos::Particle* particle, float r, float g, float b, float a) override;
	//protected:
		Thor::Texture* _texture = nullptr;
		int _interpolationPointCount = 0;

		class Polyline
		{
		public:
			struct PointData
			{
				float positionX = 0, positionY = 0;
				float offsetX = 0, offsetY = 0;
				float thickness = 0;
				float r = 0, g = 0, b = 0, a = 0;
			};

			~Polyline();
			
			void Reset();
			void Set(float size, float rotation);
			void InitPoints(int interpolationPoints, float x, float y);
			void SetPointData(int index, float offsetX, float offsetY, float thickness, float r, float g, float b, float a);
			const std::vector<PointData>& GetPoints() const;

			void Draw(Thor::Renderer* renderer, Thor::Texture* region);
			void Draw(Thor::Renderer* renderer, Thor::Texture* region, float x, float y);

			float _leftTangentX = 0, _leftTangentY = 0;
			float _rightTangentX = 0, _rightTangentY = 0;

			float _leftPointX = 0, _leftPointY = 0;
			float _rightPointX = 0, _rightPointY = 0;

			float _rotation = 0;

		protected:
			std::vector<PointData> _points;

			// inner workings
			CIwFVec3* _vertices = nullptr;
			int _verticesLength = 0;
			uint16* _indices = nullptr;
			int _indicesLength = 0;
			CIwFVec2* _uvs = nullptr;
			CIwColour* _colors = nullptr;

			void InitVertices();
			void UpdatePointPositions(float x, float y);
			void ExtrudePoint(Thor::Texture* region, int index, int pos);
			void PackVertex(Thor::Texture* region, CIwFVec3* vertices, CIwFVec2* uvs, CIwColour* colors,
				int index, float x, float y, float r, float g, float b, float a, float u, float v);
		};

		Polyline* GetPolyline();

		void SetCount(int count) override;
		void SetPointData(int pointIndex, float offsetX, float offsetY, float thickness, float r, float g, float b, float a) override;
		void SetTangents(float leftX, float leftY, float rightX, float rightY) override;

		std::map<Talos::Particle*, Polyline*> _polylineMap;
	};

	class TalosShadedDrawable : public Talos::ShadedDrawable
	{
	public:
		TalosShadedDrawable(const char* shdrFileName);

		void Draw(Talos::RenderContext* context, float x, float y, float width, float height, float rotation) override;
		void Draw(Talos::RenderContext* context, Talos::Particle* particle, float r, float g, float b, float a) override;
		
	protected:
		Thor::ShaderProgramBGFX* _shader = nullptr;
		Thor::ShaderManager _shaderMgr;
	};

	class TalosVFX : public CGE::UIComponent
	{
	public:
		TalosVFX(const char* fileName);
		~TalosVFX();

		bool Update(float dt) override;
		void DrawContent(Thor::Renderer* renderer) override;

		void RenderParticle(TalosRenderContext* context, Talos::Particle* particle, float alpha);
		
	protected:
		TalosVFXFactory* _factory = nullptr;
		Talos::ParticleEffectDescriptor* _particleEffectDescriptor = nullptr;
		Talos::ParticleEffectInstance* _particleEffectInstance = nullptr;
	};

}

#endif