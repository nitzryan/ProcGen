#pragma once
#include "IFilterMap.h"

class MountainFilterMap : public IFilterMap
{
public:
	MountainFilterMap();
	~MountainFilterMap();
	void SetNumRanges(int min, int max);
	void SetRangeLength(float min, float max);
	void SetRangeWidth(float min, float max);
	void SetLengthPerBend(float len);
	void SetBendAmount(float min, float max);
protected:
	void GenMap() override;
private:
	int minRanges, maxRanges;
	float lengthMin, lengthMax;
	float widthMin, widthMax;
	float lengthPerBend;
	float bendAmountMin, bendAmountMax;
};

