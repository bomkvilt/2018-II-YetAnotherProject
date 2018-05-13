#pragma once
#ifndef IVIEWER_HPP
#define IVIEWER_HPP

#include "Common.hpp"
#include "Configs/EngineConfig.hpp"


struct IViewer
{
	virtual ~IViewer() = default;

public:
	virtual void Render() = 0;
	virtual void DrawShape(FShape shape, FTransform transform, FColor color) = 0;
};


#endif // !IVIEWER_HPP
