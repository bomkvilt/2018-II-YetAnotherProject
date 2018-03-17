#ifndef CONTROL_TYPES_HPP
#define CONTROL_TYPES_HPP

#include <string>
#include <vector>
#include <functional>


/********************************************************************
*							Enums
********************************************************************/


enum class EKeyModifier
{
	eNone,
	eCtrl,
	eShift,
	eAlt
};

enum EInputKey
{
	KEY_NONE		= 0,
	//
	KEY_A			= 97,
	KEY_B			= 98,
	KEY_C			= 99,
	KEY_D			= 100,
	KEY_E			= 101,
	//
	KEY_F			= 102,
	KEY_G			= 103,
	KEY_H			= 104,
	KEY_I			= 105,
	KEY_J			= 106,
	//
	KEY_K			= 107,
	KEY_L			= 108,
	KEY_M			= 109,
	KEY_N			= 110,
	KEY_O			= 111,
	//
	KEY_P			= 112,
	KEY_Q			= 113,
	KEY_R			= 114,
	KEY_S			= 115,
	KEY_T			= 116,
	//
	KEY_U			= 117,
	KEY_V			= 118,
	KEY_W			= 119,
	KEY_X			= 120,
	KEY_Y			= 121,
	//
	KEY_Z			= 122,
	//
	KEY_MOUSE_L		= 300,
	KEY_MOUSE_R		= 301,
	//
	KEY_TAB			= 5000,
	KEY_ENTER		= 5001,
	KEY_SHIFT		= 5002,
	KEY_ESCAPE		= 5003,
	KEY_CAPS		= 5004
	//
};

enum EKeyAction
{
	BA_None,
	BA_Pressed,
	BA_Released,
	BA_DoubleClick
};

/********************************************************************
*							Structs
********************************************************************/


/** universal key representation
 */
struct ControlKey
{
	EKeyModifier modifier;
	EKeyAction	 action;
	EInputKey	 key;

public:

	ControlKey(
		EInputKey	 key	  = EInputKey::KEY_NONE,
		EKeyModifier modifier = EKeyModifier::eNone,
		EKeyAction	 action	  = EKeyAction::BA_None
	)	
		: modifier	(modifier)
		, action	(action)
		, key		(key)
	{}

	bool operator==(const ControlKey& r) const
	{
		return key		== r.key
			&& modifier	== r.modifier
			&& action   == r.action;
	}

	operator bool() const
	{
		return key != EInputKey::KEY_NONE;
	}

	ControlKey getKey() const //TODO: rename
	{
		auto tmp = *this;

		tmp.action = EKeyAction::BA_None;
		
		return tmp;
	}
};

template<>
struct std::hash<ControlKey>
{
	size_t operator()(const ControlKey& key) const //TODO: WIN32
	{
		return ((size_t)key.key		<< 0 )
			|  ((size_t)key.modifier<< 32)
			|  ((size_t)key.action	<< 48);
	}
};


/**
 */
struct ControlAxisPair
{
	ControlKey key;
	float	 value;

public:

	ControlAxisPair(
		EInputKey button	  = EInputKey::KEY_NONE, 
		float value			  = 0, 
		EKeyModifier modifier = EKeyModifier::eNone
	)
		: key(button, modifier)
		, value(value)
	{}
};


/** Game control axis (setup in controller)
 */
struct ControlAxis
{
public:

	std::string name;

	std::vector<ControlAxisPair> axis;

public:

	ControlAxis() = default;

	ControlAxis(std::string&& name, std::vector<ControlAxisPair>&& axis)
		: name(std::move(name))
		, axis(std::move(axis))
	{}
};


/** Game control actions (setup in controller)
 */
struct ControlAction
{
	std::string name;
	ControlKey  key;

public:

	ControlAction() = default;

	ControlAction(std::string&& name, ControlKey key)
		: name(std::move(name))
		, key(key)
	{}
};


/** Binded axis
 */
struct BindedAxis
{
	using Callback = std::function<void(float)>;

public:

	std::string name;
	Callback callback;

public:

	BindedAxis(const std::string& name, Callback callback)
		: name(name)
		, callback(callback)
	{}
};


/** Binded actions
 */
struct BindedActions
{
	using Callback = std::function<void(EKeyAction)>;

public:

	std::string name;
	EKeyAction  type;
	Callback calback;

public:

	BindedActions(const std::string& name, EKeyAction type, Callback calback)
		: name(name)
		, type(type)
		, calback(calback)
	{}
};


/** Event from a keyboard 
*/
struct KeyEvent
{
	int			 rawKey		= 0;
	EInputKey	 key		= EInputKey::KEY_NONE;
	EKeyModifier modifier	= EKeyModifier::eNone;
	EKeyAction	 action		= EKeyAction::BA_None;

public:

	bool operator==(const KeyEvent& r) const
	{
		return rawKey	== r.rawKey
			&& key		== r.key
			&& modifier == r.modifier
			&& action	== r.action;
	}

	ControlKey GetControlKey() const
	{
		return ControlKey(key, modifier, action);
	}
};


#endif // !CONTROL_TYPES_HPP