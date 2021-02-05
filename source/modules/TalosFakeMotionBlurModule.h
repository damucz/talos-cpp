#pragma once

#include "modules/TalosAbstractModule.h"

namespace Talos
{

	class FakeMotionBlurModule : public AbstractModule
	{
	public:
        enum T
        {
            VELOCITY = 0,
            SIZE = 0,
		};

		void Load(IFileProvider::ModuleData* moduleData) override;

        float GetVelocityMin() const;
        void SetVelocityMin(float velocityMin);
        float GetVelocityMax() const;
        void SetVelocityMax(float velocityMax);
        float GetSizeMin() const;
        void SetSizeMin(float sizeMin);
        float GetSizeMax() const;
        void SetSizeMax(float sizeMax);
		
	protected:
        NumericalValue* _velocity = nullptr;
        NumericalValue* _size = nullptr;

        float _velocityMin = 0;
        float _velocityMax = 0;
        float _sizeMin = 0;
        float _sizeMax = 0;

        void DefineSlots() override;
        void ProcessValues() override;
	};

}
