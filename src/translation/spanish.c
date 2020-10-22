#include "translation/common.h"
#include "translation/translation.h"

static translation_string all_strings[] = {
    {TR_NO_PATCH_TITLE, "Parche 1.0.1.0 no instalado"},
    {TR_NO_PATCH_MESSAGE,
        "Tu instalación de Caesar 3 no tiene el parche 1.0.1.0 instalado. "
        "Puedes descargar el parche en:\n"
        URL_PATCHES "\n"
        "Continúa bajo tu propio riesgo."},
    {TR_MISSING_FONTS_TITLE, "No se han encontrado archivos de fuente"},
    {TR_MISSING_FONTS_MESSAGE,
        "Tu instalación de Caesar 3 requiere archivos de fuente adicionales. "
        "Puedes descargar las fuentes requeridas para tu idioma en:\n"
        URL_PATCHES},
    {TR_NO_EDITOR_TITLE, "Editor no instalado"},
    {TR_NO_EDITOR_MESSAGE,
        "Tu instalación de Caesar 3 no contiene los archivos del editor. "
        "Puedes descargar los archivos requeridos en:\n"
        URL_EDITOR},
    {TR_INVALID_LANGUAGE_TITLE, "Directorio de idioma inválido"},
    {TR_INVALID_LANGUAGE_MESSAGE,
        "El directorio que has seleccionado no contiene un paquete de idioma válido. "
        "Por favor, comprueba el registro de errores."},
    {TR_BUTTON_OK, "Aceptar"},
    {TR_BUTTON_CANCEL, "Cancelar"},
    {TR_BUTTON_RESET_DEFAULTS, "Por defecto"},
    {TR_BUTTON_CONFIGURE_HOTKEYS, "Configurar teclado"},
    {TR_CONFIG_TITLE, "Opciones de configuración de Julius"},
    {TR_CONFIG_LANGUAGE_LABEL, "Idioma:"},
    {TR_CONFIG_LANGUAGE_DEFAULT, "(por defecto)"},
    {TR_CONFIG_HEADER_UI_CHANGES, "Cambios en la interfaz"},
    {TR_CONFIG_HEADER_GAMEPLAY_CHANGES, "Cambios en el juego"},
    {TR_CONFIG_SHOW_INTRO_VIDEO, "Reproducir vídeos al inicio"},
    {TR_CONFIG_SIDEBAR_INFO, "Mostrar información adicional en el panel de control"},
    {TR_CONFIG_SMOOTH_SCROLLING, "Activar desplazamiento suave del mapa"},
    {TR_CONFIG_DISABLE_MAP_DRAG, "Desactivar el clic derecho para arrastrar el mapa"}, // TODO: Google Translate
    {TR_CONFIG_VISUAL_FEEDBACK_ON_DELETE, "Mejorar el feedback visual al limpiar terreno"},
    {TR_CONFIG_ALLOW_CYCLING_TEMPLES, "Permitir construcción secuencial de templos"},
    {TR_CONFIG_SHOW_WATER_STRUCTURE_RANGE, "Mostrar área cubierta por depósitos, fuentes y pozos al construir"},
    {TR_CONFIG_SHOW_CONSTRUCTION_SIZE, "Mostrar medidas de construcción al arrastrar"},
    {TR_CONFIG_HIGHLIGHT_LEGIONS, "Resaltar legión al pasar el cursor"}, // TODO: Google Translate
    {TR_CONFIG_SHOW_MILITARY_SIDEBAR, "Habilitar barra lateral militar"}, // TODO: Google Translate
    {TR_CONFIG_FIX_IMMIGRATION_BUG, "Corregir bug impidiendo immigración en Muy Difícil"},
    {TR_CONFIG_FIX_100_YEAR_GHOSTS, "Corregir bug creando fantasmas con 100 años"},
    {TR_CONFIG_FIX_EDITOR_EVENTS, "Corregir cambio de Emperador y supervivencia en misiones del editor"},
    {TR_HOTKEY_TITLE, "Configuración de atajos de teclado de Julius"},
    {TR_HOTKEY_LABEL, "Tecla"},
    {TR_HOTKEY_ALTERNATIVE_LABEL, "Alternativa"},
    {TR_HOTKEY_HEADER_ARROWS, "Teclas de dirección"},
    {TR_HOTKEY_HEADER_GLOBAL, "Teclas de atajo globales"},
    {TR_HOTKEY_HEADER_CITY, "Atajos en la ciudad"},
    {TR_HOTKEY_HEADER_ADVISORS, "Asesores"},
    {TR_HOTKEY_HEADER_OVERLAYS, "Capas"},
    {TR_HOTKEY_HEADER_BOOKMARKS, "Atajos de mapa"},
    {TR_HOTKEY_HEADER_EDITOR, "Editor"},
    {TR_HOTKEY_HEADER_BUILD, "Atajos de construcción"}, // TODO: Google Translate
    {TR_HOTKEY_ARROW_UP, "Arriba"},
    {TR_HOTKEY_ARROW_DOWN, "Abajo"},
    {TR_HOTKEY_ARROW_LEFT, "Izquierda"},
    {TR_HOTKEY_ARROW_RIGHT, "Derecha"},
    {TR_HOTKEY_TOGGLE_FULLSCREEN, "Activar pantalla completa"},
    {TR_HOTKEY_CENTER_WINDOW, "Centrar pantalla"},
    {TR_HOTKEY_RESIZE_TO_640, "Redimensionar a 640x480"},
    {TR_HOTKEY_RESIZE_TO_800, "Redimensionar a 800x600"},
    {TR_HOTKEY_RESIZE_TO_1024, "Redimensionar a 1024x768"},
    {TR_HOTKEY_SAVE_SCREENSHOT, "Capturar pantalla"},
    {TR_HOTKEY_SAVE_CITY_SCREENSHOT, "Capturar ciudad completa"},
    {TR_HOTKEY_BUILD_CLONE, "Clonar edificio bajo el cursor"}, // TODO: Google Translate
    {TR_HOTKEY_LOAD_FILE, "Cargar partida"},
    {TR_HOTKEY_SAVE_FILE, "Guardar partida"},
    {TR_HOTKEY_INCREASE_GAME_SPEED, "Subir velocidad de juego"},
    {TR_HOTKEY_DECREASE_GAME_SPEED, "Bajar velocidad de juego"},
    {TR_HOTKEY_TOGGLE_PAUSE, "Activar pausa"},
    {TR_HOTKEY_CYCLE_LEGION, "Circular entre legiones"},
    {TR_HOTKEY_ROTATE_MAP_LEFT, "Rotar mapa a la izquierda"},
    {TR_HOTKEY_ROTATE_MAP_RIGHT, "Rotar mapa a la derecha"},
    {TR_HOTKEY_SHOW_ADVISOR_LABOR, "Asesor laboral"},
    {TR_HOTKEY_SHOW_ADVISOR_MILITARY, "Asesor militar"},
    {TR_HOTKEY_SHOW_ADVISOR_IMPERIAL, "Asesor imperial"},
    {TR_HOTKEY_SHOW_ADVISOR_RATINGS, "Asesor de puntuación"},
    {TR_HOTKEY_SHOW_ADVISOR_TRADE, "Asesor de comercio"},
    {TR_HOTKEY_SHOW_ADVISOR_POPULATION, "Asesor de población"},
    {TR_HOTKEY_SHOW_ADVISOR_HEALTH, "Asesor de sanidad"},
    {TR_HOTKEY_SHOW_ADVISOR_EDUCATION, "Asesor de educación"},
    {TR_HOTKEY_SHOW_ADVISOR_ENTERTAINMENT, "Asesor de entretenimiento"},
    {TR_HOTKEY_SHOW_ADVISOR_RELIGION, "Asesor de religión"},
    {TR_HOTKEY_SHOW_ADVISOR_FINANCIAL, "Asesor de finanzas"},
    {TR_HOTKEY_SHOW_ADVISOR_CHIEF, "Asesor jefe"},
    {TR_HOTKEY_TOGGLE_OVERLAY, "Capa actual"},
    {TR_HOTKEY_SHOW_OVERLAY_WATER, "Capa de agua"},
    {TR_HOTKEY_SHOW_OVERLAY_FIRE, "Capa de riesgo de fuego"},
    {TR_HOTKEY_SHOW_OVERLAY_DAMAGE, "Capa de riesgo de daño"},
    {TR_HOTKEY_SHOW_OVERLAY_CRIME, "Capa de criminalidad"},
    {TR_HOTKEY_SHOW_OVERLAY_PROBLEMS, "Capa de problemas"},
    {TR_HOTKEY_GO_TO_BOOKMARK_1, "Ir a atajo de mapa 1"},
    {TR_HOTKEY_GO_TO_BOOKMARK_2, "Ir a atajo de mapa 2"},
    {TR_HOTKEY_GO_TO_BOOKMARK_3, "Ir a atajo de mapa 3"},
    {TR_HOTKEY_GO_TO_BOOKMARK_4, "Ir a atajo de mapa 4"},
    {TR_HOTKEY_SET_BOOKMARK_1, "Definir atajo de mapa 1"},
    {TR_HOTKEY_SET_BOOKMARK_2, "Definir atajo de mapa 2"},
    {TR_HOTKEY_SET_BOOKMARK_3, "Definir atajo de mapa 3"},
    {TR_HOTKEY_SET_BOOKMARK_4, "Definir atajo de mapa 4"},
    {TR_HOTKEY_EDITOR_TOGGLE_BATTLE_INFO, "Activar información de batalla"},
    {TR_HOTKEY_EDIT_TITLE, "Presiona nueva tecla"}
};

void translation_spanish(const translation_string **strings, int *num_strings)
{
    *strings = all_strings;
    *num_strings = sizeof(all_strings) / sizeof(translation_string);
}
