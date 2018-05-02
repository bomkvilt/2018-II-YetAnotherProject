#pragma once
#ifndef IMODULE_HPP
#define IMODULE_HPP


class IModule
{
public:
	virtual void OnLoaded() {}
	virtual void OnUnload() {}
};



#endif //!IMODULE_HPP
