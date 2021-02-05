#pragma once

#include "modules/TalosAbstractModule.h"

namespace Talos
{

	class ColorModule : public AbstractModule
	{
	public:
		enum T
		{
			R = 0,
			G = 1,
			B = 2,
			OUTPUT = 0,
		};

        void Load(IFileProvider::ModuleData* moduleData);

        void SetR(float r);
        void SetG(float g);
        void SetB(float b);
        void GetColor(float& r, float& g, float& b) const;          // a is always 1
	protected:
        NumericalValue* _r = nullptr;
        NumericalValue* _g = nullptr;
        NumericalValue* _b = nullptr;
        NumericalValue* _output = nullptr;

        float _defaultR = 1.0f, _defaultG = 0, _defaultB = 0;

        void DefineSlots() override;
        void ProcessValues() override;
	};
	
}
