#pragma once

#include "TalosIEmitter.h"
#include "TalosTypes.h"

namespace Talos
{

	class EmitterModule;
	class ParticleModule;
	class ParticleEffectInstance;
	
	class ParticleEmitterInstance : public IEmitter
	{
	public:
		ParticleEmitterInstance(ParticleEmitterDescriptor* moduleGraph, ParticleEffectInstance* particleEffectInstance);
		~ParticleEmitterInstance();

		void Init() override;
		void Update(float delta) override;
		ParticleEmitterDescriptor* GetEmitterGraph() const override;
		bool IsVisible() const override;
		bool IsAdditive() const override;
		bool IsBlendAdd() const override;
		const std::vector<Particle*>& GetActiveParticles() const override;

		void UpdateParticles(float delta);

		void Restart() override;
		float GetDelayRemaining() const override;
		void SetScope(ScopePayload* scope) override;
		ScopePayload* GetScope() const override;
		int GetActiveParticleCount() const override;
		bool IsContinuous() const override;
		bool IsComplete() const override;
		float GetAlpha() const override;
		ParticleModule* GetParticleModule() const override;
		EmitterModule* GetEmitterModule() const override;
		void GetEffectPosition(float& x, float& y) const override;
		void GetTint(float& r, float& g, float& b, float& a) const override;
		
		ParticleEffectInstance* GetEffect() const;
		bool IsAttached() const;

		void Stop() override;
		void Pause() override;
		void Resume() override;
		void SetVisible(bool isVisible) override;
		void SetTint(float r, float g, float b, float a);

		RandomXS128& GetRandom() override;
		
	protected:
		ParticleEffectInstance* _parentParticleInstance = nullptr;
		bool _isComplete = false;
		ParticleEmitterDescriptor* _emitterGraph = nullptr;
		ScopePayload* _scopePayload = nullptr;

		bool _isAdditive = true;
		bool _isBlendAdd = false;

		float _positionX = 0, _positionY = 0;
		float _duration = 0;
		float _delay = 0;
		float _delayTimer = 0;

		bool _isVisible = true;
		bool _paused = false;
		bool _isContinuous = false;
		bool _isAttached = false;
		bool _isImmortal = false;

		float _tintR = 1.0f, _tintG = 1.0f, _tintB = 1.0f, _tintA = 1.0f;		// WHITE

		EmitterModule* _emitterModule = nullptr;

		float _rate = 0;		// emission rate

		// inner vars
		float _alpha = 0;
		float _particlesToEmit = 1.0f;

		bool _initialized = false;

		std::vector<Particle*> _activeParticles;

		bool _isStopped = false;

		RandomXS128 _random;
	};
	
}
