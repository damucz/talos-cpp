#include "TalosScopePayload.h"
#include "values/TalosNumericalValue.h"

namespace Talos
{

	ScopePayload::ScopePayload()
	{
        for (int i = 0; i < TLAST; ++i)
        {
            _map[i] = new NumericalValue;
            _dynamicValues[i] = new NumericalValue;
        }
	}

	ScopePayload::~ScopePayload()
	{
        for (auto it = _map.begin(); it != _map.end(); ++it)
            delete it->second;
        for (auto it = _dynamicValues.begin(); it != _dynamicValues.end(); ++it)
            delete it->second;
	}

	void ScopePayload::Set(int index, float value)
    {
        _map[index]->Set(value);
    }

    void ScopePayload::SetParticle(Particle* particle)
    {
        _processingParticleRef = particle;
    }

    Particle* ScopePayload::CurrParticle() const
    {
        return _processingParticleRef;
    }

    void ScopePayload::Set(int index, float x, float y)
    {
        _map[index]->Set(x, y);
    }

    void ScopePayload::Set(int index, NumericalValue* value)
    {
        _map[index]->Set(value);
    }

    NumericalValue* ScopePayload::Get(int index) const
    {
        return _map.at(index);
    }

    float ScopePayload::GetFloat(int index) const
    {
        return _map.at(index)->GetFloat();
    }

    void ScopePayload::Reset()
    {
        for (int i = 0; i < TLAST; ++i)
            _map[i]->Set(0.0f);
    }

    NumericalValue* ScopePayload::GetDynamicValue(int key) const
    {
        return _dynamicValues.at(key);
    }

    void ScopePayload::SetDynamicValue(int key, float val)
    {
        _dynamicValues[key]->Set(val);
    }

    void ScopePayload::SetDynamicValue(int key, float x, float y)
    {
        _dynamicValues[key]->Set(x, y);
    }

    void ScopePayload::SetDynamicValue(int key, float x, float y, float z)
    {
        _dynamicValues[key]->Set(x, y, z);
    }

    void ScopePayload::SetDynamicValue(int key, NumericalValue* val)
    {
        _dynamicValues[key]->Set(val);
    }

}