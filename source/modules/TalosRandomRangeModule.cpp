#include "modules/TalosRandomRangeModule.h"
#include "values/TalosNumericalValue.h"
#include "TalosScopePayload.h"

namespace Talos
{
	
	void RandomRangeModule::Load(IFileProvider::ModuleData* moduleData)
	{
		AbstractModule::Load(moduleData);
		
		_min = moduleData->GetFloat("min");
		_max = moduleData->GetFloat("max");
		_distributed = moduleData->GetBool("distributed");
	}

	void RandomRangeModule::SetMinMax(float min, float max)
	{
        _min = min;
        _max = max;
	}

	float RandomRangeModule::GetMin() const
	{
        return _min;
	}

	float RandomRangeModule::GetMax() const
	{
        return _max;
	}

	NumericalValue* RandomRangeModule::GetOutputValue() const
	{
		return _output;
	}

	bool RandomRangeModule::IsDistributed() const
	{
		return _distributed;
	}

	void RandomRangeModule::SetDistributed(bool distributed)
	{
		_distributed = distributed;
	}

	void RandomRangeModule::DefineSlots()
	{
		_minInput = CreateInputSlot(MIN_INPUT);
		_maxInput = CreateInputSlot(MAX_INPUT);

		_output = CreateOutputSlot(OUTPUT);
	}

	void RandomRangeModule::ProcessValues()
	{
        // what's worse, keeping thousands of long values, or keeping floats but casting 1000 times to long?
		// I'll leave the answer to the reader
        unsigned seed = (unsigned)(GetScope()->GetFloat(ScopePayload::PARTICLE_SEED) * 10000.0f * (float)(_index + 1) * 1000.0f);

        float startPos;
        //if (!_distributed)
        {
			_random.SetSeed(seed);
			startPos = _random.NextFloat();
        }
        //else
        //{
        //    distributedRandom.setSeed((int)(10000 * (index + 1)));
        //    startPos = distributedRandom.nextFloat();
        //}

        float min = _min;
        float max = _max;

        if (!_minInput->IsEmpty()) min = _minInput->GetFloat();
        if (!_maxInput->IsEmpty()) max = _maxInput->GetFloat();

        float res = min + (max - min) * startPos;

        _output->Set(res);
	}
	
}
