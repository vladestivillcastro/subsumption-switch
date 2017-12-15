#include "stringConstants.h"

/* This cosntants label freze points of computation for which we identify a Kripke state as the state of computaiton at the frezze point*/

/* Breakpoint the happens after the OnExit of the previos state and Before
  the execution of any statement or the evalaution of any conditions (expression)
  in the current state */
const char  *pcBefore = "Before";

/* Breakpoint just before the state executes the OnEntry block of the state */
const char  *pcAfterOnEntry = "AfterOnEntry";

/* The following constants indicates a break point that happens when a FSM
   evaluates (atomicly) a condition (expression) labeling a transition.
   It would be completed with the ID of the transition evaluated and with
        the result (TRUe or FALSE)
 */
const char  *pcAfterEvaluate = "AfterEvaluate";
 
 /* The following frezze point can be arrived in two ways
  1) Directly from the corresponding
  AfterEvaluate to the next transition (in the transition order) because
  the previous transition evaluated to FALSE, and essentially there
  is no change, or
  2) from another "RINGLET" but the change was a change of external variables
  */
 const char  *pcBeforeEvaluate = "BeforeEvaluate";

/* the Following constants are just to complete the strings that label
 the pc of the Kripke structure
 */
const char  *pcBoolean = "B";
const char  *pcTrue = "True";
const char  *pcFalse = "False";
