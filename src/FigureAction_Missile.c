#include "FigureAction_private.h"

#include "Figure.h"
#include "Formation.h"

#include "figure/formation.h"
#include "figure/properties.h"
#include "figure/sound.h"
#include "sound/effect.h"

static const int cloudGraphicOffsets[] = {
	0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2,
	2, 2, 2, 2, 3, 3, 3, 4, 4, 5, 6, 7
};

void FigureAction_explosionCloud(figure *f)
{
	f->useCrossCountry = 1;
	f->progressOnTile++;
	if (f->progressOnTile > 44) {
		f->state = FigureState_Dead;
	}
	FigureMovement_crossCountryWalkTicks(f, f->speedMultiplier);
	if (f->progressOnTile < 48) {
		f->graphicId = image_group(GROUP_FIGURE_EXPLOSION) +
			cloudGraphicOffsets[f->progressOnTile / 2];
	} else {
		f->graphicId = image_group(GROUP_FIGURE_EXPLOSION) + 7;
	}
}

void FigureAction_arrow(figure *f)
{
	f->useCrossCountry = 1;
	f->progressOnTile++;
	if (f->progressOnTile > 120) {
		f->state = FigureState_Dead;
	}
	int shouldDie = FigureMovement_crossCountryWalkTicks(f, 4);
	int targetId = Figure_getCitizenOnSameTile(f->id);
	if (targetId) {
        figure *target = figure_get(targetId);
		int targetType = target->type;
		int formationId = target->formationId;
        const figure_properties *target_props = figure_properties_for_type(targetType);
        int maxDamage = target_props->max_damage;
        int damageInflicted =
            figure_properties_for_type(f->type)->missile_attack_value -
            target_props->missile_defense_value;
        const formation *m = formation_get(formationId);
		if (damageInflicted < 0) {
			damageInflicted = 0;
		}
		if (targetType == FIGURE_FORT_LEGIONARY && m->is_halted && m->layout == FORMATION_COLUMN) {
			damageInflicted = 1;
		}
		int targetDamage = damageInflicted + target->damage;
		if (targetDamage <= maxDamage) {
			target->damage = targetDamage;
		} else { // kill target
			target->damage = maxDamage + 1;
			target->actionState = FigureActionState_149_Corpse;
			target->waitTicks = 0;
			figure_play_die_sound(targetType);
			Formation_updateAfterDeath(formationId);
		}
		f->state = FigureState_Dead;
        // for missiles: building_id contains the figure who shot it
		int arrowFormation = figure_get(f->buildingId)->formationId;
        formation_record_missile_attack(formationId, arrowFormation);
		sound_effect_play(SOUND_EFFECT_ARROW_HIT);
	} else if (shouldDie) {
		f->state = FigureState_Dead;
	}
	int dir = (16 + f->direction - 2 * Data_State.map.orientation) % 16;
	f->graphicId = image_group(GROUP_FIGURE_MISSILE) + 16 + dir;
}

void FigureAction_spear(figure *f)
{
	f->useCrossCountry = 1;
	f->progressOnTile++;
	if (f->progressOnTile > 120) {
		f->state = FigureState_Dead;
	}
	int shouldDie = FigureMovement_crossCountryWalkTicks(f, 4);
	int targetId = Figure_getCitizenOnSameTile(f->id);
	if (targetId) {
        figure *target = figure_get(targetId);
		int targetType = target->type;
		int formationId = target->formationId;
        const figure_properties *target_props = figure_properties_for_type(targetType);
        int maxDamage = target_props->max_damage;
        int damageInflicted =
            figure_properties_for_type(f->type)->missile_attack_value -
            target_props->missile_defense_value;
        const formation *m = formation_get(formationId);
		if (damageInflicted < 0) {
			damageInflicted = 0;
		}
		if (targetType == FIGURE_FORT_LEGIONARY && m->is_halted && m->layout == FORMATION_COLUMN) {
			damageInflicted = 1;
		}
		int targetDamage = damageInflicted + target->damage;
		if (targetDamage <= maxDamage) {
			target->damage = targetDamage;
		} else { // kill target
			target->damage = maxDamage + 1;
			target->actionState = FigureActionState_149_Corpse;
			target->waitTicks = 0;
			figure_play_die_sound(targetType);
			Formation_updateAfterDeath(formationId);
		}
        // for missiles: building_id contains the figure who shot it
		int arrowFormation = figure_get(f->buildingId)->formationId;
		formation_record_missile_attack(formationId, arrowFormation);
		sound_effect_play(SOUND_EFFECT_JAVELIN);
		f->state = FigureState_Dead;
	} else if (shouldDie) {
		f->state = FigureState_Dead;
	}
	int dir = (16 + f->direction - 2 * Data_State.map.orientation) % 16;
	f->graphicId = image_group(GROUP_FIGURE_MISSILE) + dir;
}

void FigureAction_javelin(figure *f)
{
	f->useCrossCountry = 1;
	f->progressOnTile++;
	if (f->progressOnTile > 120) {
		f->state = FigureState_Dead;
	}
	int shouldDie = FigureMovement_crossCountryWalkTicks(f, 4);
	int targetId = Figure_getNonCitizenOnSameTile(f->id);
	if (targetId) {
        figure *target = figure_get(targetId);
		int targetType = target->type;
		int formationId = target->formationId;
        const figure_properties *target_props = figure_properties_for_type(targetType);
        int maxDamage = target_props->max_damage;
        int damageInflicted =
            figure_properties_for_type(f->type)->missile_attack_value -
            target_props->missile_defense_value;
        const formation *m = formation_get(formationId);
		if (damageInflicted < 0) {
			damageInflicted = 0;
		}
		if (targetType == FIGURE_ENEMY_CAESAR_LEGIONARY &&
			m->is_halted && m->layout == FORMATION_COLUMN) {
			damageInflicted = 1;
		}
		int targetDamage = damageInflicted + target->damage;
		if (targetDamage <= maxDamage) {
			target->damage = targetDamage;
		} else { // kill target
			target->damage = maxDamage + 1;
			target->actionState = FigureActionState_149_Corpse;
			target->waitTicks = 0;
			figure_play_die_sound(targetType);
			Formation_updateAfterDeath(formationId);
		}
        // for missiles: building_id contains the figure who shot it
		int javelinFormation = figure_get(f->buildingId)->formationId;
		formation_record_missile_attack(formationId, javelinFormation);
		sound_effect_play(SOUND_EFFECT_JAVELIN);
		f->state = FigureState_Dead;
	} else if (shouldDie) {
		f->state = FigureState_Dead;
	}
	int dir = (16 + f->direction - 2 * Data_State.map.orientation) % 16;
	f->graphicId = image_group(GROUP_FIGURE_MISSILE) + dir;
}

void FigureAction_bolt(figure *f)
{
	f->useCrossCountry = 1;
	f->progressOnTile++;
	if (f->progressOnTile > 120) {
		f->state = FigureState_Dead;
	}
	int shouldDie = FigureMovement_crossCountryWalkTicks(f, 4);
	int targetId = Figure_getNonCitizenOnSameTile(f->id);
	if (targetId) {
        figure *target = figure_get(targetId);
		int targetType = target->type;
		int formationId = target->formationId;
        const figure_properties *target_props = figure_properties_for_type(targetType);
        int maxDamage = target_props->max_damage;
        int damageInflicted =
            figure_properties_for_type(f->type)->missile_attack_value -
            target_props->missile_defense_value;
		if (damageInflicted < 0) {
			damageInflicted = 0;
		}
		int targetDamage = damageInflicted + target->damage;
		if (targetDamage <= maxDamage) {
			target->damage = targetDamage;
		} else { // kill target
			target->damage = maxDamage + 1;
			target->actionState = FigureActionState_149_Corpse;
			target->waitTicks = 0;
			figure_play_die_sound(targetType);
			Formation_updateAfterDeath(formationId);
		}
		sound_effect_play(SOUND_EFFECT_BALLISTA_HIT_PERSON);
		f->state = FigureState_Dead;
	} else if (shouldDie) {
		f->state = FigureState_Dead;
		sound_effect_play(SOUND_EFFECT_BALLISTA_HIT_GROUND);
	}
	int dir = (16 + f->direction - 2 * Data_State.map.orientation) % 16;
	f->graphicId = image_group(GROUP_FIGURE_MISSILE) + 32 + dir;
}
