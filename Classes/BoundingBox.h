#pragma once
class BoundingBox
{
private:
	// Mininums taken as negative from (0, 0)
	float _maxX, _minX, _maxY, _minY;
	// These are multipliers, take MaxX for example: 1.0 = maxX of bounding box, 0.0 = centre of bounding box, -1.0 = minX of bounding box
	float _scalerMaxX, _scalerMinX, _scalerMaxY, _scalerMinY;
public:
	BoundingBox();
	~BoundingBox();

	void Update(float delta, float scaleWidth, float scaleHeight, float dispX, float dispY, float prevDispX, float prevDispY);

	void SetAdjustmentScaler(float maxX, float minX, float maxY, float minY);

	float GetMaxX() { return _maxX; };
	float GetMinX() { return _minX; };
	float GetMaxY() { return _maxY; };
	float GetMinY() { return _minY; };
};

