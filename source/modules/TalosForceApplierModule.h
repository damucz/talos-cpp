#pragma once

#include "modules/TalosAbstractModule.h"

namespace Talos
{

	class ForceApplierModule : public AbstractModule
	{
	public:
		enum T
		{
			SUM_FORCES = 0,
			ANGLE = 0,
			VELOCITY = 1,
		};
	protected:
        NumericalValue* _sumForces = nullptr;
        NumericalValue* _angleOutput = nullptr;
        NumericalValue* _velocityOutput = nullptr;

        void DefineSlots() override;
        void ProcessValues() override;
	};
	
}
