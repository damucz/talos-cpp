#pragma once

#include "TalosScopePayload.h"
#include <vector>

namespace Talos
{

	class ParticleEffectDescriptor;
	class ParticleEmitterDescriptor;
	class IEmitter;
	class ScopePayload;
	class IParticleRenderer;
	
	class ParticleEffectInstance
	{
	public:
		ParticleEffectInstance(ParticleEffectDescriptor* particleEffectDescriptor);
		~ParticleEffectInstance();
		
		void Init();
		float GetTotalTime() const;
		bool IsPaused() const;

		void SetScope(ScopePayload* scopePayload);
		ScopePayload* GetScope() const;

		void Update(float delta);
		void Render(IParticleRenderer* particleRenderer);

		void AddEmitter(ParticleEmitterDescriptor* particleEmitterDescriptor);
		void RemoveEmitterForEmitterDescriptor(ParticleEmitterDescriptor* emitter);

		bool IsContinuous() const;
		bool IsComplete() const;
		void AllowCompletion();
		void Pause();
		void Resume();
		void Restart();

		const std::vector<IEmitter*> GetEmitters() const;
		IEmitter* GetEmitter(ParticleEmitterDescriptor* descriptor) const;

		void SetPosition(float x, float y);
		float GetPositionX() const;
		float GetPositionY() const;
		int GetParticleCount() const;
		int GetNodeCalls() const;
		float GetAlpha() const;

		void ReportNodeCall();
		void SortEmitters();

	protected:
		ParticleEffectDescriptor* _descriptor = nullptr;
		std::vector<IEmitter*> _emitters;
		float _positionX = 0, _positionY = 0;
		ScopePayload* _scopePayload = nullptr;

		bool _loopable = false;
		int _particleCount = 0;
		int _nodeCalls = 0;
		float _totalTime = 0;
		float _alpha = 1.0f;

		bool _paused = false;
	};

}