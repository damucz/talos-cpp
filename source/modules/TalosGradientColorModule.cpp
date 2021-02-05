#include "modules/TalosGradientColorModule.h"
#include "values/TalosNumericalValue.h"
#include "TalosScopePayload.h"
#include <algorithm>

namespace Talos
{
	
	void GradientColorModule::Init()
	{
		AbstractModule::Init();
		ResetPoints();
	}

	void GradientColorModule::Load(IFileProvider::ModuleData* moduleData)
	{
		AbstractModule::Load(moduleData);
		
		auto colorPoints = moduleData->GetColorPoints("points");
		_points.clear();
		for (auto p : colorPoints)
			CreatePoint(p.r, p.g, p.b, p.pos);
	}

	void GradientColorModule::ProcessValues()
	{
		ProcessAlphaDefaults();
		Interpolate(_alpha->GetFloat(), _output);
	}

	const std::vector<ColorPoint>& GradientColorModule::GetPoints() const
	{
		return _points;
	}

	ColorPoint GradientColorModule::CreatePoint(float r, float g, float b, float pos)
	{
		ColorPoint p{ r, g, b, pos };
		_points.push_back(p);
		std::sort(_points.begin(), _points.end(), [](const ColorPoint& a, const ColorPoint& b) -> bool
		{
			return a.pos < b.pos;
		});
		return p;
	}

	void GradientColorModule::RemovePoint(int index)
	{
		if (_points.size() > 1)
		{
			auto it = _points.begin();
			std::advance(it, index);
			_points.erase(it);
		}
	}

	void GradientColorModule::GetPosColor(float pos, float& r, float& g, float& b) const
	{
		if (pos <= _points[0].pos)
		{
			r = _points[0].r;
			g = _points[0].g;
			b = _points[0].b;
			return;
		}

		if (pos >= _points[_points.size() - 1].pos)
		{
			r = _points[_points.size() - 1].r;
			g = _points[_points.size() - 1].g;
			b = _points[_points.size() - 1].b;
			return;
		}

		for (int i = 0; i < (int)_points.size() - 1; i++)
		{
			if (_points[i].pos < pos && _points[i + 1].pos > pos)
			{
				// found it

				if (_points[i + 1].pos == _points[i].pos)
				{
					r = _points[i].r;
					g = _points[i].g;
					b = _points[i].b;
				}
				else {
					float localAlpha = (pos - _points[i].pos) / (_points[i + 1].pos - _points[i].pos);
					r = _points[i].r + (_points[i + 1].r - _points[i].r) * localAlpha;
					g = _points[i].g + (_points[i + 1].g - _points[i].g) * localAlpha;
					b = _points[i].b + (_points[i + 1].b - _points[i].b) * localAlpha;
				}
				break;
			}
		}
	}

	void GradientColorModule::DefineSlots()
	{
		_alpha = CreateInputSlot(ALPHA);
		_output = CreateOutputSlot(OUTPUT);
	}

	void GradientColorModule::ProcessAlphaDefaults()
	{
		if (_alpha->IsEmpty())
		{
			// as default we are going to fetch the lifetime or duration depending on context
			float requester = GetScope()->GetFloat(ScopePayload::REQUESTER_ID);
			if (requester < 1.0f)
			{
				// particle
				_alpha->Set(GetScope()->Get(ScopePayload::PARTICLE_ALPHA));
				_alpha->SetEmpty(false);
			}
			else if (requester > 1.0f)
			{
				// emitter
				_alpha->Set(GetScope()->Get(ScopePayload::EMITTER_ALPHA));
				_alpha->SetEmpty(false);
			}
			else
			{
				// whaat?
				_alpha->Set(0.0f);
			}
		}
	}

	void GradientColorModule::Interpolate(float alpha, NumericalValue* output)
	{
		float r, g, b;
		GetPosColor(alpha, r, g, b);
		_output->Set(r, g, b, 1.0f);
	}

	void GradientColorModule::ResetPoints()
	{
		// need to guarantee at least one point
		_points.clear();
		_points.push_back({ 255 / 255.0f, 68 / 255.0f, 26 / 255.0f, 1.0f });
	}
	
}
