#pragma once

#include "TalosAbstractModule.h"

namespace Talos
{

	class CartToRadModule : public AbstractModule
	{
	public:
        enum T
        {
            POSITION = 0,
        	ANGLE = 0,
        	LENGTH = 1,
		};

    protected:
        NumericalValue* _position = nullptr;
        NumericalValue* _angle = nullptr;
        NumericalValue* _length = nullptr;

        void DefineSlots() override;
        void ProcessValues() override;
	};
	
}