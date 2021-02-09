#include "TalosParticleEmitterInstance.h"
#include "TalosParticleEmitterDescriptor.h"
#include "TalosParticleEffectInstance.h"
#include "TalosParticle.h"
#include "modules/TalosEmitterModule.h"
#include <algorithm>

namespace Talos
{
	ParticleEmitterInstance::ParticleEmitterInstance(ParticleEmitterDescriptor* moduleGraph,
		ParticleEffectInstance* particleEffectInstance)
	{
		_emitterGraph = moduleGraph;
		_parentParticleInstance = particleEffectInstance;
	}

	ParticleEmitterInstance::~ParticleEmitterInstance()
	{
		for (auto* particle : _activeParticles)
			delete particle;
	}

	void ParticleEmitterInstance::Init()
	{
		_positionX = 0;
		_positionY = 0;

		_emitterModule = _emitterGraph->GetEmitterModule();
		if (_emitterModule == nullptr)
			return;

		_delay = _emitterModule->GetDelay();
		_duration = _emitterModule->GetDuration();
		_isContinuous = _emitterModule->IsContinuous();

		_delayTimer = _delay;

		// inner variable defaults
		_alpha = 0.0f;
		_isComplete = false;
		_particlesToEmit = 1.0f; // always emmit one first

		_initialized = true;
	}

	void ParticleEmitterInstance::Update(float delta)
	{
		_emitterModule = _emitterGraph->GetEmitterModule();
		if (_emitterModule == nullptr)
			return;

		if (!_initialized)
			Init();

		if (_paused)
			return;

		//update variables to their real values
		_emitterModule->UpdateScopeData(this);

		_delay = _emitterModule->GetDelay();
		_duration = _emitterModule->GetDuration();
		_isContinuous = _emitterModule->IsContinuous();
		_rate = _emitterModule->GetRate();
		_isAttached = _emitterModule->IsAttached();
		_isAdditive = _emitterModule->IsAdditive();
		_isBlendAdd = _emitterModule->IsBlendAdd();
		_isImmortal = _emitterModule->IsImmortal();

		if (_delayTimer > 0)
		{
			_delayTimer -= delta;
			if (_delayTimer < 0)
				_delayTimer = 0;
			if (_delayTimer > 0)
			{
				UpdateParticles(delta); // process existing particles at least

				_emitterGraph->ResetRequesters();
				return;
			}
		}

		float normDelta = delta / _duration;

		float deltaLeftover = 0;
		if (_alpha + normDelta > 1.0f)
		{
			deltaLeftover = (1.0f - _alpha) * _duration;
			_alpha = 1.0f;
		}
		else
		{
			_alpha += normDelta;
			deltaLeftover = delta;
		}

		//update variables to their real values
		_emitterModule->UpdateScopeData(this);

		//
		if (_alpha < 1.0f || (_alpha == 1.0f && deltaLeftover > 0))		 // emission only here
		{
			// let's emit
			_particlesToEmit += _rate * deltaLeftover;

			if (_isImmortal)
				_particlesToEmit = std::max(0.0f, roundf(_rate * _duration) - _activeParticles.size());

			int count = (int)_particlesToEmit;
			for (int i = 0; i < count; ++i)
			{
				Particle* particle = new Particle();
				if (_emitterGraph->GetParticleModule() != nullptr)
				{
					particle->Init(this);
					_activeParticles.push_back(particle);
				}
			}
			_particlesToEmit -= (float)count;
		}

		// process existing particles.
		UpdateParticles(delta);

		if (_alpha == 1.0f)
		{
			if (_isContinuous && !_isStopped)
			{
				// let's repeat
				Restart();
			}
			else
			{
				// all immortals must die
				if (_isImmortal)
				{
					for (auto* particle : _activeParticles)
					{
						particle->_alpha = 1.0f;
						particle->NotifyKill();
						delete particle;
					}
					_activeParticles.clear();
				}
			}
		}

		_isComplete = _activeParticles.empty();

		_emitterGraph->ResetRequesters();
	}

	ParticleEmitterDescriptor* ParticleEmitterInstance::GetEmitterGraph() const
	{
		return _emitterGraph;
	}

	bool ParticleEmitterInstance::IsVisible() const
	{
		return _isVisible;
	}

	bool ParticleEmitterInstance::IsAdditive() const
	{
		return _isAdditive;
	}

	bool ParticleEmitterInstance::IsBlendAdd() const
	{
		return _isBlendAdd;
	}

	const std::vector<Particle*>& ParticleEmitterInstance::GetActiveParticles() const
	{
		return _activeParticles;
	}

	void ParticleEmitterInstance::UpdateParticles(float delta)
	{
		for (auto it = _activeParticles.begin(); it != _activeParticles.end(); )
		{
			Particle* particle = *it;
			particle->Update(delta);

			if (_isImmortal)
			{
				// if immortal we don't kill them
				if (particle->_alpha >= 1.0f)
				{
					particle->_alpha = particle->_alpha - 1.0f;
				}
			}
			
			if (particle->_alpha >= 1.0f)
			{
				particle->NotifyKill();
				delete particle;
				it = _activeParticles.erase(it);
			}
			else
				++it;
		}

		// do some immortality cleaning
		if (_isImmortal)
		{
			int particlesToExpect = std::max(0, (int)roundf(_rate * _duration));
			int particlesToDelete = (int)_activeParticles.size() - particlesToExpect;
			if (particlesToDelete > 0)
			{
				for (auto it = _activeParticles.rbegin(); particlesToDelete > 0; --particlesToDelete)
				{
					Particle* particle = *it;
					particle->_alpha = 1.0f;
					particle->NotifyKill();
					delete particle;
					std::advance(it, 1);
					_activeParticles.erase(it.base());
				}
			}
		}
	}

	void ParticleEmitterInstance::Restart()
	{
		_delayTimer = _delay;
		_alpha = 0;
		_isComplete = false;
		_particlesToEmit = 1.0f;
		_isStopped = false;
	}

	float ParticleEmitterInstance::GetDelayRemaining() const
	{
		return _delayTimer;
	}

	void ParticleEmitterInstance::SetScope(ScopePayload* scope)
	{
		_scopePayload = scope;
	}

	ScopePayload* ParticleEmitterInstance::GetScope() const
	{
		return _scopePayload;
	}

	int ParticleEmitterInstance::GetActiveParticleCount() const
	{
		return _activeParticles.size();
	}

	bool ParticleEmitterInstance::IsContinuous() const
	{
		return _isContinuous;
	}

	bool ParticleEmitterInstance::IsComplete() const
	{
		return _isComplete;
	}

	float ParticleEmitterInstance::GetAlpha() const
	{
		return _alpha;
	}

	ParticleModule* ParticleEmitterInstance::GetParticleModule() const
	{
		return _emitterGraph->GetParticleModule();
	}

	EmitterModule* ParticleEmitterInstance::GetEmitterModule() const
	{
		return _emitterGraph->GetEmitterModule();
	}

	void ParticleEmitterInstance::GetEffectPosition(float& x, float& y) const
	{
		x = GetEffect()->GetPositionX();
		y = GetEffect()->GetPositionY();
	}

	void ParticleEmitterInstance::GetTint(float& r, float& g, float& b, float& a) const
	{
		r = _tintR;
		g = _tintG;
		b = _tintB;
		a = _tintA;
	}

	ParticleEffectInstance* ParticleEmitterInstance::GetEffect() const
	{
		return _parentParticleInstance;
	}

	bool ParticleEmitterInstance::IsAttached() const
	{
		return _isAttached;
	}

	void ParticleEmitterInstance::Stop()
	{
		_alpha = 1.0f;
		_isStopped = true;
	}

	void ParticleEmitterInstance::Pause()
	{
		_paused = true;
	}

	void ParticleEmitterInstance::Resume()
	{
		_paused = false;
	}

	void ParticleEmitterInstance::SetVisible(bool isVisible)
	{
		_isVisible = isVisible;
	}

	void ParticleEmitterInstance::SetTint(float r, float g, float b, float a)
	{
		_tintR = r;
		_tintG = g;
		_tintB = b;
		_tintA = a;
	}

	RandomXS128& ParticleEmitterInstance::GetRandom()
	{
		return _random;
	}
	
}
