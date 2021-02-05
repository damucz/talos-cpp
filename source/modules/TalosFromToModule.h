#pragma once

#include "modules/TalosAbstractModule.h"

namespace Talos
{

	class FromToModule : public AbstractModule
	{
	public:
		enum T
		{
			FROM = 0,
			TO = 1,
			POSITION = 0,
			ROTATION = 1,
			LENGTH = 2,
		};

		void Load(IFileProvider::ModuleData* moduleData) override;

		void SetDefaults(float dFromX, float dFromY, float dToX, float dToY);
		
	protected:
        NumericalValue* _from = nullptr;
        NumericalValue* _to = nullptr;

        NumericalValue* _position = nullptr;
        NumericalValue* _rotation = nullptr;
        NumericalValue* _length = nullptr;

        float _defaultFromX = 0, _defaultFromY = 0;
        float _defaultToX = 0, _defaultToY = 0;

        void DefineSlots() override;
		void ProcessValues() override;
	};

}
