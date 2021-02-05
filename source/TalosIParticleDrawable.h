#pragma once

namespace Talos
{

	class Particle;

	class RenderContext
	{
	public:
		virtual ~RenderContext() {}
	};
	
	class IParticleDrawable
	{
	public:
		virtual ~IParticleDrawable() {}
		
		virtual void Draw(RenderContext* context, float x, float y, float width, float height, float rotation) = 0;
		virtual void Draw(RenderContext* context, Particle* particle, float r, float g, float b, float a) = 0;

		virtual float GetAspectRatio() const { return 1.0f; }

		void SetCurrentParticle(Particle* particle) { _currentParticle = particle; }

		void GetTextureRegion() const {}

	protected:
		Particle* _currentParticle = nullptr;
	};
	
}
