#pragma once

#include "TalosIParticleDrawable.h"

namespace Talos
{

	class PolylineDrawable : public IParticleDrawable
	{
	public:
		PolylineDrawable(const char* region) {}

		void Draw(RenderContext* context, float x, float y, float width, float height, float rotation) override {}
		void Draw(RenderContext* context, Particle* particle, float r, float g, float b, float a) override {}

		float GetAspectRatio() const override { return 1.0f; }

		virtual void SetCount(int count) {}
		virtual void SetPointData(int pointIndex, float offsetX, float offsetY, float thickness, float r, float g, float b, float a) {}
		virtual void SetTangents(float leftX, float leftY, float rightX, float rightY) {}

	protected:
	};

}