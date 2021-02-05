#include "modules/TalosAttractorModule.h"
#include "values/TalosNumericalValue.h"
#include "TalosScopePayload.h"
#define _USE_MATH_DEFINES
#include <math.h>

namespace Talos
{
	
	void AttractorModule::DefineSlots()
	{
		_initialAngle = CreateInputSlot(INITIAL_ANGLE);
		_initialVelocity = CreateInputSlot(INITIAL_VELOCITY);
		_attractorPosition = CreateInputSlot(ATTRACTOR_POSITION);
		_alpha = CreateInputSlot(ALPHA);

		_angle = CreateOutputSlot(ANGLE);
		_velocity = CreateOutputSlot(VELOCITY);

		_initialAngle->SetFlavour(NumericalValue::ANGLE);
	}

	void AttractorModule::ProcessValues()
	{
		NumericalValue* posNumVal = GetScope()->Get(ScopePayload::PARTICLE_POSITION);
		_posX = posNumVal->Get(0);
		_posY = posNumVal->Get(1);

		float alphaVal = GetScope()->GetFloat(ScopePayload::PARTICLE_ALPHA);;
		if (!_alpha->IsEmpty())
			alphaVal = _alpha->GetFloat();

		_initialVectorX = _initialVelocity->GetFloat();
		_initialVectorY = 0;
		// rotate
		float angle1 = _initialAngle->GetFloat() * (float)M_PI / 180.0f;
		float cos = cosf(angle1);
		float sin = sinf(angle1);
		_initialVectorX = _initialVectorX * cos - _initialVectorY * sin;
		_initialVectorY = _initialVectorY * sin + _initialVectorY * cos;

		_attractionVectorX = _attractorPosition->Get(0) - _posX;
		_attractionVectorY = _attractorPosition->Get(1) - _posY;
		// nor
		float len = sqrtf(_attractionVectorX * _attractionVectorX + _attractionVectorY * _attractionVectorY);
		if (len != 0)
		{
			_attractionVectorX /= len;
			_attractionVectorY /= len;
		}
		// scale
		_attractionVectorX *= _initialVelocity->GetFloat();
		_attractionVectorY *= _initialVelocity->GetFloat();

		// now let's mix them
		_resultX = _initialVectorX + (_attractionVectorX - _initialVectorX) * alphaVal;
		_resultY = _initialVectorY + (_attractionVectorY - _initialVectorY) * alphaVal;

		float angle2 = atan2f(_resultY, _resultX) * 180.0f / (float)M_PI;
		if (angle2 < 0) angle2 += 360.0f;
		_angle->Set(angle2);

		_velocity->Set(sqrtf(_resultX * _resultX + _resultY * _resultY));
	}
	
}
