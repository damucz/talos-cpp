#pragma once

#include "modules/TalosAbstractModule.h"

namespace Talos
{

	class RandomRangeModule : public AbstractModule
	{
	public:
        enum T
        {
            MIN_INPUT = 0,
            MAX_INPUT = 1,
            OUTPUT = 0,
		};

		void Load(IFileProvider::ModuleData* moduleData) override;

        void SetMinMax(float min, float max);
        float GetMin() const;
        float GetMax() const;
        NumericalValue* GetOutputValue() const;

        bool IsDistributed() const;
        void SetDistributed(bool distributed);

	protected:
        NumericalValue* _minInput = nullptr;
        NumericalValue* _maxInput = nullptr;

        NumericalValue* _output = nullptr;

        float _min = 1.0f, _max = 1.0f;
        bool _distributed = false;

        RandomXS128 _random;

        void DefineSlots() override;
		void ProcessValues() override;
		
        //private DistributedRandom distributedRandom = new DistributedRandom();
	};

}
