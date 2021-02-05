#pragma once

#include "modules/TalosAbstractModule.h"
#include "TalosTypes.h"

namespace Talos
{

	class NumericalValue;

	class GradientColorModule : public AbstractModule
	{
	public:
		enum T
		{
			ALPHA = 0,
			OUTPUT = 0,
		};

		void Init() override;
		void Load(IFileProvider::ModuleData* moduleData) override;

		void ProcessValues() override;

		const std::vector<ColorPoint>& GetPoints() const;

		ColorPoint CreatePoint(float r, float g, float b, float pos);
		void RemovePoint(int index);

		void GetPosColor(float pos, float& r, float& g, float& b) const;
		
	protected:
		NumericalValue* _alpha = nullptr;
		NumericalValue* _output = nullptr;
		
		std::vector<ColorPoint> _points;

		void DefineSlots() override;
		void ProcessAlphaDefaults();
		void Interpolate(float alpha, NumericalValue* output);
		void ResetPoints();
	};

}
