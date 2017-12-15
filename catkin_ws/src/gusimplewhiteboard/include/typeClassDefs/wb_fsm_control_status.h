/*
 *  wb_fsm_control_status.h
 *  gusimplewhiteboard / clfsm
 *
 *  Created by Rene Hexel on 18/06/2014.
 *  Copyright (c) 2013, 2014 Rene Hexel. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgement:
 *
 *        This product includes software developed by Rene Hexel.
 *
 * 4. Neither the name of the author nor the names of contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * -----------------------------------------------------------------------
 * This program is free software; you can redistribute it and/or
 * modify it under the above terms or under the terms of the GNU
 * General Public License as published by the Free Software Foundation;
 * either version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see http://www.gnu.org/licenses/
 * or write to the Free Software Foundation, Inc., 51 Franklin Street,
 * Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */
#ifndef gusimplewhiteboard_wb_fsm_control_status_h
#define gusimplewhiteboard_wb_fsm_control_status_h

#include <gu_util.h>
#include "gusimplewhiteboard.h"

#define CONTROLSTATUS_BYTE_SIZE     (sizeof(gu_simple_message))
#define CONTROLSTATUS_BIT_SIZE      (CONTROLSTATUS_BYTE_SIZE*8)
#define CONTROLSTATUS_BITS_RESERVED 2   // must be big enough for enum
#define CONTROLSTATUS_NUM_FSMS      (CONTROLSTATUS_BIT_SIZE - CONTROLSTATUS_BITS_RESERVED)
#define CONTROLSTATUS_CMD           (CONTROLSTATUS_BYTE_SIZE-1)
#define CONTROLSTATUS_CMD_LO        (1<<6)
#define CONTROLSTATUS_CMD_HI        (1<<7)
#define CONTROLSTATUS_CMD_MASK      (CONTROLSTATUS_CMD_LO|CONTROLSTATUS_CMD_HI)

#define CONTROLSTATUS_CLR_FSM(s, fsm)   ((s)->_fsms[(fsm)%8] &=~(1<<((fsm)%8)))
#define CONTROLSTATUS_SET_FSM(s, fsm)   ((s)->_fsms[(fsm)%8] |= (1<<((fsm)%8)))
#define CONTROLSTATUS_GET_FSM(s, fsm)   ((((s)->_fsms[(fsm)%8]) & (1<<((fsm)%8))) != 0)

#define CONTROLSTATUS_SET_CMD(s, cmd)   ((s)->_fsms[CONTROLSTATUS_CMD] = static_cast<uint8_t>(((s)->_fsms[CONTROLSTATUS_CMD] & ~CONTROLSTATUS_CMD_MASK) | (((cmd) & 3) << 6)))
#define CONTROLSTATUS_GET_CMD(s)    ((((s)->_fsms[CONTROLSTATUS_CMD]) >> 6) & 3)

#ifdef __cplusplus
namespace guWhiteboard
{
#endif
    /**
     * command for ControlStatus
     */
    enum FSMControlType
    {
        FSMStatus,          ///< check status only
        FSMSuspend,         ///< suspend the corresponding state machines
        FSMResume,          ///< resume the corresponding state machines
        FSMRestart          ///< restart the corresponding state machine
    };
#ifdef __cplusplus
}
#endif

struct wb_fsm_control_status
{
    ARRAY_PROPERTY(uint8_t, fsms, CONTROLSTATUS_BYTE_SIZE)

#ifdef __cplusplus
    wb_fsm_control_status(guWhiteboard::FSMControlType t = guWhiteboard::FSMStatus) { memset(this, 0, sizeof(*this)); CONTROLSTATUS_SET_CMD(this, t); }
#endif
};

#endif
