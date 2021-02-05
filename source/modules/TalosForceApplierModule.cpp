#include "modules/TalosForceApplierModule.h"
#include "values/TalosNumericalValue.h"
#include "TalosScopePayload.h"
#include "TalosParticle.h"
#define _USE_MATH_DEFINES
#include <math.h>

namespace Talos
{
	
	void ForceApplierModule::DefineSlots()
	{
		_sumForces = CreateInputSlot(SUM_FORCES);

		_angleOutput = CreateOutputSlot(ANGLE);
		_velocityOutput = CreateOutputSlot(VELOCITY);
	}

	void ForceApplierModule::ProcessValues()
	{
		float particleAlpha = GetScope()->GetFloat(ScopePayload::PARTICLE_ALPHA);
		if (GetScope()->CurrParticle() == nullptr) return;

		float particleLife = GetScope()->CurrParticle()->_life;
		float timePassed = particleAlpha * particleLife;

		float x = _sumForces->Get(0);
		float y = _sumForces->Get(1);

		float angle = atan2f(y, x) * 180.0f / (float)M_PI;
		if (angle < 0) angle += 360.0f;

		float velocity = sqrtf(x * x + y * y) * timePassed; // V = F * T for mass 1;

		_angleOutput->Set(angle);
		_velocityOutput->Set(velocity);
	}
	
}
