#pragma once

#include <vector>

namespace Talos
{

	class ParticleEmitterDescriptor;
	class ParticleEffectInstance;
	class IFactory;
	class IFileProvider;
	
	class ParticleEffectDescriptor
	{
	public:
		ParticleEffectDescriptor();
		ParticleEffectDescriptor(IFactory* factory, const char* fileName);
		~ParticleEffectDescriptor();

		IFactory* GetFactory() const;

		void AddEmitter(ParticleEmitterDescriptor* emitter);
		void RemoveEmitter(ParticleEmitterDescriptor* emitter);
		ParticleEmitterDescriptor* CreateEmitter();

		void Load(IFileProvider* fileProvider);

		ParticleEffectInstance* CreateEffectInstance();

		bool IsContinuous() const;

		void SetEffectReference(ParticleEffectInstance* particleEffectInstance);
		ParticleEffectInstance* GetInstanceReference() const;

		const std::vector<ParticleEmitterDescriptor*>& GetEmitterModuleGraphs() const;
		
	protected:
		IFactory* _factory = nullptr;
		// graph per each emitter
		std::vector<ParticleEmitterDescriptor*> _emitterModuleGraphs;

		ParticleEffectInstance* _processingEffectReference = nullptr;
	};

}
