#ifndef FIGUREACTION_PRIVATE_H
#define FIGUREACTION_PRIVATE_H

#include "figure/image.h"
#include "figure/formation_layout.h"

#define FigureActionIncreaseGraphicOffset(f, max) (f)->graphicOffset++; if ((f)->graphicOffset >= (max)) (f)->graphicOffset = 0;
#define FigureActionDirection(f) ((f->direction < Data_State.map.orientation ? 8 : 0) + f->direction - Data_State.map.orientation)
#define FigureActionNormalizeDirection(d) ((d) = (8 + (d) - Data_State.map.orientation) % 8)
#define FigureActionCorpseGraphicOffset(f) figure_image_corpse_offset(f)
#define FigureActionMissileLauncherGraphicOffset(f) figureActionMissileLauncherGraphicOffsets[f->attackGraphicOffset / 2]
#define FigureActionFormationLayoutPositionX(layout, index) formation_layout_position_x(layout, index)
#define FigureActionFormationLayoutPositionY(layout, index) formation_layout_position_y(layout, index)

#define FigureActionUpdateGraphic(f,g) figure_image_update(f, g)

extern const int figureActionMissileLauncherGraphicOffsets[128];

#include "FigureAction.h"
#include "FigureMovement.h"

#include "Data/State.h"

#include "graphics/image.h"

#endif
