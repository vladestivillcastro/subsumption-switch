/*
 *  gugenericwhiteboardobject.h
 *
 *  Created by Carl Lusty in 2013.
 *  Copyright (c) 2013 Carl Lusty
 *  All rights reserved.
 */

#ifndef GENERIC_WB_OBJ_H
#define GENERIC_WB_OBJ_H


#include "gusimplewhiteboard.h"
#include <iostream>
#include <assert.h>
#include <string>
#include <vector>

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
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#pragma clang diagnostic ignored "-Wdeprecated"

extern gu_simple_whiteboard_descriptor *local_whiteboard_descriptor;

template <class object_type> class generic_whiteboard_object
{
        gu_simple_whiteboard_descriptor *_wbd;
        uint16_t type_offset;
        bool atomic;
        bool notify_subscribers;

public:
        /**
         * designated constructor
         */
        generic_whiteboard_object(gu_simple_whiteboard_descriptor *wbd, uint16_t toffs, bool want_atomic = true, bool do_notify_subscribers = true) //Constructor
        {
                init(toffs, wbd, want_atomic, do_notify_subscribers);
        }

        /**
         * copy constructor
         */
        generic_whiteboard_object(const generic_whiteboard_object<object_type> &source)
        {
                init(source.type_offset, source._wbd, source.atomic, source.notify_subscribers);
        }

        /**
         * value conversion reference constructor (needs to be overridden by subclasses to set toffs to be useful)
         */
        generic_whiteboard_object(const object_type &value, uint16_t toffs, gu_simple_whiteboard_descriptor *wbd = NULL, bool want_atomic = true)
        {
                init(toffs, wbd, want_atomic);
                set(value);
        }

        /**
         * intialiser (called from constructors)
         */
        void init(uint16_t toffs, gu_simple_whiteboard_descriptor *wbd = NULL, bool want_atomic = true, bool do_notify_subscribers = true)
        {
                if(!wbd)
                {
			wbd = get_local_singleton_whiteboard();
                }
                type_offset = toffs;
                atomic = want_atomic;
                notify_subscribers = do_notify_subscribers;
                _wbd = wbd;
        }

        /**
         * designated setter for posting whiteboard messages
         */
        void set(const object_type &msg);

        /**
         * designated getter for getting a whiteboard message
         */
        object_type get()
        {
//              return *(object_type *)gsw_current_message(_wbd->wb, type_offset);
                return get_from(gsw_current_message(_wbd->wb, type_offset));
        }

        /**
         * access method to get data from an existing, low-level message
         */
        object_type get_from(gu_simple_message *msg);

        /**
         * post method (calls set())
         */
        void post(const object_type &msg) { set(msg); }

        /**
         * shift left operator (calls set())
         */
        const object_type &operator<<(const object_type &value)
        {
                set(value);

                return value;
        }

        /**
         * shift right operator (calls get())
         */
        generic_whiteboard_object<object_type> &operator>>(object_type &value)
        {
                value = get();

                return *this;
        }

        /**
         * assignment operator (calls set())
         */
        const object_type &operator=(const object_type &value)
        {
                set(value);

                return value;
        }

        /**
         * assignment copy operator (calls set())
         */
        object_type operator=(object_type value)
        {
                set(value);

                return value;
        }

        /**
         * cast operator (calls get())
         */
        operator object_type()
        {
                return get();
        }

        /**
         * empty function operator (calls get())
         */
        object_type operator()()
        {
                return get();
        }

        /**
         * function operator with object_type copy parameter (calls set())
         */
        void operator()(object_type value)
        {
                set(value);
        }
};

template<> void generic_whiteboard_object<std::string>::set(const std::string &msg);
template<> void generic_whiteboard_object<std::vector<int> >::set(const std::vector<int> &msg);
template<> std::string generic_whiteboard_object<std::string>::get_from(gu_simple_message *msg);
template<> std::vector<int> generic_whiteboard_object<std::vector<int> >::get_from(gu_simple_message *msg);


template <typename object_type>
object_type generic_whiteboard_object<object_type>::get_from(gu_simple_message *msg)
{
        return *reinterpret_cast<object_type *>(msg);
}

template <class object_type>
void generic_whiteboard_object<object_type>::set(const object_type &msg)
{
        int t = type_offset;
        
#ifndef NO_SAFETY
        assert(GU_SIMPLE_WHITEBOARD_BUFSIZE >= sizeof(object_type));
#endif
        if (atomic) gsw_procure(_wbd->sem, GSW_SEM_PUTMSG);
        
        gu_simple_whiteboard *wb = _wbd->wb;
        gu_simple_message *m = gsw_next_message(wb, t);
        object_type *wbobj = reinterpret_cast<object_type*>(m);
        *wbobj = msg;
        
        gsw_increment(wb, t);
        gsw_increment_event_counter(wb, t);
        if (atomic) gsw_vacate(_wbd->sem, GSW_SEM_PUTMSG);
        if (notify_subscribers && wb->subscribed) gsw_signal_subscribers(wb);
}

#pragma clang diagnostic pop


#endif //GENERIC_WB_OBJ_H
