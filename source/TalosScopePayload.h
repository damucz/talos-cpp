#pragma once
#include <map>

namespace Talos
{

    class Particle;
    class NumericalValue;

    class ScopePayload
    {
    public:
        enum T
        {
            EMITTER_ALPHA = 0,
            PARTICLE_ALPHA = 1,
            PARTICLE_SEED = 2,
            REQUESTER_ID = 3,
            EMITTER_ALPHA_AT_P_INIT = 4,
            DRAWABLE_ASPECT_RATIO = 5,
            SECONDARY_SEED = 6,
            TOTAL_TIME = 7,
            PARTICLE_POSITION = 8,

            TLAST = 9,
        };

        ScopePayload();
        ~ScopePayload();

        void Set(int index, float value);

        void SetParticle(Particle* particle);
        Particle* CurrParticle() const;

        void Set(int index, float x, float y);
        void Set(int index, NumericalValue* value);
        NumericalValue* Get(int index) const;
        float GetFloat(int index) const;

        void Reset();

        NumericalValue* GetDynamicValue(int key) const;
        void SetDynamicValue(int key, float val);
        void SetDynamicValue(int key, float x, float y);
        void SetDynamicValue(int key, float x, float y, float z);
        void SetDynamicValue(int key, NumericalValue* val);

    protected:
        Particle* _processingParticleRef = nullptr;

        std::map<int, NumericalValue*> _map;
        std::map<int, NumericalValue*> _dynamicValues;
	};

}