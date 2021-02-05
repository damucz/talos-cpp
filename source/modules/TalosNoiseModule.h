#pragma once

#include "modules/TalosAbstractModule.h"

namespace Talos
{

	class NoiseModule : public AbstractModule
	{
	public:
		enum T
		{
			X = 0,
			Y = 1,
			OUTPUT = 0,
		};

		void Load(IFileProvider::ModuleData* moduleData) override;

        void SetFrequency(float frequency);
        float GetFrequency() const;
		
	protected:
        NumericalValue* _x = nullptr;
        NumericalValue* _y = nullptr;
        NumericalValue* _output = nullptr;

        float _frequency = 20.0f;

        void DefineSlots() override;
        void ProcessValues() override;

        float NoiseFunction(float x, float y);

		float NoiseQuery(float x, float y, float frequency);
	};
	
}