#include "modules/TalosOffsetModule.h"
#include "values/TalosNumericalValue.h"
#include "TalosScopePayload.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

namespace Talos
{
	
	OffsetModule::~OffsetModule()
	{
		delete _lowPos;
		delete _lowSize;
		delete _highPos;
		delete _highSize;
	}

	void OffsetModule::Init()
	{
		AbstractModule::Init();
		ResetPoints();
	}

	void OffsetModule::Load(IFileProvider::ModuleData* moduleData)
	{
		AbstractModule::Load(moduleData);

		auto points = moduleData->GetPoints("points");
		_points.clear();
		for (auto p : points)
			CreatePoint(p.x, p.y);

		auto low = moduleData->GetSubData("low");
		auto high = moduleData->GetSubData("high");

		_lowEdge = low->GetBool("edge");
		_highEdge = high->GetBool("edge");

		_lowShape = low->GetInt("shape");
		_highShape = high->GetInt("shape");

		_lowSide = low->GetInt("side");
		_highSide = high->GetInt("side");

		auto lowRect = low->GetSubData("rect");
		auto highRect = high->GetSubData("rect");

		_lowPos->Set(lowRect->GetFloat("x"), lowRect->GetFloat("y"));
		_lowSize->Set(lowRect->GetFloat("width"), lowRect->GetFloat("height"));

		_highPos->Set(highRect->GetFloat("x"), highRect->GetFloat("y"));
		_highSize->Set(highRect->GetFloat("width"), highRect->GetFloat("height"));

		delete lowRect;
		delete highRect;
		delete low;
		delete high;
	}

	const std::vector<Point>& OffsetModule::GetPoints() const
	{
		return _points;
	}

	Point OffsetModule::CreatePoint(float x, float y)
	{
		if (x < 0) x = 0;
		if (x > 1) x = 1;
		if (y < 0) y = 0;
		if (y > 1) y = 1;

		Point p{ x, y };
		_points.push_back(p);
		std::sort(_points.begin(), _points.end(), [](const Point& a, const Point& b) -> bool
		{
			return a.x < b.x;
		});
		return p;
	}

	void OffsetModule::RemovePoint(int index)
	{
		if (_points.size() > 1)
		{
			auto it = _points.begin();
			std::advance(it, index);
			_points.erase(it);
		}
	}

	void OffsetModule::SetLowPos(float posx, float posy)
	{
		_lowPos->Set(posx, posy);
	}

	void OffsetModule::GetLowPos(float& resultx, float& resulty) const
	{
		resultx = _lowPos->Get(0);
		resulty = _lowPos->Get(1);
	}

	void OffsetModule::SetHighPos(float posx, float posy)
	{
		_highPos->Set(posx, posy);
	}

	void OffsetModule::GetHighPos(float& resultx, float& resulty) const
	{
		resultx = _highPos->Get(0);
		resulty = _highPos->Get(1);
	}

	void OffsetModule::SetLowSize(float sizex, float sizey)
	{
		_lowSize->Set(sizex, sizey);
	}

	void OffsetModule::GetLowSize(float& resultx, float& resulty) const
	{
		resultx = _lowSize->Get(0);
		resulty = _lowSize->Get(1);
	}

	void OffsetModule::SetHighSize(float sizex, float sizey)
	{
		_highSize->Set(sizex, sizey);
	}

	void OffsetModule::GetHighSize(float& resultx, float& resulty) const
	{
		resultx = _highSize->Get(0);
		resulty = _highSize->Get(1);
	}

	void OffsetModule::SetLowShape(int shape)
	{
		_lowShape = shape;
	}

	int OffsetModule::GetLowShape() const
	{
		return _lowShape;
	}

	void OffsetModule::SetHighShape(int shape)
	{
		_highShape = shape;
	}

	int OffsetModule::GetHighShape() const
	{
		return _highShape;
	}

	void OffsetModule::SetLowEdge(bool edge)
	{
		_lowEdge = edge;
	}

	bool OffsetModule::GetLowEdge() const
	{
		return _lowEdge;
	}

	void OffsetModule::SetHighEdge(bool edge)
	{
		_highEdge = edge;
	}

	bool OffsetModule::GetHighEdge() const
	{
		return _highEdge;
	}

	void OffsetModule::SetLowSide(int side)
	{
		_lowSide = side;
	}

	int OffsetModule::GetLowSide() const
	{
		return _lowSide;
	}

	void OffsetModule::SetHighSide(int side)
	{
		_highSide = side;;
	}

	int OffsetModule::GetHighSide() const
	{
		return _highSide;
	}

	void OffsetModule::ResetPoints()
	{
		// need to guarantee at least one point
		_points.clear();
		_points.push_back({ 0.0f, 0.5f });
	}

	void OffsetModule::DefineSlots()
	{
		_alpha = CreateInputSlot(ALPHA);
		_output = CreateOutputSlot(OUTPUT);

		_lowPos = new NumericalValue();
		_lowSize = new NumericalValue();
		_highPos = new NumericalValue();
		_highSize = new NumericalValue();
	}

	void OffsetModule::ProcessValues()
	{
		ProcessAlphaDefaults();

		float alpha = _alpha->GetFloat();

		alpha = Interpolate(alpha); // apply the curve

		// let's find pos by shape
		float randLowx, randLowy, randHighx, randHighy;
		GetRandomPosOn(_lowSide, _lowEdge, _lowShape, _lowPos, _lowSize, randLowx ,randLowy);
		GetRandomPosOn(_highSide, _highEdge, _highShape, _highPos, _highSize, randHighx, randHighy);

		float x = randLowx + (randHighx - randLowx) * alpha;
		float y = randLowy + (randHighy - randLowy) * alpha;

		_output->Set(x, y);
	}

	void OffsetModule::ProcessAlphaDefaults()
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

	float OffsetModule::Interpolate(float alpha)
	{
		// interpolate alpha in this point space

		if (_points[0].x >= 0 && alpha <= _points[0].x)
			return _points[0].y;

		for (int i = 0; i < (int)_points.size() - 1; i++)
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
			return _points[_points.size() - 1].y;

		return 0;
	}

	float OffsetModule::Interpolate(float alpha, float x1, float y1, float x2, float y2)
	{
		if (y1 == y2) return y1;
		if (x1 == x2) return y1;

		return y1 + (y2 - y1) * alpha;
	}

	void OffsetModule::GetRandomPosOn(int side, bool edge, int shape, NumericalValue* pos, NumericalValue* size,
		float& resultx, float& resulty)
	{
		_random.SetSeed((unsigned)(GetScope()->GetFloat(ScopePayload::PARTICLE_SEED) * 10000.0f * (float)_index * 1000.0f));
		float angle = _random.NextFloat();

		if (side == SIDE_TOP) angle = angle / 2.0f;
		if (side == SIDE_BOTTOM) angle = angle / 2.0f + 0.5f;
		if (side == SIDE_LEFT) angle = angle / 2.0f + 0.25f;
		if (side == SIDE_RIGHT) angle = angle / 2.0f - 0.25f;

		if (shape == TYPE_SQUARE) FindRandomSquarePos(angle, pos, size, resultx, resulty);
		else if (shape == TYPE_ELLIPSE) FindRandomEllipsePos(angle, pos, size, resultx, resulty);
		else if (shape == TYPE_LINE) FindRandomLinePos(angle, pos, size, resultx, resulty);

		float endX = resultx + _tolerance;
		float endY = resulty + _tolerance;
		float startX = resultx - _tolerance;
		float startY = resulty - _tolerance;
		if (!edge)
		{
			startX = pos->Get(0);
			startY = pos->Get(1);
		}

		resultx = _random.NextFloat() * (endX - startX) + startX;
		resulty = _random.NextFloat() * (endY - startY) + startY;
	}

	void OffsetModule::FindRandomSquarePos(float angle, NumericalValue* pos, NumericalValue* size, float& resultx,
		float& resulty)
	{
		angle = angle * 360.0f;
		float rectx = pos->Get(0) - size->Get(0) / 2.0f;
		float recty = pos->Get(1) - size->Get(1) / 2.0f;
		float rectw = size->Get(0);
		float recth = size->Get(1);
		IntersectSegmentRectangle(pos->Get(0), pos->Get(1),
			pos->Get(0) + rectw * cosf(angle * (float)M_PI / 180.0f),
			pos->Get(0) + rectw * cosf(angle * (float)M_PI / 180.0f),
			rectx, recty, rectw, recth,
			resultx, resulty);
	}

	void OffsetModule::FindRandomEllipsePos(float angle, NumericalValue* pos, NumericalValue* size, float& resultx,
		float& resulty)
	{
		angle = angle * 360.0f;

		resultx = cosf(angle * (float)M_PI / 180.0f) * size->Get(0) / 2.0f + pos->Get(0);
		resulty = sinf(angle * (float)M_PI / 180.0f) * size->Get(1) / 2.0f + pos->Get(1);
	}

	void OffsetModule::FindRandomLinePos(float angle, NumericalValue* pos, NumericalValue* size, float& resultx,
		float& resulty)
	{
		angle = angle * 360.0f;
		float rectw = size->Get(0);
		float recth = size->Get(1);
		float alpha = atan2f(recth, rectw) * 180.0f / (float)M_PI;
		if (alpha < 0) angle += 360.0f;
		float beta = angle - alpha;
		float w = cosf(angle * (float)M_PI / 180.0f) * rectw / 2.0f;
		float h = sinf(angle * (float)M_PI / 180.0f) * recth / 2.0f;
		float r3 = sqrtf(w * w + h * h);
		float r4 = cosf(beta * (float)M_PI / 180.0f) * r3;
		resultx = r4 * cosf(alpha * (float)M_PI / 180.0f) + pos->Get(0);
		resulty = r4 * sinf(alpha * (float)M_PI / 180.0f) + pos->Get(1);
	}

	bool OffsetModule::IntersectSegmentRectangle(float startX, float startY, float endX, float endY, float rectx,
	                                             float recty, float rectw, float recth, float& intersectionx, float& intersectiony)
	{
		float rectangleEndX = rectx + rectw;
		float rectangleEndY = recty + recth;

		/** Intersects the two line segments and returns the intersection point in intersection.
		 * @param p1 The first point of the first line segment
		 * @param p2 The second point of the first line segment
		 * @param p3 The first point of the second line segment
		 * @param p4 The second point of the second line segment
		 * @param intersection The intersection point. May be null.
		 * @return Whether the two line segments intersect */
		auto IntersectSegments = [](float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4,
			float& intersectionx, float& intersectiony) {

			float d = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
			if (d == 0) return false;

			float yd = y1 - y3;
			float xd = x1 - x3;
			float ua = ((x4 - x3) * yd - (y4 - y3) * xd) / d;
			if (ua < 0 || ua > 1) return false;

			float ub = ((x2 - x1) * yd - (y2 - y1) * xd) / d;
			if (ub < 0 || ub > 1) return false;

			intersectionx = x1 + (x2 - x1) * ua;
			intersectiony = y1 + (y2 - y1) * ua;

			return true;
		};
		
		if (IntersectSegments(startX, startY, endX, endY, rectx, recty, rectx, rectangleEndY, intersectionx, intersectiony))
			return true;

		if (IntersectSegments(startX, startY, endX, endY, rectx, recty, rectangleEndX, recty, intersectionx, intersectiony))
			return true;

		if (IntersectSegments(startX, startY, endX, endY, rectangleEndX, recty, rectangleEndX, rectangleEndY, intersectionx, intersectiony))
			return true;

		if (IntersectSegments(startX, startY, endX, endY, rectx, rectangleEndY, rectangleEndX, rectangleEndY, intersectionx, intersectiony))
			return true;

		return rectx <= startX && rectx + rectw >= startX && recty <= startY && recty + recth >= startY;
	}
	
}
