/* MiPAL 2010
Author: Tyrone Trevorrow, Carl Lusty, and Rene Hexel
Purpose: Provides a more generic mechanism for function callbacks.
		 Feel free to extend this to support any function's parameter lists.
 Change Log:
        21/1/2013 - Extended to add simple wb compatability, Carl.
*/
#ifndef WBFUNCTOR_H
#define WBFUNCTOR_H

#ifdef bool
#undef bool
#endif

#ifdef true
#undef true
#undef false
#endif

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#pragma clang diagnostic ignored "-Wpadded"

#include <string>

#include "WBMsg.h"
#include "guwhiteboardtypelist_generated.h" //for type enum
#include "gusimplewhiteboard.h" //for gu_simple_message

#define WB_BIND( f ) createWBFunctor(this, &f)
#define WB_TYPE_BIND( t, f ) createWBFunctor(this, &f, t)

class WBFunctorBase
{
public:
	virtual void call(std::string s, WBMsg* m) = 0;                         //old wbmsg format for callbacks
	virtual void call(gu_simple_message* m) = 0;                            //new simple_message callbacks
	virtual void call(guWhiteboard::WBTypes t, gu_simple_message* m) = 0;   //new simple_message callbacks (with type overwrite for subscribe to all special type)
        virtual guWhiteboard::WBTypes type() = 0;
        virtual uint16_t get_event_count() = 0;
        virtual void set_event_count(uint16_t e) = 0;
        virtual bool is_simple_wb_version() = 0;
	virtual ~WBFunctorBase(){}
};

template <typename C>
class WBFunctor: public WBFunctorBase
{
public:
	WBFunctor(C* obj, void (C::*pFunc) (std::string, WBMsg*)):
		fObject(obj), fFunction(pFunc), simple_wb_version(false) { }
	WBFunctor(C* obj, void (C::*pFunc) (guWhiteboard::WBTypes, gu_simple_message*), guWhiteboard::WBTypes t):
                fObject(obj), s_fFunction(pFunc), type_enum(t), event_count(0), simple_wb_version(true) { }

	void call(std::string s, WBMsg* m)
	{
		(fObject->*fFunction)(s,m);
	}
	void call(gu_simple_message* m)
	{
		(fObject->*s_fFunction)(type_enum, m);
	}
	void call(guWhiteboard::WBTypes t, gu_simple_message* m)
	{
		(fObject->*s_fFunction)(t, m);
	}
	guWhiteboard::WBTypes type() { return type_enum; }
	uint16_t get_event_count() { return event_count; }
	void set_event_count(uint16_t e) { event_count = e; }
	bool is_simple_wb_version() { return simple_wb_version; }
	typedef void (C::*s_func) (guWhiteboard::WBTypes, gu_simple_message*); //simple wb implementation
	s_func get_s_func_ptr() { return s_fFunction; }
protected:
	C* fObject;
	typedef void (C::*func) (std::string, WBMsg*);
	func fFunction;
	s_func s_fFunction;
	guWhiteboard::WBTypes type_enum;
	uint16_t event_count;
	bool simple_wb_version;
};

template <typename C>
WBFunctorBase* createWBFunctor(C *obj, void (C::*f) (std::string, WBMsg*))
{
	return new WBFunctor<C>(obj, f);
}
template <typename C>
WBFunctorBase* createWBFunctor(C *obj, void (C::*f) (guWhiteboard::WBTypes, gu_simple_message*), guWhiteboard::WBTypes t)
{
	return new WBFunctor<C>(obj, f, t);
}



/*template <typename T>
class WBFunctor_Say: public WBFunctor<T>
{
public:
        WBFunctor_Say(T* obj, void (T::*pFunc) (guWhiteboard::WBTypes, std::string &), guWhiteboard::WBTypes t):
                WBFunctor<T>(obj, pFunc, t) { };

        static WBFunctorBase *bind(T *obj, void (T::*f)(guWhiteboard::WBTypes, std::string &), guWhiteboard::WBTypes t) { return new WBFunctor_Say<T>(obj, T::func(f), t); }

        void call(gu_simple_message *m)
        {
                std::string result = guWhiteboard::kSay_t::get_from(m);
                (Say_function_t(*WBFunctor<T>::fObject->fFunction))(WBFunctor<T>::type_enum, &result);
        }

        typedef void (T::*Say_function_t) (guWhiteboard::WBTypes, std::string &);
};*/

#pragma clang diagnostic pop

#include "WBFunctor_types_generated.h"

#endif
