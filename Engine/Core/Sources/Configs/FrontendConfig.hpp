#pragma once
#ifndef FRONTEND_CONFIG_HPP
#define FRONTEND_CONFIG_HPP

#include "Reflection/Archived.hpp"
#include "Common.hpp"


struct FFrontendConfig : public Archived
{
	GENERATED_CONFIG_BODY(FFrontendConfig, Archived)
		ARCHIVE_MEMBER("", title         )
		ARCHIVE_MEMBER("", ScreenSize    )
		ARCHIVE_MEMBER("", WindowPosition)
		ARCHIVATION_END()
public:
	std::string title       = "Default Name";
	FVector2 ScreenSize     = FVector2(1200, 800);
	FVector2 WindowPosition = FVector2(700 , 200);
};



#endif // !FRONTEND_CONFIG_HPP
