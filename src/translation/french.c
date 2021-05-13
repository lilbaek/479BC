#include "translation/common.h"
#include "translation/translation.h"

static translation_string all_strings[] = {
    {TR_NO_PATCH_TITLE, "Patch 1.0.1.0 manquant"},
    {TR_NO_PATCH_MESSAGE,
        "Votre installation de Caesar 3 ne contient pas le patch 1.0.1.0. "
        "Vous pouvez le télécharger depuis : \n"
        URL_PATCHES "\n"
        "Procédez à vos propres risques."},
    {TR_MISSING_FONTS_TITLE, "Polices manquantes"},
    {TR_MISSING_FONTS_MESSAGE,
        "Votre installation de Caesar 3 requiert des polices d'écriture supplémentaires. "
        "Vous pouvez les télécharger pour votre langue depuis : \n"
        URL_PATCHES},
    {TR_NO_EDITOR_TITLE, "Éditeur manquant"},
    {TR_NO_EDITOR_MESSAGE,
        "Votre installation de Cesar 3 ne contient pas le fichier éditeur. "
        "Vous pouvez le télécharger depuis : \n"
        URL_EDITOR},
    {TR_INVALID_LANGUAGE_TITLE, "Répertoire de langue invalide"},
    {TR_INVALID_LANGUAGE_MESSAGE,
        "Le répertoire que vous avez sélectionné ne contient pas de pack de langue valide. "
        "Veuillez consulter les erreurs dans le fichier de log."},
    {TR_BUILD_ALL_TEMPLES, "Tout"},
    {TR_BUTTON_OK, "OK"},
    {TR_BUTTON_CANCEL, "Annuler"},
    {TR_BUTTON_RESET_DEFAULTS, "Réglages par défaut"},
    {TR_BUTTON_CONFIGURE_HOTKEYS, "Contrôles du clavier"},
    {TR_CONFIG_TITLE, "Options de configuration de Julius"},
    {TR_CONFIG_LANGUAGE_LABEL, "Langue :"},
    {TR_CONFIG_LANGUAGE_DEFAULT, "(par défaut)"},
    {TR_CONFIG_DISPLAY_SCALE, "Mise à l'échelle de l'affichage :"},
    {TR_CONFIG_CURSOR_SCALE, "Taille du curseur de souris :"},
    {TR_CONFIG_HEADER_UI_CHANGES, "Configuration de l'interface"},
    {TR_CONFIG_HEADER_GAMEPLAY_CHANGES, "Configuration du jeu"},
    {TR_CONFIG_SHOW_INTRO_VIDEO, "Jouer la vidéo d'introduction"},
    {TR_CONFIG_SIDEBAR_INFO, "Informations supplémentaires dans le panneau latéral"},
    {TR_CONFIG_SMOOTH_SCROLLING, "Activer le défilement doux"},
    {TR_CONFIG_DISABLE_MOUSE_EDGE_SCROLLING, "Désactiver le défilement de la carte sur le bord de la fenêtre"},
    {TR_CONFIG_DISABLE_RIGHT_CLICK_MAP_DRAG, "Désactiver le clic droit pour déplacer la vue de la cité"},
    {TR_CONFIG_VISUAL_FEEDBACK_ON_DELETE, "Améliore le retour visuel durant le dégagement du terrain"},
    {TR_CONFIG_ALLOW_CYCLING_TEMPLES, "Permet de construire tous les temples successivement"},
    {TR_CONFIG_SHOW_WATER_STRUCTURE_RANGE, "Voir la zone d'effet en plaçant les bâtiments de distribution d'eau"},
    {TR_CONFIG_SHOW_CONSTRUCTION_SIZE, "Voir la taille des constructions durant le glissement de la souris"},
    {TR_CONFIG_HIGHLIGHT_LEGIONS, "Mettre en surbrillance les légions au survol du curseur"},
    {TR_CONFIG_SHOW_MILITARY_SIDEBAR, "Activer la gestion militaire dans le panneau latéral"},
    {TR_CONFIG_FIX_IMMIGRATION_BUG, "Corrige le bug d'immigration en mode très difficile"},
    {TR_CONFIG_FIX_100_YEAR_GHOSTS, "Corrige le bug des fantômes de 100 ans"},
    {TR_HOTKEY_TITLE, "Configuration du clavier"},
    {TR_HOTKEY_LABEL, "Touche"},
    {TR_HOTKEY_ALTERNATIVE_LABEL, "Alternative"},
    {TR_HOTKEY_HEADER_ARROWS, "Flèches directionnelles"},
    {TR_HOTKEY_HEADER_GLOBAL, "Contrôles globaux"},
    {TR_HOTKEY_HEADER_CITY, "Contrôles de la cité"},
    {TR_HOTKEY_HEADER_ADVISORS, "Rapports des conseillers"},
    {TR_HOTKEY_HEADER_OVERLAYS, "Cartes de visualisation"},
    {TR_HOTKEY_HEADER_BOOKMARKS, "Points d'intérêt de la cité"},
    {TR_HOTKEY_HEADER_EDITOR, "Éditeur"},
    {TR_HOTKEY_HEADER_BUILD, "Contrôles de construction"},
    {TR_HOTKEY_ARROW_UP, "Haut"},
    {TR_HOTKEY_ARROW_DOWN, "Bas"},
    {TR_HOTKEY_ARROW_LEFT, "Gauche"},
    {TR_HOTKEY_ARROW_RIGHT, "Droite"},
    {TR_HOTKEY_TOGGLE_FULLSCREEN, "Basculer en plein écran"},
    {TR_HOTKEY_CENTER_WINDOW, "Centrer la fenêtre"},
    {TR_HOTKEY_RESIZE_TO_640, "Redimensionner en 640x480"},
    {TR_HOTKEY_RESIZE_TO_800, "Redimensionner en 800x600"},
    {TR_HOTKEY_RESIZE_TO_1024, "Redimensionner en 1024x768"},
    {TR_HOTKEY_SAVE_SCREENSHOT, "Capturer l'écran de jeu"},
    {TR_HOTKEY_SAVE_CITY_SCREENSHOT, "Capturer la cité entière"},
    {TR_HOTKEY_BUILD_CLONE, "Cloner le bâtiment sous le curseur"},
    {TR_HOTKEY_LOAD_FILE, "Charger une partie"},
    {TR_HOTKEY_SAVE_FILE, "Sauvegarder une partie"},
    {TR_HOTKEY_INCREASE_GAME_SPEED, "Augmenter la vitesse du jeu"},
    {TR_HOTKEY_DECREASE_GAME_SPEED, "Diminuer la vitesse du jeu"},
    {TR_HOTKEY_TOGGLE_PAUSE, "Activer la pause"},
    {TR_HOTKEY_CYCLE_LEGION, "Parcourir les légions en boucle"},
    {TR_HOTKEY_ROTATE_MAP_LEFT, "Tourner la carte vers la gauche"},
    {TR_HOTKEY_ROTATE_MAP_RIGHT, "Tourner la carte vers la droite"},
    {TR_HOTKEY_SHOW_ADVISOR_LABOR, "Main-d'œuvre"},
    {TR_HOTKEY_SHOW_ADVISOR_MILITARY, "Légion"},
    {TR_HOTKEY_SHOW_ADVISOR_IMPERIAL, "Empereur"},
    {TR_HOTKEY_SHOW_ADVISOR_RATINGS, "Évaluation"},
    {TR_HOTKEY_SHOW_ADVISOR_TRADE, "Commerce"},
    {TR_HOTKEY_SHOW_ADVISOR_POPULATION, "Population"},
    {TR_HOTKEY_SHOW_ADVISOR_HEALTH, "Hygiène"},
    {TR_HOTKEY_SHOW_ADVISOR_EDUCATION, "Éducation"},
    {TR_HOTKEY_SHOW_ADVISOR_ENTERTAINMENT, "Loisirs"},
    {TR_HOTKEY_SHOW_ADVISOR_RELIGION, "Religion"},
    {TR_HOTKEY_SHOW_ADVISOR_FINANCIAL, "Finances"},
    {TR_HOTKEY_SHOW_ADVISOR_CHIEF, "Conseiller personnel"},
    {TR_HOTKEY_TOGGLE_OVERLAY, "Basculer sur la carte précédente"},
    {TR_HOTKEY_SHOW_OVERLAY_WATER, "Eau"},
    {TR_HOTKEY_SHOW_OVERLAY_FIRE, "Incendies"},
    {TR_HOTKEY_SHOW_OVERLAY_DAMAGE, "Dégâts"},
    {TR_HOTKEY_SHOW_OVERLAY_CRIME, "Criminalité"},
    {TR_HOTKEY_SHOW_OVERLAY_PROBLEMS, "Problèmes"},
    {TR_HOTKEY_GO_TO_BOOKMARK_1, "Aller au point d'intérêt 1"},
    {TR_HOTKEY_GO_TO_BOOKMARK_2, "Aller au point d'intérêt 2"},
    {TR_HOTKEY_GO_TO_BOOKMARK_3, "Aller au point d'intérêt 3"},
    {TR_HOTKEY_GO_TO_BOOKMARK_4, "Aller au point d'intérêt 4"},
    {TR_HOTKEY_SET_BOOKMARK_1, "Définir comme point d'intérêt 1"},
    {TR_HOTKEY_SET_BOOKMARK_2, "Définir comme point d'intérêt 2"},
    {TR_HOTKEY_SET_BOOKMARK_3, "Définir comme point d'intérêt 3"},
    {TR_HOTKEY_SET_BOOKMARK_4, "Définir comme point d'intérêt 4"},
    {TR_HOTKEY_EDITOR_TOGGLE_BATTLE_INFO, "Afficher les infos de bataille"},
    {TR_HOTKEY_EDIT_TITLE, "Appuyez sur une touche"},
    {TR_HOTKEY_DUPLICATE_TITLE, "Touche de raccourci déjà utilisée"},
    {TR_HOTKEY_DUPLICATE_MESSAGE, "Cette combinaison de touches est déjà assignée à l'action suivante :"}
};

void translation_french(const translation_string **strings, int *num_strings)
{
    *strings = all_strings;
    *num_strings = sizeof(all_strings) / sizeof(translation_string);
}
