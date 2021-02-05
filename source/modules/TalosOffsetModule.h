#pragma once

#include "modules/TalosAbstractModule.h"
#include "TalosTypes.h"
#include <vector>

namespace Talos
{

	class OffsetModule : public AbstractModule
	{
	public:
		enum T
		{
			ALPHA = 0,
			OUTPUT = 0,
		};
		enum Type
		{
            TYPE_SQUARE = 0,
            TYPE_ELLIPSE = 1,
            TYPE_LINE = 2,
		};
		enum Side
		{
            SIDE_ALL = 0,
            SIDE_TOP = 1,
            SIDE_BOTTOM = 2,
            SIDE_LEFT = 3,
            SIDE_RIGHT = 4,
		};

        ~OffsetModule();

        void Init() override;
		void Load(IFileProvider::ModuleData* moduleData) override;

        const std::vector<Point>& GetPoints() const;

        Point CreatePoint(float x, float y);
        void RemovePoint(int index);

        void SetLowPos(float posx, float posy);
        void GetLowPos(float& resultx, float& resulty) const;
        void SetHighPos(float posx, float posy);
        void GetHighPos(float& resultx, float& resulty) const;
        void SetLowSize(float sizex, float sizey);
        void GetLowSize(float& resultx, float& resulty) const;
        void SetHighSize(float size, float sizey);
        void GetHighSize(float& resultx, float& resulty) const;
        void SetLowShape(int shape);
        int GetLowShape() const;
        void SetHighShape(int shape);
        int GetHighShape() const;
        void SetLowEdge(bool edge);
        bool GetLowEdge() const;
        void SetHighEdge(bool edge);
        bool GetHighEdge() const;
        void SetLowSide(int side);
        int GetLowSide() const;
        void SetHighSide(int side);
        int GetHighSide() const;

	protected:
        NumericalValue* _alpha = nullptr;
        NumericalValue* _output = nullptr;

        NumericalValue* _lowPos = nullptr;
        NumericalValue* _lowSize = nullptr;
        NumericalValue* _highPos = nullptr;
        NumericalValue* _highSize = nullptr;

        int _lowShape = 0;
        int _highShape = 0;

        bool _lowEdge = true;
        bool _highEdge = true;

        float _tolerance = 0;

        int _lowSide = SIDE_BOTTOM;
        int _highSide = SIDE_RIGHT;

        std::vector<Point> _points;

        RandomXS128 _random;

        void ResetPoints();

        void DefineSlots() override;
        void ProcessValues() override;

        void ProcessAlphaDefaults();

        float Interpolate(float alpha);
        float Interpolate(float alpha, float x1, float y1, float x2, float y2);

        void GetRandomPosOn(int side, bool edge, int shape, NumericalValue* pos, NumericalValue* size, float& resultx, float& resulty);
        void FindRandomSquarePos(float angle, NumericalValue* pos, NumericalValue* size, float& resultx, float& resulty);
        void FindRandomEllipsePos(float angle, NumericalValue* pos, NumericalValue* size, float& resultx, float& resulty);
        void FindRandomLinePos(float angle, NumericalValue* pos, NumericalValue* size, float& resultx, float& resulty);
		
        static bool IntersectSegmentRectangle(float startX, float startY, float endX, float endY,
            float rectx, float recty, float rectw, float recth, float& intersectionx, float& intersectiony);
	};

}
