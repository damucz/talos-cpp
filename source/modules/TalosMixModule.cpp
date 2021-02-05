#include "modules/TalosMixModule.h"
#include "values/TalosNumericalValue.h"
#include <algorithm>

namespace Talos
{
	
	void MixModule::DefineSlots()
	{
		_alpha = CreateInputSlot(ALPHA);
		_val1 = CreateInputSlot(VAL1);
		_val2 = CreateInputSlot(VAL2);

		_output = CreateOutputSlot(OUTPUT);
	}

	void MixModule::ProcessValues()
	{
		int count = std::max(_val1->ElementsCount(), _val2->ElementsCount());
		for (int i = 0; i < count; i++)
		{
			float a = _val1->GetElements()[i];
			float b = _val2->GetElements()[i];
			_output->Set(i, a + (b - a) * _alpha->GetFloat());
		}
		_output->SetElementsCount(count);
	}
	
}
