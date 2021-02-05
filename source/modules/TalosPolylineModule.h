#pragma once

#include "modules/TalosAbstractModule.h"

namespace Talos
{

    class DrawableValue;
	class PolylineDrawable;

	class PolylineModule : public AbstractModule
	{
	public:
		enum T
		{
			OFFSET = 0,
			THICKNESS = 1,
			COLOR = 2,
			TRANSPARENCY = 3,
			LEFT_TANGENT = 4,
			RIGHT_TANGENT = 5,
			OUTPUT = 0,
		};

        ~PolylineModule();
		
		void Load(IFileProvider::ModuleData* moduleData) override;

        void FetchAllInputSlotValues() override;
        void SetModuleGraph(ParticleEmitterDescriptor* graph) override;

        void SetInterpolationPoints(int count);

	protected:
        NumericalValue* _offset = nullptr;
        NumericalValue* _thickness = nullptr;
        NumericalValue* _color = nullptr;
        NumericalValue* _transparency = nullptr;

        NumericalValue* _leftTangent = nullptr;
        NumericalValue* _rightTangent = nullptr;

        DrawableValue* _outputValue = nullptr;
		
		PolylineDrawable* _polylineDrawable = nullptr;
		int _pointCount = 2;

        char* _regionName = nullptr;

        void DefineSlots() override;
        void ProcessValues() override;
	};
	
}
