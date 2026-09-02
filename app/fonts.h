#ifndef APP_FONTS_H
#define APP_FONTS_H

/**
 * Charge les polices embarquees dans l'executable (ressources RCDATA) :
 *  - Noto Sans Regular / Bold  -> police de texte par defaut
 *  - FluentSystemIcons         -> icones de l'interface
 * puis regle Noto Sans comme police par defaut de LCUI.
 *
 * Sans effet hors Windows (le build Linux s'appuie sur fontconfig).
 * A appeler apres lcui_init(), avant la creation des widgets.
 */
void app_load_embedded_fonts(void);

#endif
