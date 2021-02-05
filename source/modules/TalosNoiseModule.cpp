#include "modules/TalosNoiseModule.h"
#include "values/TalosNumericalValue.h"
#include "TalosScopePayload.h"
#include <algorithm>

namespace Talos
{
	
	void NoiseModule::Load(IFileProvider::ModuleData* moduleData)
	{
        AbstractModule::Load(moduleData);
		
		_frequency = moduleData->GetFloat("frequency", 20.0f);
	}

	void NoiseModule::SetFrequency(float frequency)
	{
		_frequency = frequency;
	}

	float NoiseModule::GetFrequency() const
	{
		return _frequency;
	}

	void NoiseModule::DefineSlots()
	{
		_x = CreateInputSlot(X);
		_y = CreateInputSlot(Y);

		_output = CreateOutputSlot(OUTPUT);
	}

	void NoiseModule::ProcessValues()
	{
 		_output->Set(NoiseFunction(_x->GetFloat(), _y->GetFloat()));
	}

	float NoiseModule::NoiseFunction(float x, float y)
	{
		// normalize
		x = x - (int)x;
		y = y - (int)y;

		float particleSeed = GetScope()->GetFloat(ScopePayload::PARTICLE_SEED);
		y = y * particleSeed;
		y = y - (int)y;

		float result = NoiseQuery(x, y, _frequency);

		//result = result * 0.5f + 0.5f; // bring to 0-1 range (actually no not needed)

		return result;
	}

	float NoiseModule::NoiseQuery(float x, float y, float frequency)
	{
        float C[4] = { 0.211324865405187f, 0.366025403784439f, -0.577350269189626f, 0.024390243902439f };

        float vx = x * frequency;
        float vy = y * frequency;

        // First corner (x0)
        float dot = vx * C[1] + vy * C[1];
        float ix = floorf(vx + dot);
        float iy = floorf(vy + dot);
        dot = ix * C[0] + iy * C[0];
        float x0x = vx - ix + dot;
        float x0y = vy - iy + dot;

        // Other two corners (x1, x2)
        float i1x, i1y;
        if (x0x > x0y) { i1x = 1.0f; i1y = 0.0f; }
        else { i1x = 0.0f; i1y = 1.0f; }
        float x1x = x0x + C[0] - i1x;
        float x1y = x0y + C[0] - i1y;
        float x2x = x0x + C[2];
        float x2y = x0y + C[2];

        // Do some permutations to avoid
        // truncation effects in permutation
        auto mod289 = [](float v)
        {
            return v - floorf(v * (1.0f / 289.0f)) * 289.0f;
        };
        ix = mod289(ix);
        iy = mod289(iy);

        auto permute = [](float x)
        {
            return ((x * 34.0f) + 1.0f) * x;
        };
        float px = iy;
        float py = i1y + iy;
        float pz = 1.0f + iy;
        px = mod289(permute(px));
        py = mod289(permute(py));
        pz = mod289(permute(pz));
        px += ix;
        py += ix + i1x;
        pz += ix + 1.0f;
        px = mod289(permute(px));
        py = mod289(permute(py));
        pz = mod289(permute(pz));

        float mx = 0.5f - (x0x * x0x + x0y * x0y);
        float my = 0.5f - (x1x * x1x + x1y * x1y);
        float mz = 0.5f - (x2x * x2x + x2y * x2y);
        mx = std::max(mx, 0.0f);
        my = std::max(my, 0.0f);
        mz = std::max(mz, 0.0f);
        mx = mx * mx * mx * mx;
        my = my * my * my * my;
        mz = mz * mz * mz * mz;

        // Gradients:
        //  41 pts uniformly over a line, mapped onto a diamond
        //  The ring size 17*17 = 289 is close to a multiple
        //      of 41 (41*7 = 287)
        float Xx = px * C[3];
        float Xy = py * C[3];
        float Xz = pz * C[3];
        auto fract = [](float v)
        {
            return v - floorf(v);
        };
        Xx = fract(Xx) * 2.0f - 1.0f;
        Xy = fract(Xy) * 2.0f - 1.0f;
        Xz = fract(Xz) * 2.0f - 1.0f;
        float hx = std::abs(Xx) - 0.5f;
        float hy = std::abs(Xy) - 0.5f;
        float hz = std::abs(Xz) - 0.5f;
        float oxx = floorf(Xx + 0.5f);
        float oxy = floorf(Xy + 0.5f);
        float oxz = floorf(Xz + 0.5f);
        float a0x = Xx - oxx;
        float a0y = Xy - oxy;
        float a0z = Xz - oxz;

        // Normalise gradients implicitly by scaling m
        // Approximation of: m *= inverse sqrt(a0*a0 + h*h);
        mx *= 1.79284291400159f - 0.85373472095314f * (a0x * a0x + hx * hx);
        my *= 1.79284291400159f - 0.85373472095314f * (a0y * a0y + hy * hy);
        mz *= 1.79284291400159f - 0.85373472095314f * (a0z * a0z + hz * hz);

        // Compute final noise value at P
        float gx = a0x * x0x + hx * x0y;
        float gy = x1x * a0y + x1y * hy;
        float gz = x2x * a0z + x2y * hz;

		// dot
        return 130.0f * (mx * gx + my * gy + mz * gz);
	}
	
}
