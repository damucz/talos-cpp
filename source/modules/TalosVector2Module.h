#pragma once

#include "modules/TalosAbstractModule.h"

namespace Talos
{

	class Vector2Module : public AbstractModule
	{
	public:
		enum T
		{
			X = 0,
			Y = 1,
			OUTPUT = 0,
		};

		void Load(IFileProvider::ModuleData* moduleData) override;

        void SetX(float x);
        void SetY(float y);
        float GetDefaultX() const;
        float GetDefaultY() const;

	protected:
        NumericalValue* _x = nullptr;
        NumericalValue* _y = nullptr;
        NumericalValue* _output = nullptr;

        float _defaultX = 0, _defaultY = 0;

        void DefineSlots() override;
        void ProcessValues() override;
	};
	
}
