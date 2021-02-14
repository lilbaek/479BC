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
    {TR_BUILD_ALL_TEMPLES, "Todo"},
    {TR_BUTTON_OK, "Aceptar"},
    {TR_BUTTON_CANCEL, "Cancelar"},
    {TR_BUTTON_RESET_DEFAULTS, "Por defecto"},
    {TR_BUTTON_CONFIGURE_HOTKEYS, "Atajos de teclado"},
    {TR_BUTTON_NEXT, "+"},
    {TR_BUTTON_PREV, "-"},
    {TR_CONFIG_TITLE, "Opciones de configuración de Augustus"},
    {TR_CONFIG_LANGUAGE_LABEL, "Idioma:"},
    {TR_CONFIG_LANGUAGE_DEFAULT, "(por defecto)"},
    {TR_CONFIG_PAGE_LABEL, "Página"},
    {TR_CONFIG_DISPLAY_SCALE, "Escala del monitor:"},
    {TR_CONFIG_CURSOR_SCALE, "Escala del cursor:"},
    {TR_CONFIG_HEADER_UI_CHANGES, "Cambios en la interfaz"},
    {TR_CONFIG_HEADER_GAMEPLAY_CHANGES, "Cambios en el juego"},
    {TR_CONFIG_SHOW_INTRO_VIDEO, "Reproducir vídeos al inicio"},
    {TR_CONFIG_SIDEBAR_INFO, "Mostrar información adicional en el panel de control"},
    {TR_CONFIG_SMOOTH_SCROLLING, "Activar desplazamiento suave del mapa"},
    {TR_CONFIG_DISABLE_RIGHT_CLICK_MAP_DRAG, "Desactivar click derecho para desplazar el mapa"},
    {TR_CONFIG_VISUAL_FEEDBACK_ON_DELETE, "Mejorar el feedback visual al limpiar terreno"},
    {TR_CONFIG_ALLOW_CYCLING_TEMPLES, "Permitir construcción secuencial de templos"},
    {TR_CONFIG_SHOW_WATER_STRUCTURE_RANGE, "Mostrar área cubierta por depósitos, fuentes y pozos al construir"},
    {TR_CONFIG_SHOW_CONSTRUCTION_SIZE, "Mostrar medidas de construcción al arrastrar"},
    {TR_CONFIG_HIGHLIGHT_LEGIONS, "Resaltar legión bajo el cursor"},
    {TR_CONFIG_ROTATE_MANUALLY, "Rotar Portón y Arco del Triunfo manualmente"},
    {TR_CONFIG_SHOW_MILITARY_SIDEBAR, "Activar barra militar lateral"},
    {TR_CONFIG_FIX_IMMIGRATION_BUG, "Corregir bug impidiendo inmigración en Muy Difícil"},
    {TR_CONFIG_FIX_100_YEAR_GHOSTS, "Corregir bug creando fantasmas con 100 años"},
    {TR_CONFIG_FIX_EDITOR_EVENTS, "Corregir cambio de Emperador y tiempo de supervivencia en misiones del editor"},
    {TR_CONFIG_DRAW_WALKER_WAYPOINTS, "Ver ruta de caminantes en Capas al hacer click derecho en un edificio"},
    {TR_CONFIG_ENABLE_ZOOM, "Habilitar zoom (usa más RAM, puede ralentizar el juego)"},
    {TR_CONFIG_COMPLETE_RATING_COLUMNS, "Arreglar columnas de ratings incompletas con bajos requerimientos"},
    {TR_CONFIG_GRANDFESTIVAL, "Super-Festivales permiten bendiciones extras de un dios"},
    {TR_CONFIG_JEALOUS_GODS, "Desactivar la envidia entre dioses"},
    {TR_CONFIG_GLOBAL_LABOUR, "Activar mano de obra global"},
    {TR_CONFIG_SCHOOL_WALKERS, "Extender el rango de los caminantes de los colegios"},
    {TR_CONFIG_RETIRE_AT_60, "Cambiar le edad de jubilación de 50 a 60 años"},
    {TR_CONFIG_FIXED_WORKERS, "Mano de obra fija - 38% de la población plebeya"},
    {TR_CONFIG_EXTRA_FORTS, "Permite construir 4 fuertes adicionales"},
    {TR_CONFIG_WOLVES_BLOCK, "Deshabilitar la construcción alrededor de lobos"},
    {TR_CONFIG_DYNAMIC_GRANARIES, "Bloquear los caminos no conectados de un granero"},
    {TR_CONFIG_MORE_STOCKPILE, "Las residencias reservan más bienes del mercado"},
    {TR_CONFIG_NO_SUPPLIER_DISTRIBUTION, "Las mercaderes que compran no distribuyen bienes"},
    {TR_CONFIG_IMMEDIATELY_DELETE_BUILDINGS, "Destruir edificios inmediatamente"},
    {TR_CONFIG_GETTING_GRANARIES_GO_OFFROAD, "Los carreteros de graneros que obtienen bienes no necesitan caminos"},
    {TR_CONFIG_GRANARIES_GET_DOUBLE, "Doblar la capacidad de carreteros de graneros que obtienen bienes"},
    {TR_CONFIG_TOWER_SENTRIES_GO_OFFROAD, "Los centinelas de las torres no necesitan conexión a una barraca"},
    {TR_CONFIG_FARMS_DELIVER_CLOSE, "Granjas y dársenas solo distribuyen a graneros cercanos"},
    {TR_CONFIG_DELIVER_ONLY_TO_ACCEPTING_GRANARIES, "No distribuir alimentos a graneros que obtienen bienes"},
    {TR_CONFIG_ALL_HOUSES_MERGE, "Todas las residencias se unen"},
    {TR_CONFIG_WINE_COUNTS_IF_OPEN_TRADE_ROUTE, "Las rutas de comercio abiertas cuentan como provisión de distintos tipos de vino"},
    {TR_CONFIG_RANDOM_COLLAPSES_TAKE_MONEY, "Pozos de arcilla y Minas de hierro no colapsan, cobran dinero"},
    {TR_CONFIG_MULTIPLE_BARRACKS, "Permite construir múltiples barracas"},
    {TR_CONFIG_NOT_ACCEPTING_WAREHOUSES, "Los almacenes no aceptan bienes cuando se construyen"},
    {TR_CONFIG_HOUSES_DONT_EXPAND_INTO_GARDENS, "Las residencias no se expanden hacia los jardines"},
    {TR_HOTKEY_TITLE, "Configuración de atajos de teclado de Augustus"},
    {TR_HOTKEY_LABEL, "Principal"},
    {TR_HOTKEY_ALTERNATIVE_LABEL, "Secundaria"},
    {TR_HOTKEY_HEADER_ARROWS, "Teclas de dirección"},
    {TR_HOTKEY_HEADER_GLOBAL, "Atajos globales"},
    {TR_HOTKEY_HEADER_CITY, "Atajos de la ciudad"},
    {TR_HOTKEY_HEADER_ADVISORS, "Asesores"},
    {TR_HOTKEY_HEADER_OVERLAYS, "Capas"},
    {TR_HOTKEY_HEADER_BOOKMARKS, "Atajos de mapa"},
    {TR_HOTKEY_HEADER_EDITOR, "Editor"},
    {TR_HOTKEY_HEADER_BUILD, "Atajos de construcción"},
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
    {TR_HOTKEY_BUILD_CLONE, "Clonar edificio bajo el cursor"},
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
    {TR_HOTKEY_SHOW_ADVISOR_HOUSING, "Asesor de vivienda"},
    {TR_HOTKEY_TOGGLE_OVERLAY, "Capa actual"},
    {TR_HOTKEY_SHOW_OVERLAY_WATER, "Capa de agua"},
    {TR_HOTKEY_SHOW_OVERLAY_FIRE, "Capa de riesgo de fuego"},
    {TR_HOTKEY_SHOW_OVERLAY_DAMAGE, "Capa de riesgo de daño"},
    {TR_HOTKEY_SHOW_OVERLAY_CRIME, "Capa de criminalidad"},
    {TR_HOTKEY_SHOW_OVERLAY_PROBLEMS, "Capa de problemas"},
    {TR_HOTKEY_ROTATE_BUILDING, "Rotar edificio"},
    {TR_HOTKEY_GO_TO_BOOKMARK_1, "Ir a atajo de mapa 1"},
    {TR_HOTKEY_GO_TO_BOOKMARK_2, "Ir a atajo de mapa 2"},
    {TR_HOTKEY_GO_TO_BOOKMARK_3, "Ir a atajo de mapa 3"},
    {TR_HOTKEY_GO_TO_BOOKMARK_4, "Ir a atajo de mapa 4"},
    {TR_HOTKEY_SET_BOOKMARK_1, "Definir atajo de mapa 1"},
    {TR_HOTKEY_SET_BOOKMARK_2, "Definir atajo de mapa 2"},
    {TR_HOTKEY_SET_BOOKMARK_3, "Definir atajo de mapa 3"},
    {TR_HOTKEY_SET_BOOKMARK_4, "Definir atajo de mapa 4"},
    {TR_HOTKEY_EDITOR_TOGGLE_BATTLE_INFO, "Activar información de batalla"},
    {TR_HOTKEY_EDIT_TITLE, "Presiona nueva tecla"},
    {TR_BUILDING_ROADBLOCK, "Control de carretera"},
    {TR_BUILDING_ROADBLOCK_DESC, "Los controles de carretera detienen a los ciudadanos ociosos."},
    {TR_BUILDING_ARCHITECT_GUILD, "Gremio de ingenieros"},
    {TR_BUILDING_ARCHITECT_GUILD_DESC, "Los ingenieros aquí trabajan sin cesar para construir monumentos a la gloria de Roma."},
    {TR_BUILDING_GRAND_TEMPLE_ADD_MODULE, "Confiere un epíteto al templo"},
    {TR_BUILDING_GRAND_TEMPLE_CERES, "Ceres"},
    {TR_BUILDING_GRAND_TEMPLE_NEPTUNE, "Neptuno"},
    {TR_BUILDING_GRAND_TEMPLE_MERCURY, "Mercurio"},
    {TR_BUILDING_GRAND_TEMPLE_MARS, "Marte"},
    {TR_BUILDING_GRAND_TEMPLE_VENUS, "Venus"},
    {TR_BUILDING_PANTHEON, "Panteón"},
    {TR_BUILDING_GRAND_TEMPLE_CERES_DESC, "Gran templo de Ceres"},
    {TR_BUILDING_GRAND_TEMPLE_NEPTUNE_DESC, "Gran templo de Neptuno"},
    {TR_BUILDING_GRAND_TEMPLE_MERCURY_DESC, "Gran templo de Mercurio"},
    {TR_BUILDING_GRAND_TEMPLE_MARS_DESC, "Gran templo de Marte"},
    {TR_BUILDING_GRAND_TEMPLE_VENUS_DESC, "Gran templo de Venus"},
    {TR_BUILDING_PANTHEON_DESC, "Panteón"},
    {TR_BUILDING_GRAND_TEMPLE_CERES_BONUS_DESC, "Los carreteros de las granjas son un 50% más rápidos."},
    {TR_BUILDING_GRAND_TEMPLE_NEPTUNE_BONUS_DESC, "Otorga +1 rango a los pozos y fuentes y +2 a depósitos. Los servicios hidráulicos usan 50% menos trabajadores. Los barcos de comercio viajan un 25% más rápido."},
    {TR_BUILDING_GRAND_TEMPLE_MERCURY_BONUS_DESC, "Los comerciantes terrestres y marítimos tienen un 50% más de capacidad. Las caravanas de mercaderes son un 25% más rápidas."},
    {TR_BUILDING_GRAND_TEMPLE_MARS_BONUS_DESC, "Entrena soldados como una barraca. Se pueden construir 4 fuertes adicionales."},
    {TR_BUILDING_GRAND_TEMPLE_VENUS_BONUS_DESC, "Jardines, estatuas y templos de la ciudad brillan con increíble belleza, aumentando su atractivo. Las residencias reservan más bienes e involucionan más lento."},
    {TR_BUILDING_PANTHEON_BONUS_DESC, "Otorga acceso a todos los dioses. Celebra festivales anuales y reduce el mantenimiento de las iglesias un 25%."},
    {TR_BUILDING_GRAND_TEMPLE_CERES_MODULE_1_DESC, "Los sacerdotes reducen el consumo de alimento en un 20%."},
    {TR_BUILDING_GRAND_TEMPLE_NEPTUNE_MODULE_1_DESC, "Los templos producen cuadrigas para el hipódromo."},
    {TR_BUILDING_GRAND_TEMPLE_MERCURY_MODULE_1_DESC, "Los sacerdotes reducen el consumo de cerámica y muebles en un 20%."},
    {TR_BUILDING_GRAND_TEMPLE_MARS_MODULE_1_DESC, "Los templos generan alimentos en una cantidad proporcional al número de casas que cubren y lo entregan al puesto de suministro."},
    {TR_BUILDING_GRAND_TEMPLE_VENUS_MODULE_1_DESC, "El Gran templo produce vino en una cantidad proporcional a la población con acceso a templos de Venus. Los templos lo recolectan y distribuyen a las residencias."},
    {TR_BUILDING_GRAND_TEMPLE_CERES_MODULE_2_DESC, "Los sacerdotes recolectan y distribuyen aceite y un tipo de alimento del disponible en la ciudad."},
    {TR_BUILDING_GRAND_TEMPLE_NEPTUNE_MODULE_2_DESC, "Los sacerdotes expanden la capacidad de las viviendas en un 5%. El Gran templo provee agua como si fuese un depósito."},
    {TR_BUILDING_GRAND_TEMPLE_MERCURY_MODULE_2_DESC, "Los sacerdotes reducen el consumo de aceite y vino en un 20%."},
    {TR_BUILDING_GRAND_TEMPLE_MARS_MODULE_2_DESC, "Los sacerdotes reducen el consumo de todos los bienes en un 10%."},
    {TR_BUILDING_GRAND_TEMPLE_VENUS_MODULE_2_DESC, "Los sacerdotes proveen entretenimiento y hermosean las residencias, aumentando el atractivo del vecindario sin necesidad de decoraciones adicionales."},
    {TR_BUILDING_PANTHEON_MODULE_1_DESC, "Todos los templos en la ciudad envían sacerdotes al Panteón, esparciendo la fé y bendiciones de sus dioses."},
    {TR_BUILDING_PANTHEON_MODULE_2_DESC, "Las residencias con acceso al Panteón puede evolucionar un nivel adicional."},
    {TR_BUILDING_GRAND_TEMPLE_CERES_DESC_MODULE_1, "Templo de Ceres Promitor"},
    {TR_BUILDING_GRAND_TEMPLE_CERES_DESC_MODULE_2, "Templo de Ceres Reparator"},
    {TR_BUILDING_GRAND_TEMPLE_NEPTUNE_DESC_MODULE_1, "Templo de Neptunus Equester"},
    {TR_BUILDING_GRAND_TEMPLE_NEPTUNE_DESC_MODULE_2, "Templo de Neptunus Adiutor"},
    {TR_BUILDING_GRAND_TEMPLE_MERCURY_DESC_MODULE_1, "Templo de Mercurius Fortunus"},
    {TR_BUILDING_GRAND_TEMPLE_MERCURY_DESC_MODULE_2, "Templo de Mercurius Abundantia"},
    {TR_BUILDING_GRAND_TEMPLE_MARS_DESC_MODULE_1, "Templo de Mars Ultor"},
    {TR_BUILDING_GRAND_TEMPLE_MARS_DESC_MODULE_2, "Templo de Mars Quirinus"},
    {TR_BUILDING_GRAND_TEMPLE_VENUS_DESC_MODULE_1, "Templo de Venus Verticordia"},
    {TR_BUILDING_GRAND_TEMPLE_VENUS_DESC_MODULE_2, "Templo de Venus Genetrix"},
    {TR_BUILDING_PANTHEON_DESC_MODULE_1, "Pantheum Ara Maxima"},
    {TR_BUILDING_PANTHEON_DESC_MODULE_2, "Pantheum Roma Aeterna"},
    {TR_BUILDING_GRAND_TEMPLE_MENU, "Gran templo"},
    {TR_BUILDING_WORK_CAMP, "Campamento de trabajo"},
    {TR_BUILDING_WORK_CAMP_DESC, "Los trabajadores se reunen aquí para transportar los materiales necesarios a los sitios de construcción."},
    {TR_HEADER_HOUSING, "Vivienda"},
    {TR_ADVISOR_HOUSING_ROOM, "En la ciudad hay habitaciones para"},
    {TR_ADVISOR_HOUSING_NO_ROOM, "No hay habitaciones disponibles en la ciudad."},
    {TR_ADVISOR_RESIDENCES_DEMANDING_POTTERY, "Residencias usando cerámica"},
    {TR_ADVISOR_RESIDENCES_DEMANDING_FURNITURE, "Residencias usando muebles"},
    {TR_ADVISOR_RESIDENCES_DEMANDING_OIL, "Residencias usando aceite"},
    {TR_ADVISOR_RESIDENCES_DEMANDING_WINE, "Residencias usando vino"},
    {TR_ADVISOR_TOTAL_NUM_HOUSES, "Residencias totales:"},
    {TR_ADVISOR_AVAILABLE_HOUSING_CAPACITY, "Capacidad disponible:"},
    {TR_ADVISOR_TOTAL_HOUSING_CAPACITY, "Capacidad total:"},
    {TR_ADVISOR_ADVISOR_HEADER_HOUSING, "Población - Vivienda"},
    {TR_ADVISOR_BUTTON_GRAPHS, "Gráficos"},
    {TR_ADVISOR_HOUSING_PROSPERITY_RATING, "La puntuación de prosperidad de las viviendas es"},
    {TR_ADVISOR_PERCENTAGE_IN_VILLAS_PALACES, "El porcentaje de tu población en villas y palacios es"},
    {TR_ADVISOR_PERCENTAGE_IN_TENTS_SHACKS, "El porcentaje de tu población en tiendas y tugurios es"},
    {TR_ADVISOR_AVERAGE_TAX, "El ingreso promedio de impuestos por residencia es"},
    {TR_ADVISOR_AVERAGE_AGE, "La edad promedio de tu población es"},
    {TR_ADVISOR_PERCENT_IN_WORKFORCE, "El porcentaje de tu población en la fuerza de trabajo es"},
    {TR_ADVISOR_BIRTHS_LAST_YEAR, "Nacimientos el año pasado:"},
    {TR_ADVISOR_DEATHS_LAST_YEAR, "Muertes el año pasado:"},
    {TR_ADVISOR_TOTAL_POPULATION, "residentes totales"},
    {TR_REQUIRED_RESOURCES, "Recursos entregados para la fase actual:"},
    {TR_BUILDING_GRAND_TEMPLE_CONSTRUCTION_DESC, "Construir Grandes templos requiere material de un almacén, trabajadores de un campamento de trabajo e ingenieros de un gremio de ingenieros."},
    {TR_CONSTRUCTION_PHASE, "Fase de construcción:"},
    {TR_ADD_MODULE, "¿Rededicar templo?"},
    {TR_BUILDING_TEMPLE_MODULE_CONSTRUCTED, "Epíteto conferido."},
    {TR_BUILDING_CERES_TEMPLE_QUOTE, "La primera Ceres el terrón dividió con el corvo arado,\nla primera dio granos y alimentos suaves a las tierras,\nla primera dio sus leyes;Ceres todo esto regaló,\na ella de cantar yo he; ojalá tan sólo decir pudiera\ncanciones dignas de la diosa. Ciertamente digna es.\n-Ovidio, Metamorfosis, Libro V"},
    {TR_BUILDING_NEPTUNE_TEMPLE_QUOTE, "Del mar la ira desaparece y, dejada su tricúspide arma\ncalma las aguas su regente, y al que sobre el profundo\nemerge y sus hombros con su innato múrice cubre,\nal azul Tritón llama, y en su concha sonante\nsoplar le ordena, y los oleajes y las corrientes ya\nrevocar, su señal dando.\n-Ovidio, Metamorfosis, Libro I"},
    {TR_BUILDING_MERCURY_TEMPLE_QUOTE, "Se pueden apresar los bueyes y las gordas ovejas,\nse pueden adquirir los trípodes y los tostados alazanes;\npero no es posible hacer volver el alma humana,\nuna vez ha salvado la barrera que forman los dientes.\n-Homero, La Iliada, Libro IX"},
    {TR_BUILDING_MARS_TEMPLE_QUOTE, "Trabajaban con afán en concluir un carro\ny unas veloces ruedas para Marte,\ncon que concita a los hombres y las ciudades.\n-Virgilio, La Eneida, Libro VIII"},
    {TR_BUILDING_VENUS_TEMPLE_QUOTE, "Por Venus y con su tierna madre el volador Cupido,\ngoces cuán grandes sentí, cuán manifiesto deleite,\ncuán relajada hasta en las médulas he quedado\ncómo acordarse agrada.\n-Ovidio, Metamorfosis, Libro IX"},
    {TR_BUILDING_PANTHEON_QUOTE, "Abrese en tanto la morada del omnipotente Olimpo,\ny el padre de los dioses y rey de los hombres\nconvoca a concilio en la estrellada mansión,\ndesde donde abarca con la vista toda la tierra, y\nlos reales de los Troyanos y los pueblos latinos.\nToman asiento los dioses\nen una estancia abierta por ambos lados.\n-Virgilio, La Eneida, Libro X"},
    {TR_BUILDING_GRAND_TEMPLE_PHASE_1, "(Podium)"},
    {TR_BUILDING_GRAND_TEMPLE_PHASE_2, "(Portico)"},
    {TR_BUILDING_GRAND_TEMPLE_PHASE_3, "(Cella)"},
    {TR_BUILDING_GRAND_TEMPLE_PHASE_4, "(Praecinctum)"},
    {TR_BUILDING_GRAND_TEMPLE_PHASE_5, "(Dedicatio)"},
    {TR_BUILDING_GRAND_TEMPLE_PHASE_1_TEXT, "Los trabajadores construyen el podium que elevará el templo y proveerá una sólida fundación para hacer frente al paso del tiempo."},
    {TR_BUILDING_GRAND_TEMPLE_PHASE_2_TEXT, "Los artesanos elaboran el portico, que contiene el altar de sacrificios del templo."},
    {TR_BUILDING_GRAND_TEMPLE_PHASE_3_TEXT, "Los ingenieros elaboran la cella, la cámara que albergará sagradas imágenes."},
    {TR_BUILDING_GRAND_TEMPLE_PHASE_4_TEXT, "Ha comenzado el trabajo en el praecinctum del templo, donde los fieles se reunirán para las ceremonias públicas."},
    {TR_BUILDING_GRAND_TEMPLE_PHASE_5_TEXT, "Con el templo casi completo, los sacerdotes se reunen para ejecutar los ritos de dedicación."},
    {TR_BUILDING_MENU_TREES, "Árboles"},
    {TR_BUILDING_MENU_PATHS, "Caminos"},
    {TR_BUILDING_MENU_PARKS, "Parques"},
    {TR_BUILDING_SMALL_POND, "Estanque pequeño"},
    {TR_BUILDING_LARGE_POND, "Estanque grande"},
    {TR_BUILDING_PINE_TREE, "Pino"},
    {TR_BUILDING_FIR_TREE, "Abeto"},
    {TR_BUILDING_OAK_TREE, "Roble"},
    {TR_BUILDING_ELM_TREE, "Olmo"},
    {TR_BUILDING_FIG_TREE, "Higuera"},
    {TR_BUILDING_PLUM_TREE, "Ciruelo"},
    {TR_BUILDING_PALM_TREE, "Palmera"},
    {TR_BUILDING_DATE_TREE, "Datilero"},
    {TR_BUILDING_PINE_PATH, "Camino de pinos"},
    {TR_BUILDING_FIR_PATH, "Camino de abetos"},
    {TR_BUILDING_OAK_PATH, "Camino de robles"},
    {TR_BUILDING_ELM_PATH, "Camino de olmos"},
    {TR_BUILDING_FIG_PATH, "Camino de higueras"},
    {TR_BUILDING_PLUM_PATH, "Camino de ciruelos"},
    {TR_BUILDING_PALM_PATH, "Camino de palmeras"},
    {TR_BUILDING_DATE_PATH, "Camino de datileros"},
    {TR_BUILDING_BLUE_PAVILION, "Pabellón azúl"},
    {TR_BUILDING_RED_PAVILION, "Pabellón rojo"},
    {TR_BUILDING_ORANGE_PAVILION, "Pabellón naranja"},
    {TR_BUILDING_YELLOW_PAVILION, "Pabellón amarillo"},
    {TR_BUILDING_GREEN_PAVILION, "Pabellón verde"},
    {TR_BUILDING_SMALL_STATUE_ALT, "Estatua de la diosa"},
    {TR_BUILDING_SMALL_STATUE_ALT_B, "Estatua del senador"},
    {TR_BUILDING_OBELISK, "Obelisco"},
    {TR_BUILDING_POND_DESC, "Depósitos llenan los estanques que proveen agua para plantas y animales y un fresco y relajante oasis para la gente. Todos los ciudadanos amarían vivir cerca de un estanque."},
    {TR_BUILDING_WINDOW_POND, "Estanque"},
    {TR_BUILDING_OBELISK_DESC, "Un prestigioso monumento, el trabajo de un antiguo rey del Nilo. Los Egipcios no lo necesitaban."},
    {TR_ADVISOR_FINANCE_LEVIES, "Mantenimiento de edificios"},
    {TR_CONFIRM_DELETE_MONUMENT, "Demoliendo este monumento"},
    {TR_SELECT_EPITHET_PROMPT_HEADER, "Selecciona el epíteto a conferir"},
    {TR_SELECT_EPITHET_PROMPT_TEXT, "Conferir un epíteto dedicará permanentemente tu templo a un aspecto de su deidad, inbuyendo a los sacerdotes de poder divino. Esto costará 1000 Dn."},
    {TR_BUILDING_INFO_MONTHLY_LEVY, "/mes"},
    {TR_BUILDING_MESS_HALL, "Puesto de suministro"},
    {TR_BUILDING_MESS_HALL_DESC, "El puesto de suministro recolecta alimentos de los graneros de la ciudad para alimentar a los soldados de los fuertes. Un mal abastecimiento ralentizará el reclutamiento y dañará la moral."},
    {TR_BUILDING_MESS_HALL_FULFILLMENT, "Provisiones del mes pasado:"},
    {TR_BUILDING_MESS_HALL_TROOP_HUNGER, "Los soldados están: "},
    {TR_BUILDING_MESS_HALL_TROOP_HUNGER_1, "Bien alimentados"},
    {TR_BUILDING_MESS_HALL_TROOP_HUNGER_2, "Alimentados"},
    {TR_BUILDING_MESS_HALL_TROOP_HUNGER_3, "Hambrientos"},
    {TR_BUILDING_MESS_HALL_TROOP_HUNGER_4, "Muy hambrientos"},
    {TR_BUILDING_MESS_HALL_TROOP_HUNGER_5, "Famélicos"},
    {TR_BUILDING_MESS_HALL_FOOD_TYPES_BONUS_1, "La variada dieta de tus soldados aumenta mucho su moral."},
    {TR_BUILDING_MESS_HALL_FOOD_TYPES_BONUS_2, "La diversa y nutritiva dieta que gozan tus soldados aumenta mucho su moral."},
    {TR_BUILDING_MESS_HALL_NO_SOLDIERS, "No tienes soldados que alimentar."},
    {TR_BUILDING_MESS_HALL_MONTHS_FOOD_STORED, "Meses de comida almacenados:"},
    {TR_BUILDING_BARRACKS_FOOD_WARNING, "La escasez de alimento en tu puesto de suministro ralentiza el reclutamiento."},
    {TR_BUILDING_BARRACKS_FOOD_WARNING_2, "La escasez crítica de alimento en tu puesto de suministro estropea el reclutamiento."},
    {TR_BUILDING_LEGION_FOOD_WARNING_1, "La reciente escasez de alimento disminuyó la moral"},
    {TR_BUILDING_LEGION_FOOD_WARNING_2, "La reciente escasez de alimento disminuyó mucho la moral"},
    {TR_BUILDING_LEGION_STARVING, "Escasez de alimento"},
    {TR_ADVISOR_LEGION_FOOD_SATISFIED, "Tus soldados tienen todo el alimento que necesitan."},
    {TR_ADVISOR_LEGION_FOOD_NEEDED, "Tus soldados necesitan más alimento."},
    {TR_ADVISOR_LEGION_FOOD_CRITICAL, "¡Tus soldados están famélicos!"},
    {TR_ADVISOR_LEGION_MONTHS_FOOD_STORED, "Meses de comida almacenados en puestos de suministros:"},
    {TR_CITY_MESSAGE_TITLE_MESS_HALL_NEEDS_FOOD, "Soldados hambrientos"},
    {TR_CITY_MESSAGE_TEXT_MESS_HALL_NEEDS_FOOD, "Tu puesto de suministro tiene muy poca comida, y la moral de tus tropas mengua. Asegúrate de que tu puesto de suministros puede acceder a un granero bien surtido."},
    {TR_CITY_MESSAGE_TEXT_MESS_HALL_MISSING, "Tu ciudad perdió su puesto de suministros, y tus tropas tienen hambre. Construye un puesto de suministros inmediatamente."},
    {TR_MARKET_SPECIAL_ORDERS_HEADER, "Bienes a recolectar"},
    {TR_WARNING_NO_MESS_HALL, "Debes construir primero un puesto de suministro para alimentar a tus tropas."},
    {TR_WARNING_MAX_GRAND_TEMPLES, "Solo dos Grandes templos pueden construirse."},
    {TR_CITY_MESSAGE_TITLE_GRAND_TEMPLE_COMPLETE, "Gran templo completado"},
    {TR_CITY_MESSAGE_TEXT_GRAND_TEMPLE_COMPLETE, "Sacerdotes y fieles se congregan en el nuevo Gran templo para los ritos de consagración. Tu gente ve asombrada la majestuosidad de tus obras, y tu deidad elegida te colma de favor."},
    {TR_CITY_MESSAGE_TITLE_MERCURY_BLESSING, "Una bendición de Mercurio"},
    {TR_CITY_MESSAGE_TEXT_MERCURY_BLESSING, "Encantado por tu atención, Mercurio bendice tu industria, descubriendo materias primas para tus artesanos."},
    {TR_FIGURE_TYPE_WORK_CAMP_WORKER, "Capataz"},
    {TR_FIGURE_TYPE_WORK_CAMP_SLAVE, "Transportista"},
    {TR_FIGURE_TYPE_WORK_CAMP_ARCHITECT, "Arquitecto"},
    {TR_FIGURE_TYPE_MESS_HALL_SUPPLIER, "Intendente militar"},
    {TR_FIGURE_TYPE_MESS_HALL_COLLECTOR, "Sirviente del campo"},
    {TR_BUILDING_CERES_TEMPLE_MODULE_DESC, "El Gran templo coordina a los sacerdotes de Ceres para ayudar distributendo comida y aceite a tus hambrientos ciudadanos."},
    {TR_BUILDING_VENUS_TEMPLE_MODULE_DESC, "El Gran templo permite la distribución de vino sagrado a tus ciudadanos por los sacerdotes de Venus ."},
    {TR_BUILDING_MARS_TEMPLE_MODULE_DESC, "El Gran templo coordina la entrega de provisiones a tu puesto de suministros por los sacerdotes de Marte."},
    {TR_BUILDING_SMALL_TEMPLE_CERES_NAME, "Templo de Ceres"},
    {TR_BUILDING_SMALL_TEMPLE_NEPTUNE_NAME, "Templo de Neptuno"},
    {TR_BUILDING_SMALL_TEMPLE_MERCURY_NAME, "Templo de Mercurio"},
    {TR_BUILDING_SMALL_TEMPLE_MARS_NAME, "Templo de Marte0"},
    {TR_BUILDING_SMALL_TEMPLE_VENUS_NAME, "Templo de Venus"},
    {TR_FIGURE_TYPE_PRIEST_SUPPLIER, "Sacerdote"},
    // Transcription of mission_exact4.wav
    {TR_PHRASE_FIGURE_MISSIONARY_EXACT_4, "\"Haré lo que pueda para aplacar a estos bárbaros. Estoy seguro de que dejarán de atacar la ciudad cuando haya hablado con ellos.\""},
    {TR_CITY_MESSAGE_TITLE_PANTHEON_FESTIVAL, "Festival anual"},
    {TR_CITY_MESSAGE_TEXT_PANTHEON_FESTIVAL_CERES, "Los fieles acuden a los templos para la celebración de Cerealia, en honor a Ceres. Granjeros ofrecen sacrificios por ricas cosechas en los años venideros."},
    {TR_CITY_MESSAGE_TEXT_PANTHEON_FESTIVAL_NEPTUNE, "Hoy es el festival de Neptunalia, en honor a Neptune. Los fieles construyen chozas de ramas y follaje y festejan bajo el calor abrasador del verano"},
    {TR_CITY_MESSAGE_TEXT_PANTHEON_FESTIVAL_MERCURY, "Comerciantes y mercaderes salen en masa a celebrar Mercuralia. Agua sagrada es rociada en barcos y almacenes buscando la divina protección del dios Mercurio."},
    {TR_CITY_MESSAGE_TEXT_PANTHEON_FESTIVAL_MARS, "Ciudadanos se reunen en las afueras de la ciudad para la celebración de Equirria, buscando el favor de Marte. El aire se llena con el trueno de los cascos y el crujir de los carros mientras los fieles compiten en honor a su dios."},
    {TR_CITY_MESSAGE_TEXT_PANTHEON_FESTIVAL_VENUS, "Los fieles se reunen para la celebración de Veneralia, día consagrado Venus. Tus ciudadanos ruegan por buena fortuna en asuntos del corazón."},
    {TR_TOOLTIP_BUTTON_DELETE_READ_MESSAGES, "Borrar mensajes leídos"},
    {TR_TOOLTIP_BUTTON_MOTHBALL_ON, "Deshabilitar este edificio"},
    {TR_TOOLTIP_BUTTON_MOTHBALL_OFF, "Habilitar este edificio"},
    {TR_TOOLTIP_BUTTON_ACCEPT_MARKET_LADIES, "Permite a las mercaderes comprar aquí"},
    {TR_TOOLTIP_BUTTON_ACCEPT_TRADE_CARAVAN, "Permite a las caravanas comerciar aquí"},
    {TR_TOOLTIP_BUTTON_ACCEPT_TRADE_SHIPS, "Permite a los barcos de comercio comerciar aquí"},
    {TR_CONFIG_HEADER_CITY_MANAGEMENT_CHANGES, "Gestión de la ciudad"},
    {TR_BUILDING_LIGHTHOUSE, "Faro"},
    {TR_BUILDING_LIGHTHOUSE_PHASE_1, "(Cimientos)"},
    {TR_BUILDING_LIGHTHOUSE_PHASE_2, "(Pedestal)"},
    {TR_BUILDING_LIGHTHOUSE_PHASE_3, "(Torre)"},
    {TR_BUILDING_LIGHTHOUSE_PHASE_4, "(Cima)"},
    {TR_BUILDING_LIGHTHOUSE_PHASE_1_TEXT, "Los ingenieros sientan sólidos cimientos para soportar el peso de la gran torre de piedra."},
    {TR_BUILDING_LIGHTHOUSE_PHASE_2_TEXT, "Canteros construyen el pedestal que elevará la llama del faro por sobre el horizonte."},
    {TR_BUILDING_LIGHTHOUSE_PHASE_3_TEXT, "La torre del faro se eleva cada día más a medida que los canteros cuidadosamente llevan a cabo su labor."},
    {TR_BUILDING_LIGHTHOUSE_PHASE_4_TEXT, "Ingenieros realizan toques finales al faro. Pronto brillará por kilómetros, guiando a los barcos a su destino."},
    {TR_BUILDING_LIGHTHOUSE_CONSTRUCTION_DESC, "Construir el faro requiere material de un almacén, trabajadores de un campamento de trabajo e ingenieros de un gremio de ingenieros."},
    {TR_BUILDING_LIGHTHOUSE_BONUS_DESC, "Barcos pesqueros se mueven un 10% más rápido. Las tormentas marinas duran la mitad."},
    {TR_EDITOR_ALLOWED_BUILDINGS_MONUMENTS, "Monumentos"},
    {TR_CITY_MESSAGE_TEXT_LIGHTHOUSE_COMPLETE, "El faro, una enorme torre de piedra, se cierne sobre el horizonte. Que su baliza guíe a los barcos a su destino hasta el fin de los tiempos."},
    {TR_CITY_MESSAGE_TEXT_PANTHEON_COMPLETE, "El Panteón está completo. Se erige incomparable como monumento al asombroso poder de los dioses y del pueblo de Roma."},
    {TR_CITY_MESSAGE_TITLE_MONUMENT_COMPLETE, "Monumento finalizado"},
    {TR_CITY_MESSAGE_TITLE_NEPTUNE_BLESSING, "Bendición de Neptuno"},
    {TR_CITY_MESSAGE_TEXT_NEPTUNE_BLESSING, "Premiando la devoción de tu ciudad, Neptuno otorga viaje seguro a comerciantes por doce meses, durante los cuales tus exportaciones valdrán un 50% adicional."},
    {TR_CITY_MESSAGE_TITLE_VENUS_BLESSING, "Bendición de Venus"},
    {TR_CITY_MESSAGE_TEXT_VENUS_BLESSING, "Encantada por la alegre reverencia mostrada a ella, Venus otorga juventud, salud y felicidad a tu gente, aumentando la cantidad de tus trabajadores."},
    {TR_BUILDING_MENU_STATUES, "Estatuas"},
    {TR_BUILDING_MENU_GOV_RES, "Mansión del gobernador"},
    {TR_OVERLAY_ROADS, "Caminos"},
    {TR_NO_EXTRA_ASSETS_TITLE, "La carpeta de assets no está"},
    {TR_NO_EXTRA_ASSETS_MESSAGE,
        "Tu instalación de Caesar 3 no tiene bien instalada la carpeta de assets. "
        "Los nuevos elementos no se mostrarán correctamente.\n"
        "Asegúrate de que la carpeta '/assets' existe en el directorio de instalación de Caesar 3. "},
    {TR_WARNING_WATER_NEEDED_FOR_LIGHTHOUSE, "El faro debe ser construido cerca del agua."},
    {TR_TOOLTIP_OVERLAY_PANTHEON_ACCESS, "Esta casa tiene acceso a todos los dioses del Panteón"},
    {TR_BUILDING_LEGION_FOOD_BONUS, "La abundante comida aumenta la moral"},
    {TR_BUILDING_LEGION_FOOD_STATUS, "Estado nutricional"},
    {TR_TOOLTIP_BUTTON_ACCEPT_QUARTERMASTER, "Permite a los intendentes obtener alimentos aquí"},
    {TR_WARNING_RESOURCES_NOT_AVAILABLE, "No puedes obtener los materiales necesarios"},
    {TR_CONFIG_GP_CH_MONUMENTS_BOOST_CULTURE_RATING, "Grandes templos completados otorgan +8 de puntuación de cultura"},
    {TR_BUTTON_BACK_TO_MAIN_MENU, "Volver al menú principal"},
    {TR_LABEL_PAUSE_MENU, "Pausa"},
    {TR_OVERLAY_LEVY, "Mantenimiento"},
    {TR_TOOLTIP_OVERLAY_LEVY, " denarii pagados mensualmente para dar mantenimiento."}, 
    {TR_MAP_EDITOR_OPTIONS, "Configuración de escenario"},
    {TR_BUILDING_MARS_TEMPLE_MODULE_DESC_NO_MESS, "Este templo recolectará comida para tu puesto de suministro, una vez lo construyas."},
    {TR_BUTTON_GO_TO_SITE, "Ir al sitio" },
};

void translation_spanish(const translation_string **strings, int *num_strings)
{
    *strings = all_strings;
    *num_strings = sizeof(all_strings) / sizeof(translation_string);
}
