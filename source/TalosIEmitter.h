#pragma once

#include <vector>

namespace Talos
{

    class ParticleModule;
    class EmitterModule;
    class ScopePayload;
    class ParticleEmitterDescriptor;
    class Particle;
    class RandomXS128;
	
	class IEmitter
	{
	public:
        virtual ~IEmitter() {}
		
        virtual void Init() = 0;

        virtual float GetAlpha() const = 0;
        virtual ParticleModule* GetParticleModule() const = 0;
        virtual EmitterModule* GetEmitterModule() const = 0;
        virtual void GetEffectPosition(float& x, float& y) const = 0;
        virtual ScopePayload* GetScope() const = 0;
        virtual void GetTint(float&r, float& g, float& b, float& a) const = 0;
        virtual ParticleEmitterDescriptor* GetEmitterGraph() const = 0;
        virtual const std::vector<Particle*>& GetActiveParticles() const = 0;
		
        virtual void SetScope(ScopePayload* scope) = 0;
        virtual int GetActiveParticleCount() const = 0;
        virtual bool IsContinuous() const = 0;
        virtual bool IsComplete() const = 0;
        virtual float GetDelayRemaining() const = 0;
        virtual void SetVisible(bool isVisible) = 0;
        virtual bool IsVisible() const = 0;
        virtual bool IsAdditive() const = 0;
        virtual bool IsBlendAdd() const = 0;

        virtual void Stop() = 0;
        virtual void Pause() = 0;
        virtual void Resume() = 0;
        virtual void Restart() = 0;

        virtual void Update(float delta) = 0;

        virtual RandomXS128& GetRandom() = 0;
	};
	
}
