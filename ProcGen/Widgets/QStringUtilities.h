#pragma once

#include <qstring.h>

namespace QStringUtilities
{
	inline QString& ToFileQString(QString& text)
	{
		return text.replace(" ", "_");
	}

	inline QString& FromFileQString(QString& text)
	{
		return text.replace("_", " ");
	}
}

