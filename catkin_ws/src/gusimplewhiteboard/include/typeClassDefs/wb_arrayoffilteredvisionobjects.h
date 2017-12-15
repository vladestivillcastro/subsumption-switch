//
//  wb_arrayoffilteredvisionobjects.h
//  gusimplewhiteboard
//
//  Created by Vlad Estivill-Castro on 18/06/2014.
//  Copyright (c) 2014 ICT. All rights reserved.
//

#ifndef gusimplewhiteboard_wb_arrayoffilteredvisionobjects_h
#define gusimplewhiteboard_wb_arrayoffilteredvisionobjects_h

#include "wb_filteredvisionobject.h"


/**
 * Discriminate sigthings
 */
enum FilteredVisionObjectType
{
        FVOGoalPostTop,          ///< Filtered informaiton for a post we cannot tell is Left or right
        FVOGoalPostLeftTop,          ///< Filtered informaiton for a post we know is Left
        FVOGoalPostRightTop,          ///< Filtered informaiton for a post we know is right
        FVOGoalCrossBarTop,          ///< Filtered informaiton for the Ball
    //FVOGoalPostBottom,          ///< Filtered informaiton for a post we cannot tell is Left or right
    //FVOGoalPostLeftBottom,          ///< Filtered informaiton for a post we know is Left
    //FVOGoalPostRightBottom,          ///< Filtered informaiton for a post we know is right
    //FVOGoalCrossBarBottom,          ///< Filtered informaiton for the Ball
        FVO_NUM_OBJECTS          ///< number of different kind of objects
};

struct wb_arrayoffilteredvisionobjects
{
    
    ARRAY_PROPERTY(struct wb_filteredvisionobject, objects, FVO_NUM_OBJECTS)
     
};


#endif
