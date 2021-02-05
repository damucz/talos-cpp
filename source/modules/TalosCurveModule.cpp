#include "modules/TalosCurveModule.h"
#include "values/TalosNumericalValue.h"
#include "TalosScopePayload.h"
#include <algorithm>

namespace Talos
{
	void CurveModule::Init()
	{
		AbstractModule::Init();
		ResetPoints();
	}

	void CurveModule::Load(IFileProvider::ModuleData* moduleData)
	{
		AbstractModule::Load(moduleData);
		
		auto points = moduleData->GetPoints("points");
		_points.clear();
		for (auto p : points)
			CreatePoint(p.x, p.y);
	}

	int CurveModule::CreatePoint(float x, float y)
	{
		if (x < 0) x = 0;
		if (x > 1) x = 1;
		if (y < 0) y = 0;
		if (y > 1) y = 1;

		Point p{ x, y };
		_points.push_back(p);

		SortPoints();

		for (int i = 0; i < (int)_points.size(); ++i)
		{
			Point& t = _points.at(i);
			if (t.x == p.x && t.y == p.y)
				return i;
		}
		return -1;
	}

	void CurveModule::RemovePoint(int index)
	{
		if (_points.size() > 1)
		{
			auto it = _points.begin();
			std::advance(it, index);
			_points.erase(it);
		}
	}

	void CurveModule::DefineSlots()
	{
		_alpha = CreateInputSlot(ALPHA);
		_output = CreateOutputSlot(OUTPUT);
	}

	void CurveModule::ResetPoints()
	{
		// need to guarantee at least one point
		_points.clear();
		_points.push_back({ 0, 0.5f });
	}

	void CurveModule::SortPoints()
	{
		std::sort(_points.begin(), _points.end(), [](const Point& a, const Point& b) -> bool
		{
			return a.x < b.x;
		});
	}

	void CurveModule::ProcessValues()
	{
		ProcessAlphaDefaults();
		_output->Set(Interpolate(_alpha->GetFloat()));
	}

	void CurveModule::ProcessAlphaDefaults()
	{
		if (_alpha->IsEmpty())
		{
			// as default we are going to fetch the lifetime or duration depending on context
			float requester = GetScope()->GetFloat(ScopePayload::REQUESTER_ID);
			if (requester < 1)
			{
				// particle
				_alpha->Set(GetScope()->Get(ScopePayload::PARTICLE_ALPHA));
				_alpha->SetEmpty(false);
			}
			else if (requester > 1)
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

	float CurveModule::Interpolate(float alpha)
	{
		// interpolate alpha in this point space

		if (_points[0].x >= 0 && alpha <= _points[0].x)
		{
			return _points[0].y;
		}

		for (int i = 0; i < (int)_points.size() - 1; ++i)
		{
			Point& from = _points[i];
			Point& to = _points[i + 1];
			if (alpha > from.x && alpha <= to.x)
			{
				float localAlpha = 1.0f;
				if (from.x != to.x)
				{
					localAlpha = (alpha - from.x) / (to.x - from.x);
				}
				return Interpolate(localAlpha, from.x, from.y, to.x, to.y);
			}
		}

		if (_points[_points.size() - 1].x <= 1.0f && alpha >= _points[_points.size() - 1].x)
		{
			return _points[_points.size() - 1].y;
		}

		return 0;
	}

	float CurveModule::Interpolate(float alpha, float x1, float y1, float x2, float y2)
	{
		if (y1 == y2) return y1;
		if (x1 == x2) return y1;

		return (y2 - y1) * alpha + y1;
	}
	
}
