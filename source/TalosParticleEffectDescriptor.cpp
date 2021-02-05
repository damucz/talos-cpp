#include "TalosParticleEffectDescriptor.h"
#include "TalosParticleEffectInstance.h"
#include "TalosParticleEmitterDescriptor.h"
#include "serialization/TalosIFileProvider.h"
#include "modules/TalosAbstractModule.h"
#include "TalosIFactory.h"
#include <map>
#include <assert.h>

namespace Talos
{
	
	ParticleEffectDescriptor::ParticleEffectDescriptor()
	{
	}

	ParticleEffectDescriptor::ParticleEffectDescriptor(IFactory* factory, const char* fileName)
		: _factory(factory)
	{
		auto* fileProvider = factory->CreateFileProvider(fileName);
		Load(fileProvider);
		delete fileProvider;
	}

	ParticleEffectDescriptor::~ParticleEffectDescriptor()
	{
		delete _factory;
		for (auto* emitter: _emitterModuleGraphs)
			delete emitter;
	}

	IFactory* ParticleEffectDescriptor::GetFactory() const
	{
		return _factory;
	}

	void ParticleEffectDescriptor::AddEmitter(ParticleEmitterDescriptor* emitter)
	{
		_emitterModuleGraphs.push_back(emitter);
	}

	void ParticleEffectDescriptor::RemoveEmitter(ParticleEmitterDescriptor* emitter)
	{
		auto it = std::find(_emitterModuleGraphs.begin(), _emitterModuleGraphs.end(), emitter);
		if (it != _emitterModuleGraphs.end())
		{
			delete* it;
			_emitterModuleGraphs.erase(it);
		}
	}

	ParticleEmitterDescriptor* ParticleEffectDescriptor::CreateEmitter()
	{
		return new ParticleEmitterDescriptor(this);
	}

	void ParticleEffectDescriptor::Load(IFileProvider* fileProvider)
	{
		ParticleEmitterDescriptor::RegisterModules();

		for (auto* emitterData : fileProvider->GetEmitters())
		{
			ParticleEmitterDescriptor* emitterDescriptor = new ParticleEmitterDescriptor(this);

			std::map<int, AbstractModule*> idMap;

			for (auto* moduleData : fileProvider->GetModules(emitterData))
			{
				const char* clazz = moduleData->GetClass();
				auto* module = ParticleEmitterDescriptor::CreateModule(clazz);
				assert(module);
				module->Init();
				module->Load(moduleData);
				module->SetModuleGraph(emitterDescriptor);
				emitterDescriptor->AddModule(module);   // I cannot understand how this was working before. This is needed so that it can later reset requesters.
				idMap[module->GetIndex()] = module;

				delete moduleData;
			}

			for (auto* connectionData : fileProvider->GetConnections(emitterData))
			{
				int moduleFromId = connectionData->moduleFrom;
				int moduleToId = connectionData->moduleTo;
				int slotFrom = connectionData->slotFrom;
				int slotTo = connectionData->slotTo;

				assert(idMap.find(moduleFromId) != idMap.end());
				assert(idMap.find(moduleToId) != idMap.end());
				
				AbstractModule* moduleFrom = idMap[moduleFromId];
				AbstractModule* moduleTo = idMap[moduleToId];

				emitterDescriptor->ConnectNode(moduleFrom, moduleTo, slotFrom, slotTo);

				delete connectionData;
			}

			_emitterModuleGraphs.push_back(emitterDescriptor);

			delete emitterData;
		}
	}

	ParticleEffectInstance* ParticleEffectDescriptor::CreateEffectInstance()
	{
		ParticleEffectInstance* particleEffectInstance = new ParticleEffectInstance(this);
		SetEffectReference(particleEffectInstance);

		// create default scope
		particleEffectInstance->SetScope(new ScopePayload);

		for (ParticleEmitterDescriptor* emitterDescriptor : _emitterModuleGraphs)
			particleEffectInstance->AddEmitter(emitterDescriptor);

		particleEffectInstance->SortEmitters();
		
		return particleEffectInstance;
	}

	bool ParticleEffectDescriptor::IsContinuous() const
	{
		for (ParticleEmitterDescriptor* emitterDescriptor : _emitterModuleGraphs)
		{
			if (emitterDescriptor->GetEmitterModule() == nullptr || emitterDescriptor->GetParticleModule() == nullptr)
				return false;
			if (GetInstanceReference() == nullptr)
				return false;
			if (emitterDescriptor->IsContinuous())
				return true;
		}

		return false;
	}

	void ParticleEffectDescriptor::SetEffectReference(ParticleEffectInstance* particleEffectInstance)
	{
		_processingEffectReference = particleEffectInstance;
	}

	ParticleEffectInstance* ParticleEffectDescriptor::GetInstanceReference() const
	{
		return _processingEffectReference;
	}

	const std::vector<ParticleEmitterDescriptor*>& ParticleEffectDescriptor::GetEmitterModuleGraphs() const
	{
		return _emitterModuleGraphs;
	}
}
