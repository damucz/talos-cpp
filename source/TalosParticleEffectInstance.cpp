#include "TalosParticleEffectInstance.h"
#include "TalosParticleEffectDescriptor.h"
#include "TalosIEmitter.h"
#include "TalosParticleEmitterInstance.h"
#include "TalosParticleEmitterDescriptor.h"
#include "render/TalosIParticleRenderer.h"
#include <algorithm>

namespace Talos
{

	ParticleEffectInstance::ParticleEffectInstance(ParticleEffectDescriptor* particleEffectDescriptor)
	{
		_descriptor = particleEffectDescriptor;
	}

	ParticleEffectInstance::~ParticleEffectInstance()
	{
		for (auto& emitter : _emitters)
			delete emitter;
		delete _scopePayload;
	}

	void ParticleEffectInstance::Init()
	{
		for (IEmitter* emitter : _emitters)
			emitter->Init();
	}

	float ParticleEffectInstance::GetTotalTime() const
	{
		return _totalTime;
	}

	bool ParticleEffectInstance::IsPaused() const
	{
		return _paused;
	}

	void ParticleEffectInstance::SetScope(ScopePayload* scopePayload)
	{
		delete _scopePayload;
		_scopePayload = scopePayload;
		for (IEmitter* emitter : _emitters)
			emitter->SetScope(_scopePayload);
	}

	ScopePayload* ParticleEffectInstance::GetScope() const
	{
		return _scopePayload;
	}

	void ParticleEffectInstance::Update(float delta)
	{
		if (_paused)
			return;

		if (IsComplete() && !_loopable)
			return;

		_descriptor->SetEffectReference(this);

		if (_totalTime > 3600.0f)
			_totalTime = 0; //TODO: maybe just supple TimeUtils time now instead...
		_totalTime += delta;

		if (_scopePayload != nullptr)
			_scopePayload->Set(ScopePayload::TOTAL_TIME, _totalTime);

		_particleCount = 0;
		_nodeCalls = 0;
		for (IEmitter* emitter : _emitters)
		{
			emitter->Update(delta);
			_particleCount += emitter->GetActiveParticleCount();
		}

		if (_particleCount == 0 && _loopable)
		{
			for (IEmitter* emitter : _emitters)
			{
				if (!emitter->IsContinuous()) {
					if (emitter->GetDelayRemaining() == 0)
					{
						emitter->Restart();
					}
				}
			}
		}
	}

	void ParticleEffectInstance::Render(IParticleRenderer* particleRenderer)
	{
		particleRenderer->Render(this);
	}

	void ParticleEffectInstance::AddEmitter(ParticleEmitterDescriptor* particleEmitterDescriptor)
	{
		ParticleEmitterInstance* particleEmitterInstance = new ParticleEmitterInstance(particleEmitterDescriptor, this);
		particleEmitterInstance->SetScope(GetScope()); //Default set to the parent payload instance
		particleEmitterInstance->Init();
		_emitters.push_back(particleEmitterInstance);
	}

	void ParticleEffectInstance::RemoveEmitterForEmitterDescriptor(ParticleEmitterDescriptor* emitter)
	{
		for (auto it = _emitters.begin(); it != _emitters.end(); )
		{
			if ((*it)->GetEmitterGraph() == emitter)
				it = _emitters.erase(it);
			else
				++it;
		}
	}

	bool ParticleEffectInstance::IsContinuous() const
	{
		for (ParticleEmitterDescriptor* emitterDescriptor : _descriptor->GetEmitterModuleGraphs())
		{
			if (emitterDescriptor->IsContinuous())
			{
				return true;
			}
		}

		return false;
	}

	bool ParticleEffectInstance::IsComplete() const
	{
		if (_loopable)
			return false;

		for (IEmitter* emitter : _emitters)
		{
			if (!emitter->IsComplete())
				return false;
		}

		return true;
	}

	void ParticleEffectInstance::AllowCompletion()
	{
		_loopable = false;
		for (IEmitter* emitter : _emitters)
			emitter->Stop();
	}

	void ParticleEffectInstance::Pause()
	{
		for (IEmitter* emitter : _emitters)
			emitter->Pause();
		_paused = true;
	}

	void ParticleEffectInstance::Resume()
	{
		for (IEmitter* emitter : _emitters)
			emitter->Resume();
		_paused = false;
	}

	void ParticleEffectInstance::Restart()
	{
		for (IEmitter* emitter : _emitters)
			emitter->Restart();
		_paused = false;
		_totalTime = 0;
	}

	const std::vector<IEmitter*> ParticleEffectInstance::GetEmitters() const
	{
		return _emitters;
	}

	IEmitter* ParticleEffectInstance::GetEmitter(ParticleEmitterDescriptor* descriptor) const
	{
		for (IEmitter* emitter : _emitters)
		{
			if (emitter->GetEmitterGraph() == descriptor)
				return emitter;
		}
		return nullptr;
	}

	void ParticleEffectInstance::SetPosition(float x, float y)
	{
		_positionX = x;
		_positionY = y;
	}

	float ParticleEffectInstance::GetPositionX() const
	{
		return _positionX;
	}

	float ParticleEffectInstance::GetPositionY() const
	{
		return _positionY;
	}

	int ParticleEffectInstance::GetParticleCount() const
	{
		return _particleCount;
	}

	int ParticleEffectInstance::GetNodeCalls() const
	{
		return _nodeCalls;
	}

	float ParticleEffectInstance::GetAlpha() const
	{
		return _alpha;
	}

	void ParticleEffectInstance::ReportNodeCall()
	{
		++_nodeCalls;
	}

	void ParticleEffectInstance::SortEmitters()
	{
		auto comparator = [](const IEmitter* o1, const IEmitter* o2)
		{
			return o1->GetEmitterGraph()->GetSortPosition() > o2->GetEmitterGraph()->GetSortPosition();
		};

		std::sort(_emitters.begin(), _emitters.end(), comparator);

		for (int i = 0; i < (int)_emitters.size(); ++i)
			_emitters[i]->GetEmitterGraph()->SetSortPosition(i);
	}

}