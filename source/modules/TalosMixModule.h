#pragma once

#include "modules/TalosAbstractModule.h"

namespace Talos
{

	class MixModule : public AbstractModule
	{
	public:
		enum T
		{
			ALPHA = 0,
			VAL1 = 1,
			VAL2 = 2,
			OUTPUT = 0,
		};
	protected:
        NumericalValue* _alpha = nullptr;
        NumericalValue* _val1 = nullptr;
        NumericalValue* _val2 = nullptr;
        NumericalValue* _output = nullptr;

        void DefineSlots() override;
        void ProcessValues() override;
	};

}
