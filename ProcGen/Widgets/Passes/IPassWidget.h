#pragma once

namespace IPassWidgetNS
{
	enum class PASS_TYPE {
		PERLIN = 1,
		OFFSET = 2
	};
}

class IPassWidget : public QWidget
{
public:
	explicit IPassWidget() : QWidget(nullptr) {}
	virtual ~IPassWidget() {}

	virtual void WriteToFile(std::ofstream& file) const = 0;
	virtual void GetPassOutput(int width, int height, float* data) = 0;
};