#pragma once

#include "modules/TalosAbstractModule.h"

namespace Talos
{

	class InterpolationModule : public AbstractModule
	{
	public:
		enum T
		{
			ALPHA = 0,
			OUTPUT = 0,
		};
		enum Interpolation
		{
			Bounce,
			BounceIn,
			BounceOut,
			Circle,
			CircleIn,
			CircleOut,
			Elastic,
			ElasticIn,
			ElasticOut,
			Exp5,
			Exp5In,
			Exp5Out,
			Exp10,
			Exp10In,
			Exp10Out,
			Fade,
			FastSlow,
			Linear,
			Pow2,
			Pow2In,
			Pow2InInverse,
			Pow2Out,
			Pow2OutInverse,
			Pow3,
			Pow3In,
			Pow3InInverse,
			Pow3Out,
            Pow3OutInverse,
			Pow4,
			Pow4In,
			Pow4Out,
			Pow5,
			Pow5In,
			Pow5Out,
			Sine,
			SineIn,
			SineOut,
			SlowFast,
			Smooth,
			Smooth2,
			Smoother,
			Swing,
			SwingIn,
            SwingOut,
		};

		void Load(IFileProvider::ModuleData* moduleData) override;

		void SetInterpolation(Interpolation interpolation);
		Interpolation GetInterpolation() const;
		
	protected:
        NumericalValue* _alpha = nullptr;
        NumericalValue* _output = nullptr;

		Interpolation _currentInterpolation = Linear;

		void DefineSlots() override;
		void ProcessValues() override;

		static float Interpolate(Interpolation interpolation, float a);
		static Interpolation GetInterpolationForName(const char* interp);
	};

}
