#pragma once

#include "modules/TalosAbstractModule.h"

namespace Talos
{

	class RadToCartModule : public AbstractModule
	{
	public:
        enum T
        {
            A = 0,
        	L = 1,
        	OUTPUT = 0,
		};
	protected:
        NumericalValue* _a = nullptr;
        NumericalValue* _l = nullptr;
        NumericalValue* _output = nullptr;

        void DefineSlots() override;
        void ProcessValues() override;
	};
	
}
