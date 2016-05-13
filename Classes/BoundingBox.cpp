#include "BoundingBox.h"


BoundingBox::BoundingBox()
{
	_maxX = 0.0f;
	_minX = 0.0f;
	_maxY = 0.0f;
	_minY = 0.0f;
	_scalerMaxX = 1.0f;
	_scalerMinX = 1.0f;
	_scalerMaxY = 1.0f;
	_scalerMinY = 1.0f;
}


BoundingBox::~BoundingBox()
{
}

void BoundingBox::Update(float delta, float scaleWidth, float scaleHeight, float dispX, float dispY, float prevDispX, float prevDispY)
{
	float minX = 0.0f;
	float maxX = 0.0f;
	float minY = 0.0f;
	float maxY = 0.0f;

	// Check if the bounding box needs to be smeared
	if (dispX != prevDispX || dispY != prevDispY)
	{
		// Calculate midpoint between previous and current position
		float midX = (dispX + prevDispX) / 2;
		float midY = (dispY + prevDispY) / 2;

		// Find min and max x axis bounds
		if (dispX < prevDispX)
		{
			minX = (dispX - midX) - (scaleWidth / 2);
			maxX = (prevDispX - midX) + (scaleWidth / 2);
		}
		else if (dispX > prevDispX)
		{
			minX = (prevDispX - midX) - (scaleWidth / 2);
			maxX = (dispX - midX) + (scaleWidth / 2);
		}
		else
		{
			minX = -1 * (scaleWidth / 2);
			maxX = (scaleWidth / 2);
		}

		// Find min and max y axis bounds
		if (dispY < prevDispY)
		{
			minY = (dispY - midY) - (scaleHeight / 2);
			maxY = (prevDispY - midY) + (scaleHeight / 2);
		}
		else if (dispY > prevDispY)
		{
			minY = (prevDispY - midY) - (scaleHeight / 2);
			maxY = (dispY - midY) + (scaleHeight / 2);
		}
		else
		{
			minY = -1 * (scaleHeight / 2);
			maxY = (scaleHeight / 2);
		}
	}
	else
	{
		minX = -1 * (scaleWidth / 2);
		maxX = (scaleWidth / 2);
		minY = -1 * (scaleHeight / 2);
		maxY = (scaleHeight / 2);
	}

	// Apply the bounding box adjustments and commit
	_maxX = maxX * _scalerMaxX;
	_minX = minX * _scalerMinX;
	_maxY = maxY * _scalerMaxY;
	_minY = minY * _scalerMinY;
}

void BoundingBox::SetAdjustmentScaler(float maxX, float minX, float maxY, float minY)
{
	// Clip values
	if (maxX > 1.0f) maxX = 1.0f; else if (maxX < -1.0f) maxX = -1.0f;
	if (minX > 1.0f) minX = 1.0f; else if (minX < -1.0f) minX = -1.0f;
	if (maxY > 1.0f) maxY = 1.0f; else if (maxY < -1.0f) maxY = -1.0f;
	if (minY > 1.0f) minY = 1.0f; else if (minY < -1.0f) minY = -1.0f;

	_scalerMaxX = maxX;
	_scalerMinX = minX;
	_scalerMaxY = maxY;
	_scalerMinY = minY;
}