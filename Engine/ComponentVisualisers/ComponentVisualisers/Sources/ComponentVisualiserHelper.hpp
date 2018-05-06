#ifndef COMPONENT_VISUALISER_HELPER_HPP
#define COMPONENT_VISUALISER_HELPER_HPP
#pragma once

#include "Common.hpp"
#include "Interfaces/IViewer.hpp"


class ComponentVisualiserHelper
{
public:

	ComponentVisualiserHelper(IViewer* viewer)
		: viewer(viewer)
	{
		assert(viewer);
	}

	void DrawShape(FShape shape, FTransform transform, FColor color)
	{
		if (viewer)
		{
			viewer->DrawShape(shape, transform, color);
		}
	}

private:
	IViewer* viewer;
};




#endif // !COMPONENT_VISUALISER_HELPER_HPP
