#include "modules/TalosEmitterModule.h"
#include "values/TalosNumericalValue.h"
#include "values/TalosEmConfigValue.h"
#include "TalosScopePayload.h"
#include "TalosParticleEmitterInstance.h"

namespace Talos
{
	void EmitterModule::Load(IFileProvider::ModuleData* moduleData)
	{
		AbstractModule::Load(moduleData);

		_defaultDelay = moduleData->GetFloat("delay");
		_defaultDuration = moduleData->GetFloat("duration");
		_defaultRate = moduleData->GetFloat("rate");
	}

	void EmitterModule::ProcessValues()
	{
		// nothing to process
	}

	void EmitterModule::UpdateScopeData(ParticleEmitterInstance* particleEmitter)
	{
		auto* scope = GetScope();
		scope->Set(ScopePayload::EMITTER_ALPHA, particleEmitter->GetAlpha());
		scope->Set(ScopePayload::REQUESTER_ID, 1.1f);   // TODO change to something more... unique when emitters are in
	}

	float EmitterModule::GetDelay()
	{
		FetchInputSlotValue(DELAY);
		if (_delay->IsEmpty()) return _defaultDelay; // defaults
		return _delay->GetFloat();
	}

	float EmitterModule::GetDuration()
	{
		FetchInputSlotValue(DURATION);
		if (_duration->IsEmpty()) return _defaultDuration; // defaults
		return _duration->GetFloat();
	}

	float EmitterModule::GetRate()
	{
		FetchInputSlotValue(RATE);
		if (_rate->IsEmpty()) return _defaultRate; // defaults
		return _rate->GetFloat();
	}

	bool EmitterModule::IsContinuous()
	{
		FetchInputSlotValue(CONFIG);
		if (_config->IsEmpty()) return false;
		return _config->_continuous;
	}

	bool EmitterModule::IsAttached()
	{
		FetchInputSlotValue(CONFIG);
		if (_config->IsEmpty()) return false;
		return _config->_attached;
	}

	bool EmitterModule::IsAligned()
	{
		FetchInputSlotValue(CONFIG);
		if (_config->IsEmpty()) return false;
		return _config->_aligned;
	}

	bool EmitterModule::IsAdditive()
	{
		FetchInputSlotValue(CONFIG);
		if (_config->IsEmpty()) return false;
		return _config->_additive;
	}

	bool EmitterModule::IsBlendAdd()
	{
		FetchInputSlotValue(CONFIG);
		if (_config->IsEmpty()) return false;
		return _config->_isBlendAdd;
	}

	bool EmitterModule::IsImmortal()
	{
		FetchInputSlotValue(CONFIG);
		if (_config->IsEmpty()) return false;
		return _config->_immortal;
	}

	bool EmitterModule::IsEmitterModule() const
	{
		return true;
	}

	void EmitterModule::DefineSlots()
	{
		_delay = CreateInputSlot(DELAY);
		_duration = CreateInputSlot(DURATION);
		_rate = CreateInputSlot(RATE);

		_config = (EmConfigValue*)CreateInputSlot(CONFIG, new EmConfigValue());
	}
	
}
