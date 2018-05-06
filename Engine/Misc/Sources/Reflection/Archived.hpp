#pragma once
#ifndef ARCHIVABLE_HPP
#define ARCHIVABLE_HPP

#include <string>
#include <sstream>

#include "Archiver.hpp"


/** A base type for all classes / structs in engine
 */
struct Archived
{
public: //~~~~~~~~~~~~~~| Generated body

	/** @return a class name
	 *  NOTE: use a GENRATED_BODY() macro */
	virtual std::string __GetClassName() const = 0;

public:
	/** Archive / Dearchive the class' mebers
	 * @param ar       - archve 
	 * @param bForward - if true - archive else - disarchive */
	virtual void __Archive(Archiver& ar, bool bForvward) {}
};


/******************************************************************************
*									Type2String
******************************************************************************/

template<typename _T, bool IsArchibale, bool IsPointer>
struct Type2Srting
{
	std::string operator()(_T& t)
	{
		static_assert(false);
	}
};


template<typename _T>
struct Type2Srting<_T, false, false>
{
	std::string operator()(_T& t)
	{
		std::string s;
		std::stringstream ss;
		ss << t;
		return ss.str();
	}
};

template<typename _T>
struct Type2Srting<_T, true, false>
{
	std::string operator()(_T& t)
	{
		Archiver ar;
		t.__Archive(ar, true);
		return ar.Archive();
	}
};

template<typename _T>
struct Type2Srting<_T, true, true>
{
	std::string operator()(_T* t)
	{
		if (t)
		{
			Archiver ar;
			t->__Archive(ar, true);
			return ar.Archive();
		}
		return std::string();
	}
};

/******************************************************************************
*									String2Type
******************************************************************************/

template<typename _T, bool IsArchibale, bool IsPointer>
struct String2Type
{
	void operator()(_T& t, const std::string& str)
	{
		static_assert(false);
	}
};

template<typename _T>
struct String2Type<_T, false, false>
{
	void operator()(_T& t, const std::string& str)
	{
		std::stringstream ss(str);
		ss >> t;
	}
};

template<typename _T>
struct String2Type<_T, true, false>
{
	void operator()(_T& t, const std::string& str)
	{
		Archiver ar;
		ar.Constract(str);
		t.__Archive(ar, false);
	}
};

template<typename _T>
struct String2Type<_T, true, true>
{
	void operator()(_T* t, const std::string& str)
	{
		if (t)
		{
			Archiver ar;
			ar.Constract(str);
			t->__Archive(ar, false);
		}
		return std::string();
	}
};

/******************************************************************************
*									Utiles
******************************************************************************/

#define SMTH2SMTH_ARGS(VAR)  decltype(VAR)              \
	, std::is_base_of<Archived,decltype(VAR)>().value \
	, std::is_pointer<decltype(VAR)>().value


/******************************************************************************
*									To Use
******************************************************************************/

#define GENERATED_BODY(CLASS, SUPER)                        \
	public:                                                 \
		virtual std::string __GetClassName() const override \
		{                                                   \
			return std::string(#CLASS);                     \
		}                                                   \
		using Super = SUPER;


#define ARCHIVATION_BEGIN() \
	public: \
	virtual void __Archive(Archiver& ar, bool bForvward) override \
	{ \
		Super::__Archive(ar, bForvward);

#define ARCHIVE_MEMBER(CATEGORY, VAR) \
	if (bForvward) \
	{ \
		ar[CATEGORY][#VAR] = Type2Srting<SMTH2SMTH_ARGS(VAR)>()(VAR); \
	} \
	else \
	{ \
		String2Type<SMTH2SMTH_ARGS(VAR)>()(VAR, ar[CATEGORY][#VAR]); \
	}

#define ARCHIVATION_END() \
	}



#define GENERATED_ARCHIVATION_BODY(CLASS, SUPER) \
	GENERATED_BODY(CLASS, SUPER)                 \
	ARCHIVATION_BEGIN()


#endif // !ARCHIVABLE_HPP
