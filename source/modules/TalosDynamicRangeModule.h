#pragma once

#include "modules/TalosCurveModule.h"

namespace Talos
{

	class DynamicRangeModule : public CurveModule
	{
	public:
		void Load(IFileProvider::ModuleData* moduleData) override;

        void SetMinMaxHigh(float highMin, float highMax);
        void SetMinMaxLow(float lowMin, float lowMax);
        float GetLowMin() const;
        float GetLowMax() const;
        float GetHighMin() const;
        float GetHighMax() const;

        NumericalValue* GetOutputValue() const;
		
	protected:
        float _lowMin = 0, _lowMax = 0;
        float _highMin = 1.0f, _highMax = 1.0f;

        RandomXS128 _random;

        void ProcessValues() override;
        float CalcRandomRange(float min, float max, int randomOffset);
	};

}
