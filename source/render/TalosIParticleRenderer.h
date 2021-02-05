#pragma once

namespace Talos
{

	class ParticleEffectInstance;

	class IParticleRenderer
	{
	public:
		void Render(ParticleEffectInstance* particleEffectInstance) {};
	};
	
}