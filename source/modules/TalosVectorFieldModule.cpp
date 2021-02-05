#if 0
#include "modules/TalosVectorFieldModule.h"
#include "values/TalosNumericalValue.h"

namespace Talos
{
	
	void VectorFieldModule::DefineSlots()
	{
		_scale = CreateInputSlot(SIZE_SCALE);
		_force = CreateInputSlot(FORCE_SCALE);
		_position = CreateInputSlot(POSITION);

		_angle = CreateOutputSlot(ANGLE);
		_velocity = CreateOutputSlot(VELOCITY);
	}
	
}
#endif