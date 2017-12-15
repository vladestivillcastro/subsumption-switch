/*
 *  gutcpinjectionwhiteboardobject.h
 *
 *  Created by Carl Lusty in 2013.
 *  Copyright (c) 2013 Carl Lusty
 *  All rights reserved.
 */

#ifndef TCP_INJECTION_WB_OBJ_H
#define TCP_INJECTION_WB_OBJ_H

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#pragma clang diagnostic ignored "-Wpadded"

#include "gusimplewhiteboard.h"
#include "guudpwhiteboard/tcp_config.h"

#include <iostream> //cerr
#include <cstdio> //stderr etc..
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <string>
#include <vector>
#include <cstring> //memset

#ifdef bool
#undef bool
#endif

#ifdef true
#undef true
#undef false
#endif

#include <dispatch/dispatch.h>

#define DISPATCH_TCP_QUEUE "guWhiteboard_tcp"

#ifndef DISPATCH_QUEUE_SERIAL
#define DISPATCH_QUEUE_SERIAL NULL
#endif


extern const char *robot_network_names[];
void transmit(void *);
struct transmit_info
{
        int sfd;
        gsw_message_packet p;
};

template <class object_type> class injection_whiteboard_object
{
        dispatch_queue_t                        send_queue;

        /* http://linux.die.net/man/3/getaddrinfo */
        struct addrinfo hints;
        struct addrinfo *result, *rp;
        int sfd, s;

        uint16_t type_offset;
        bool async;
        std::string remote_hostname;
        
public:
        /**
         * designated constructor
         */
        injection_whiteboard_object(const char *hostname, uint16_t type, bool is_async = true) //Constructor
        {
                init(hostname, type, is_async);
        }

        /**
         * copy constructor
         */
        injection_whiteboard_object(const injection_whiteboard_object<object_type> &source)
        {
                init(source.remote_hostname.c_str(), source.type_offset, source.async);
        }

        /**
         * value conversion reference constructor (needs to be overridden by subclasses to set toffs to be useful)
         */
        injection_whiteboard_object(const object_type &value, const char *hostname, uint16_t type, bool is_async = true)
        {
                init(hostname, type, is_async);
                send_message(value);
        }

        /**
         * intialiser (called from constructors)
         */
        void init(const char *hostname, uint16_t type, bool is_async = true)
        {
                type_offset = type;
                async = is_async;
                remote_hostname = std::string (hostname); //just here so the copy constructor works

                if (!(send_queue = dispatch_queue_create(DISPATCH_TCP_QUEUE, DISPATCH_QUEUE_SERIAL)))
                {
                        std::cerr << "Unable to create dispatch queue" << std::endl;
                        throw "TCP Whiteboard Injector cannot create dispatch queue";
                }

                /* Obtain address(es) matching host/port */
                memset(&hints, 0, sizeof(struct addrinfo));
                hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
                hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
                hints.ai_flags = 0;
                hints.ai_protocol = 0;          /* Any protocol */

                s = getaddrinfo(hostname, TCP_PORT_STR, &hints, &result);
                if (s != 0) {
                        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
                        throw "TCP Whiteboard Injector couldn't find service";
                }

                /* getaddrinfo() returns a list of address structures.
                 Try each address until we successfully connect(2).
                 If socket(2) (or connect(2)) fails, we (close the socket
                 and) try the next address. */

                for (rp = result; rp != NULL; rp = rp->ai_next) {
                        sfd = socket(rp->ai_family, rp->ai_socktype,
                                     rp->ai_protocol);
                        if (sfd == -1)
                                continue;

                        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
                                break;                  /* Success */

                        close(sfd);
                }

                if (rp == NULL) {               /* No address succeeded */
                        fprintf(stderr, "Could not connect\n");
                        throw "TCP Whiteboard Injector can't connect to service";
                }
                
                freeaddrinfo(result);           /* No longer needed */
        }

        /**
         * inject message over the network connection
         */
        bool send_message(const object_type &msg);

private:
        bool send_tcp(gsw_message_packet p)
        {
                transmit_info *info = new transmit_info();
                info->sfd = sfd;
                info->p = p;

                if(async)
                        dispatch_async_f(send_queue, static_cast<void *>(info), transmit);
                else
                {
                        transmit(static_cast<void *>(info));
                }
                return true;
        }
};

template<> bool injection_whiteboard_object<std::string>::send_message(const std::string &msg);
template<> bool injection_whiteboard_object<std::vector<int> >::send_message(const std::vector<int> &msg);

template <class object_type>
bool injection_whiteboard_object<object_type>::send_message(const object_type &msg)
{
        gsw_message_packet p;

        gu_simple_message *m = &p.m;
        object_type *wbobj = reinterpret_cast<object_type*>(m);
        *wbobj = msg;

        p.t = type_offset;

        return send_tcp(p);
}

#endif //TCP_INJECTION_WB_OBJ_H


