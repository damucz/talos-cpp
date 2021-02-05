#include "modules/TalosInterpolationModule.h"
#include "values/TalosNumericalValue.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <assert.h>

namespace Talos
{
	
	void InterpolationModule::Load(IFileProvider::ModuleData* moduleData)
	{
        AbstractModule::Load(moduleData);
		
        _currentInterpolation = GetInterpolationForName(moduleData->GetString("interp"));
	}

	void InterpolationModule::SetInterpolation(Interpolation interpolation)
	{
        _currentInterpolation = interpolation;
	}

	InterpolationModule::Interpolation InterpolationModule::GetInterpolation() const
	{
        return _currentInterpolation;
	}

	void InterpolationModule::DefineSlots()
	{
		_alpha = CreateInputSlot(ALPHA);
		_output = CreateOutputSlot(OUTPUT);
	}

	void InterpolationModule::ProcessValues()
	{
		_output->Set(Interpolate(_currentInterpolation, _alpha->GetFloat()));
	}

	float InterpolationModule::Interpolate(Interpolation interpolation, float a)
	{
        auto ipow = [](float a, int power)
        {
            if (a <= 0.5f) return powf(a * 2.0f, (float)power) / 2.0f;
            return powf((a - 1.0f) * 2.0f, (float)power) / (power % 2 == 0 ? -2.0f : 2.0f) + 1.0f;
        };
        auto ipowIn = [](float a, int power)
        {
            return powf(a, (float)power);
        };
        auto ipowOut = [](float a, int power)
        {
            return powf(a - 1.0f, (float)power) * (power % 2 == 0 ? -1.0f : 1.0f) + 1.0f;
        };
		
        auto iexp = [](float a, float value, float power)
        {
            float min = powf(value, -power);
            float scale = 1.0f / (1.0f - min);

            if (a <= 0.5f) return (powf(value, power * (a * 2.0f - 1.0f)) - min) * scale / 2.0f;
            return (2.0f - (powf(value, -power * (a * 2.0f - 1.0f)) - min) * scale) / 2.0f;
        };
        auto iexpIn = [](float a, float value, float power)
        {
            float min = powf(value, -power);
            float scale = 1.0f / (1.0f - min);

            return (powf(value, power * (a - 1.0f)) - min) * scale;
        };
        auto iexpOut = [](float a, float value, float power)
        {
            float min = powf(value, -power);
            float scale = 1.0f / (1.0f - min);
        	
            return 1.0f - (powf(value, -power * a) - min) * scale;
        };
		
        auto ielastic = [](float a, float value, float power, int bounces, float scale)
        {
            float fbounces = (float)bounces * (float)M_PI * (bounces % 2 == 0 ? 1.0f : -1.0f);

            if (a <= 0.5f)
            {
                a *= 2.0f;
                return powf(value, power * (a - 1.0f)) * sinf(a * fbounces) * scale / 2.0f;
            }
            a = 1.0f - a;
            a *= 2.0f;
            return 1.0f - powf(value, power * (a - 1.0f)) * sinf(a * fbounces) * scale / 2.0f;
        };
        auto ielasticIn = [](float a, float value, float power, int bounces, float scale)
        {
            float fbounces = (float)bounces * (float)M_PI * (bounces % 2 == 0 ? 1.0f : -1.0f);
        	
            if (a >= 0.99f) return 1.0f;
            return powf(value, power * (a - 1.0f)) * sinf(a * fbounces) * scale;
        };
        auto ielasticOut = [](float a, float value, float power, int bounces, float scale)
        {
            float fbounces = (float)bounces * (float)M_PI * (bounces % 2 == 0 ? 1.0f : -1.0f);
        	
            if (a == 0) return 0.0f;
            a = 1.0f - a;
            return 1.0f - powf(value, power * (a - 1.0f)) * sinf(a * fbounces) * scale;
        };

        auto ibounce = [](float a)
        {
            const int bounces = 4;
            float widths[4] = { 0.68f, 0.34f, 0.2f, 0.15f };
            float heights[4] = { 1.0f, 0.26f, 0.11f, 0.03f };

            if (a == 1.0f) return 1.0f;
            a += widths[0] / 2.0f;
            float width = 0, height = 0;
            for (int i = 0, n = bounces; i < n; i++)
            {
                width = widths[i];
                if (a <= width)
                {
                    height = heights[i];
                    break;
                }
                a -= width;
            }
            a /= width;
            float z = 4.0f / width * height * a;
            return 1.0f - (z - z * a) * width;
        };
        auto ibounceIn = [ibounce](float a)
        {
            return 1.0f - ibounce(1.0f - a);
        };
        auto ibounceOut = [ibounce](float a)
        {
            auto out = [ibounce](float a)
            {
                float test = a + 0.68f / 2.0f;         // widths[0] = 0.68f
                if (test < 0.68f) return test / (0.68f / 2.0f) - 1.0f;
                return ibounce(a);
            };
            if (a <= 0.5f) return (1.0f - out(1.0f - a * 2.0f)) / 2.0f;
            return out(a * 2.0f - 1.0f) / 2.0f + 0.5f;
        };

        auto iswing = [](float a)
        {
            float scale = 3.0f;
        	
            if (a <= 0.5f)
            {
                a *= 2.0f;
                return a * a * ((scale + 1.0f) * a - scale) / 2.0f;
            }
            a--;
            a *= 2.0f;
            return a * a * ((scale + 1.0f) * a + scale) / 2.0f + 1.0f;
        };
        auto iswingIn = [](float a)
        {
            float scale = 2.0f;

            return a * a * ((scale + 1.0f) * a - scale);
        };
        auto iswingOut = [](float a)
        {
            float scale = 2.0f;

            a--;
            return a * a * ((scale + 1.0f) * a + scale) + 1.0f;
        };
		
		switch (interpolation)
		{
        case Bounce: return ibounce(a);
        case BounceIn: return ibounceIn(a);
        case BounceOut: return ibounceOut(a);
        case Circle: if (a <= 0.5f) { a *= 2.0f; return (1.0f - sqrtf(1.0f - a * a)) / 2.0f; } a--; a *= 2.0f; return (sqrtf(1.0f - a * a) + 1.0f) / 2.0f;
        case CircleIn: return 1.0f - sqrtf(1.0f - a * a);
        case CircleOut: a--; return sqrtf(1.0f - a * a);
        case Elastic: return ielastic(a, 2.0f, 10.0f, 7, 1.0f);
        case ElasticIn: return ielasticIn(a, 2.0f, 10.0f, 6, 1.0f);
        case ElasticOut: return ielasticOut(a, 2.0f, 10.0f, 7, 1.0f);
        case Exp5: return iexp(a, 2.0f, 5.0f);
        case Exp5In: return iexpIn(a, 2.0f, 5.0f);
        case Exp5Out: return iexpOut(a, 2.0f, 5.0f);
        case Exp10: return iexp(a, 2.0f, 10.0f);
        case Exp10In: return iexpIn(a, 2.0f, 10.0f);
        case Exp10Out: return iexpOut(a, 2.0f, 10.0f);
        case Linear: return a;
        case Pow2: return ipow(a, 2);
        case SlowFast:
        case Pow2In: return ipowIn(a, 2);
        case Pow2InInverse: if (a < FLT_EPSILON) return 0; return sqrtf(a);
        case FastSlow:
        case Pow2Out: return ipowOut(a, 2);
        case Pow2OutInverse: if (a <FLT_EPSILON) return 0; return 1.0f - sqrtf(-(a - 1.0f));
        case Pow3: return ipow(a, 3);
        case Pow3In: return ipowIn(a, 3);
        case Pow3InInverse: return powf(a, 1.0f / 3.0f);
        case Pow3Out: return ipowOut(a, 3);
        case Pow3OutInverse: return powf(-(a - 1.0f), 1.0f / 3.0f);
        case Pow4: return ipow(a, 4);
        case Pow4In: return ipowIn(a, 4);
        case Pow4Out: return ipowOut(a, 4);
        case Pow5: return ipow(a, 5);
        case Pow5In: return ipowIn(a, 5);
        case Pow5Out: return ipowOut(a, 5);
        case Sine: return (1.0f - cosf(a * (float)M_PI)) / 2.0f;
        case SineIn: return 1.0f - cosf(a * (float)M_PI_2);
        case SineOut: return sinf(a * (float)M_PI_2);
        case Smooth: return a * a * (3 - 2 * a);
        case Smooth2: a = a * a * (3 - 2 * a); return a * a * (3 - 2 * a);
        case Fade:
        case Smoother: return a * a * a * (a * (a * 6 - 15) + 10);
        case Swing: return iswing(a);
        case SwingIn: return iswingIn(a);
        case SwingOut: return iswingOut(a);
		}

        return a;
	}

	InterpolationModule::Interpolation InterpolationModule::GetInterpolationForName(const char* interp)
	{
        struct { const char* name; Interpolation interp; } interplist[] = {
        	{ "bounce", Bounce },
            { "bounceIn", BounceIn },
            { "bounceOut", BounceOut },
            { "Circle", Circle },
            { "CircleIn", CircleIn },
            { "CircleOut", CircleOut },
            { "Elastic", Elastic },
            { "ElasticIn", ElasticIn },
            { "ElasticOut", ElasticOut },
            { "Exp5", Exp5 },
            { "Exp5In", Exp5In },
            { "Exp5Out", Exp5Out },
            { "Exp10", Exp10 },
            { "Exp10In", Exp10In },
            { "Exp10Out", Exp10Out },
            { "Fade", Fade },
            { "FastSlow", FastSlow },
            { "Linear", Linear },
            { "Pow2", Pow2 },
            { "Pow2In", Pow2In },
            { "Pow2InInverse", Pow2InInverse },
            { "Pow2Out", Pow2Out },
            { "Pow2OutInverse", Pow2OutInverse },
            { "Pow3", Pow3 },
            { "Pow3In", Pow3In },
            { "Pow3InInverse", Pow3InInverse },
            { "Pow3Out", Pow3Out },
            { "Pow3OutInverse", Pow3OutInverse },
            { "Pow4", Pow4 },
            { "Pow4In", Pow4In },
            { "Pow4Out", Pow4Out },
            { "Pow5", Pow5 },
            { "Pow5In", Pow5In },
            { "Pow5Out", Pow5Out },
            { "Sine", Sine },
            { "SineIn", SineIn },
            { "SineOut", SineOut },
            { "SlowFast", SlowFast },
            { "Smooth", Smooth },
            { "Smooth2", Smooth2 },
            { "Smoother", Smoother },
            { "Swing", Swing },
            { "SwingIn", SwingIn },
            { "SwingOut", SwingOut },
        };

		for (int i = 0; i < (int)(sizeof(interplist) / sizeof(interplist[0])); ++i)
		{
            if (strcmp(interplist[i].name, interp) == 0)
                return interplist[i].interp;
		}

        assert(false);
        return Linear;
	}
	
}
