#include "modules/TalosDynamicRangeModule.h"
#include "values/TalosNumericalValue.h"
#include "TalosScopePayload.h"

namespace Talos
{
	void DynamicRangeModule::Load(IFileProvider::ModuleData* moduleData)
	{
		CurveModule::Load(moduleData);
		_lowMin = moduleData->GetFloat("lowMin");
		_lowMax = moduleData->GetFloat("lowMax");
		_highMin = moduleData->GetFloat("highMin");
		_highMax = moduleData->GetFloat("highMax");
	}

	void DynamicRangeModule::SetMinMaxHigh(float highMin, float highMax)
	{
		_highMin = highMin;
		_highMax = highMax;
	}

	void DynamicRangeModule::SetMinMaxLow(float lowMin, float lowMax)
	{
		_lowMin = lowMin;
		_lowMax = lowMax;
	}

	float DynamicRangeModule::GetLowMin() const
	{
		return _lowMin;
	}

	float DynamicRangeModule::GetLowMax() const
	{
		return _lowMax;
	}

	float DynamicRangeModule::GetHighMin() const
	{
		return _highMin;
	}

	float DynamicRangeModule::GetHighMax() const
	{
		return _highMax;
	}

	NumericalValue* DynamicRangeModule::GetOutputValue() const
	{
		return _output;
	}

	void DynamicRangeModule::ProcessValues()
	{
        ProcessAlphaDefaults();

        // do the random thing first
        float low = CalcRandomRange(_lowMin, _lowMax, 1);
        float high = CalcRandomRange(_highMin, _highMax, 2);

        CurveModule::ProcessValues();

        float mix = low + (high - low) * _output->GetFloat();

        _output->Set(mix);
	}

	float DynamicRangeModule::CalcRandomRange(float min, float max, int randomOffset)
	{
		_random.SetSeed((unsigned)(GetScope()->GetFloat(ScopePayload::PARTICLE_SEED) * 10000 + (_index + randomOffset) * 1000));
		float startPos = _random.NextFloat();
		return min + (max - min) * startPos;
	}
	
}
