#pragma once
#include "IFilterMap.h"

struct MountainFilterMapArgs
{
	int minRanges = -1, maxRanges = -1;
	float minLength = -1, maxLength = -1;
	float minWidth = -1, maxWidth = -1;
	float lengthPerBend = -1;
	float minBend = -1, maxBend = -1;
	float minScale = -1, maxScale = -1; // Range for max height of range
	float minRangeMult = -1, maxRangeMult = -1; // Values used to control size of range that is at max-height

	bool IsValid() const {
		return minRanges > 0 && maxRanges >= minRanges &&
			minLength > 0 && maxLength >= minLength &&
			minWidth > 0 && maxWidth >= minWidth &&
			lengthPerBend > 0 &&
			minBend >= 0 && maxBend >= minBend &&
			minScale > 0 && maxScale >= minScale &&
			minRangeMult > 0 && maxRangeMult >= minRangeMult;
	}
};

class MountainFilterMap : public IFilterMap
{
public:
	MountainFilterMap();
	~MountainFilterMap();
	void SetArgs(MountainFilterMapArgs& args);
protected:
	void GenMap() override;
private:
	MountainFilterMapArgs args;
};

