#pragma once

#include "TalosIParticleDrawable.h"

namespace Talos
{

	class TextureRegionDrawable : public IParticleDrawable
	{
	public:
		TextureRegionDrawable(const char* region) {}
		
		void Draw(RenderContext* context, float x, float y, float width, float height, float rotation) override {}
		void Draw(RenderContext* context, Particle* particle, float r, float g, float b, float a) override {}

		float GetAspectRatio() const override { return 1.0f; }

	protected:
	};
	
}