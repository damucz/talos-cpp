#pragma once

#include "modules/TalosAbstractModule.h"

namespace Talos
{

	class AttractorModule : public AbstractModule
	{
	public:
		enum T
		{
			INITIAL_ANGLE = 0,
			INITIAL_VELOCITY = 1,
			ATTRACTOR_POSITION = 2,         // maybe change to support multiple attractors later
			ALPHA = 3,
			ANGLE = 0,
			VELOCITY = 1,
		};
	protected:
        NumericalValue* _initialAngle = nullptr;
        NumericalValue* _initialVelocity = nullptr;
        NumericalValue* _attractorPosition = nullptr;
        NumericalValue* _alpha = nullptr;
        NumericalValue* _angle = nullptr;
        NumericalValue* _velocity = nullptr;

        float _initialVectorX = 0, _initialVectorY = 0;
        float _attractionVectorX = 0, _attractionVectorY = 0;
        float _posX = 0, _posY = 0;
        float _resultX = 0, _resultY = 0;

        void DefineSlots() override;
        void ProcessValues() override;
	};
	
}
