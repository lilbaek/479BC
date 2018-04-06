#ifndef DATA_STATE_H
#define DATA_STATE_H

#include "core/time.h"

extern struct _Data_State {
	struct {
		int gridOffsetStart;
		int gridOffsetEnd;
		int roadRequired;
		time_millis roadLastUpdate;
		int drawAsConstructing;
		int reservoirOffsetX;
		int reservoirOffsetY;
	} selectedBuilding;
	int missionBriefingShown;
    struct {
        struct {
            int gridOffset;
            int x;
            int y;
        } current;
    } map;
} Data_State;

#endif
