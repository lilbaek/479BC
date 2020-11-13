#include "translation/common.h"
#include "translation/translation.h"

static translation_string all_strings[] = {
    {TR_NO_PATCH_TITLE, "Patch 1.0.1.0 non installata"},
    {TR_NO_PATCH_MESSAGE,
        "La tua installazione di Caesar 3 non comprende la patch 1.0.1.0. "
        "Puoi scaricare la patch da:\n"
        URL_PATCHES "\n"
        "Continua a tuo rischio e pericolo."},
    {TR_MISSING_FONTS_TITLE, "Font mancanti"},
    {TR_MISSING_FONTS_MESSAGE,
        "La tua installazione di Caesar 3 richiede font extra. "
        "Puoi scaricare le font per la tua lingua da:\n"
        URL_PATCHES},
    {TR_NO_EDITOR_TITLE, "Editor non installato"},
    {TR_NO_EDITOR_MESSAGE,
        "La tua installazione di Caesar 3 non comprende l'editor delle mappe. "
        "Puoi scaricarlo da:\n"
        URL_EDITOR},
    {TR_INVALID_LANGUAGE_TITLE, "Cartella della lingua non valida"},
    {TR_INVALID_LANGUAGE_MESSAGE,
        "La cartella selezionata non contiene un pacchetto di lingua corretto. "
        "Per favore controlla il log degli errori."},
    {TR_BUTTON_OK, "OK"},
    {TR_BUTTON_CANCEL, "Annulla"},
    {TR_BUTTON_RESET_DEFAULTS, "Ripristina predefiniti"},
    {TR_BUTTON_CONFIGURE_HOTKEYS, "Configura tasti rapidi"},
    {TR_CONFIG_TITLE, "Opzioni di configurazione di Julius"},
    {TR_CONFIG_LANGUAGE_LABEL, "Lingua:"},
    {TR_CONFIG_LANGUAGE_DEFAULT, "(default)"},
    {TR_CONFIG_HEADER_UI_CHANGES, "Modifiche all'interfaccia utente"},
    {TR_CONFIG_HEADER_GAMEPLAY_CHANGES, "Modifiche al gioco"},
    {TR_CONFIG_SHOW_INTRO_VIDEO, "Mostra video introduttivo"},
    {TR_CONFIG_SIDEBAR_INFO, "Informazioni extra nel pannello di controllo"},
    {TR_CONFIG_SMOOTH_SCROLLING, "Abilita lo scorrimento continuo"},
    {TR_CONFIG_DISABLE_RIGHT_CLICK_MAP_DRAG, "Disabilita lo spostamento della mappa col tasto destro"},
    {TR_CONFIG_VISUAL_FEEDBACK_ON_DELETE, "Migliora l'aspetto liberando il terreno"},
    {TR_CONFIG_ALLOW_CYCLING_TEMPLES, "Consente la costruzione di ogni tempio in successione"},
    {TR_CONFIG_SHOW_WATER_STRUCTURE_RANGE, "Mostra la copertura di cisterne, fontane e pozzi"},
    {TR_CONFIG_SHOW_CONSTRUCTION_SIZE, "Mostra le dimensioni della costruzione durante il trascinamento"},
    {TR_CONFIG_HIGHLIGHT_LEGIONS, "Evidenzia la legione al passaggio del cursore"},
    {TR_CONFIG_SHOW_MILITARY_SIDEBAR, "Abilita il pannello militare"},
    {TR_CONFIG_FIX_IMMIGRATION_BUG, "Corregge il bug dell'immigrazione al livello molto difficile"},
    {TR_CONFIG_FIX_100_YEAR_GHOSTS, "Corregge il bug dei centenari"},
    {TR_CONFIG_FIX_EDITOR_EVENTS, "Elimina le complicazioni al cambio di Imperatore"},
    {TR_HOTKEY_TITLE, "Configurazione delle scorciatoie da tastiera"},
    {TR_HOTKEY_LABEL, "Tasto"},
    {TR_HOTKEY_ALTERNATIVE_LABEL, "Alternativa"},
    {TR_HOTKEY_HEADER_ARROWS, "Frecce"},
    {TR_HOTKEY_HEADER_GLOBAL, "Scorciatoie globali"},
    {TR_HOTKEY_HEADER_CITY, "Scorciatoie per la città"},
    {TR_HOTKEY_HEADER_ADVISORS, "Consiglieri"},
    {TR_HOTKEY_HEADER_OVERLAYS, "Tabelle"},
    {TR_HOTKEY_HEADER_BOOKMARKS, "Segnaposto sulla mappa"},
    {TR_HOTKEY_HEADER_EDITOR, "Editor"},
    {TR_HOTKEY_HEADER_BUILD, "Scorciatoie per costruzione"},
    {TR_HOTKEY_ARROW_UP, "Su"},
    {TR_HOTKEY_ARROW_DOWN, "Giù"},
    {TR_HOTKEY_ARROW_LEFT, "Sinistra"},
    {TR_HOTKEY_ARROW_RIGHT, "Destra"},
    {TR_HOTKEY_TOGGLE_FULLSCREEN, "Tutto schermo"},
    {TR_HOTKEY_CENTER_WINDOW, "Centra la finestra"},
    {TR_HOTKEY_RESIZE_TO_640, "Finestra a 640x480"},
    {TR_HOTKEY_RESIZE_TO_800, "Finestra a 800x600"},
    {TR_HOTKEY_RESIZE_TO_1024, "Finestra a 1024x768"},
    {TR_HOTKEY_SAVE_SCREENSHOT, "Salva Schermata"},
    {TR_HOTKEY_SAVE_CITY_SCREENSHOT, "Salva Schermata dell'intera città"},
    {TR_HOTKEY_BUILD_CLONE, "Clona l'edificio sotto il cursore"},
    {TR_HOTKEY_LOAD_FILE, "Carica file"},
    {TR_HOTKEY_SAVE_FILE, "Salva file"},
    {TR_HOTKEY_INCREASE_GAME_SPEED, "Aumenta la velocità"},
    {TR_HOTKEY_DECREASE_GAME_SPEED, "Diminuisce la velocità"},
    {TR_HOTKEY_TOGGLE_PAUSE, "Pausa"},
    {TR_HOTKEY_CYCLE_LEGION, "Scorri le legioni"},
    {TR_HOTKEY_ROTATE_MAP_LEFT, "Ruota la mappa a sinistra"},
    {TR_HOTKEY_ROTATE_MAP_RIGHT, "Ruota la mappa a destra"},
    {TR_HOTKEY_SHOW_ADVISOR_LABOR, "Consigliere del lavoro"},
    {TR_HOTKEY_SHOW_ADVISOR_MILITARY, "Consigliere militare"},
    {TR_HOTKEY_SHOW_ADVISOR_IMPERIAL, "Consigliere dell'Impero"},
    {TR_HOTKEY_SHOW_ADVISOR_RATINGS, "Consigliere dei livelli"},
    {TR_HOTKEY_SHOW_ADVISOR_TRADE, "Consigliere commerciale"},
    {TR_HOTKEY_SHOW_ADVISOR_POPULATION, "Consigliere della popolazione"},
    {TR_HOTKEY_SHOW_ADVISOR_HEALTH, "Consigliere della sanità"},
    {TR_HOTKEY_SHOW_ADVISOR_EDUCATION, "Consigliere dell'educazione"},
    {TR_HOTKEY_SHOW_ADVISOR_ENTERTAINMENT, "Consigliere degli intrattenimenti"},
    {TR_HOTKEY_SHOW_ADVISOR_RELIGION, "Consigliere religioso"},
    {TR_HOTKEY_SHOW_ADVISOR_FINANCIAL, "Consigliere finanziario"},
    {TR_HOTKEY_SHOW_ADVISOR_CHIEF, "Consigliere capo"},
    {TR_HOTKEY_TOGGLE_OVERLAY, "Cambia la tabella corrente"},
    {TR_HOTKEY_SHOW_OVERLAY_WATER, "Tabella acqua"},
    {TR_HOTKEY_SHOW_OVERLAY_FIRE, "Tabella fuoco"},
    {TR_HOTKEY_SHOW_OVERLAY_DAMAGE, "Tabella danni"},
    {TR_HOTKEY_SHOW_OVERLAY_CRIME, "Tabella crimine"},
    {TR_HOTKEY_SHOW_OVERLAY_PROBLEMS, "Tabella problemi"},
    {TR_HOTKEY_GO_TO_BOOKMARK_1, "Va al segnaposto 1"},
    {TR_HOTKEY_GO_TO_BOOKMARK_2, "Va al segnaposto 2"},
    {TR_HOTKEY_GO_TO_BOOKMARK_3, "Va al segnaposto 3"},
    {TR_HOTKEY_GO_TO_BOOKMARK_4, "Va al segnaposto 4"},
    {TR_HOTKEY_SET_BOOKMARK_1, "Fissa il segnaposto 1"},
    {TR_HOTKEY_SET_BOOKMARK_2, "Fissa il segnaposto 2"},
    {TR_HOTKEY_SET_BOOKMARK_3, "Fissa il segnaposto 3"},
    {TR_HOTKEY_SET_BOOKMARK_4, "Fissa il segnaposto 4"},
    {TR_HOTKEY_EDITOR_TOGGLE_BATTLE_INFO, "Informazioni sulla battaglia"},
    {TR_HOTKEY_EDIT_TITLE, "Premi un nuovo tasto"}
};

void translation_italian(const translation_string **strings, int *num_strings)
{
    *strings = all_strings;
    *num_strings = sizeof(all_strings) / sizeof(translation_string);
}
