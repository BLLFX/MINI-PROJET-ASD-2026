/*
 * app/fonts.c — chargement des polices embarquees dans l'executable.
 *
 * Pourquoi : le moteur de rendu de LCUI ne charge sinon que quelques
 * polices chinoises depuis C:\Windows\Fonts (msyh, simsun, consola).
 * Sur un Windows depourvu de ces fichiers (ou sous Wine), aucun glyphe
 * latin n'est disponible et tout le texte s'affiche en carres (tofu).
 * Les polices sont donc liees a l'exe (fonts.rc) et chargees ici
 * depuis la memoire via pd_font_library_load_data().
 */

#include "fonts.h"

#ifdef _WIN32

#include <windows.h>
#include <LCUI.h>
#include <LCUI/fonts.h>
#include <pandagl.h>
#include "../resource.h"

typedef struct embedded_font {
        int resource_id;
        const char *name; /* pour les messages de log uniquement */
} embedded_font_t;

static const embedded_font_t embedded_fonts[] = {
        { IDR_FONT_NOTO_SANS, "Noto Sans" },
        { IDR_FONT_NOTO_SANS_BOLD, "Noto Sans Bold" },
        { IDR_FONT_FLUENT_REGULAR, "FluentSystemIcons-Regular" },
        { IDR_FONT_FLUENT_FILLED, "FluentSystemIcons-Filled" },
};

static int load_font_resource(int resource_id, const char *name)
{
        HRSRC res;
        HGLOBAL handle;
        void *data;
        DWORD size;

        res = FindResourceA(NULL, MAKEINTRESOURCEA(resource_id), RT_RCDATA);
        if (!res) {
                return -1;
        }
        handle = LoadResource(NULL, res);
        if (!handle) {
                return -2;
        }
        data = LockResource(handle);
        size = SizeofResource(NULL, res);
        if (!data || size == 0) {
                return -3;
        }
        /* Le tampon pointe dans la section ressources de l'exe : il reste
         * valide toute la duree du processus, comme l'exige FreeType. */
        (void)name;
        return pd_font_library_load_data(data, (long)size);
}

void app_load_embedded_fonts(void)
{
        size_t i;
        int id;

        for (i = 0; i < sizeof(embedded_fonts) / sizeof(embedded_fonts[0]); ++i) {
                load_font_resource(embedded_fonts[i].resource_id,
                                   embedded_fonts[i].name);
        }

        /* Noto Sans devient la police par defaut : tous les textes de l'app
         * (y compris les accents francais) sont rendus quelle que soit la
         * configuration du systeme hote. */
        if (lcui_fonts_set_default("Noto Sans")) {
                pd_font_library_set_font_family_alias("sans-serif", "Noto Sans");
                pd_font_library_set_font_family_alias("system-ui", "Noto Sans");
                pd_font_library_set_font_family_alias("serif", "Noto Sans");
        }

        /* Les styles en gras doivent trouver la variante Bold. */
        id = pd_font_library_get_font_id("Noto Sans", PD_FONT_STYLE_NORMAL,
                                         PD_FONT_WEIGHT_BOLD);
        if (id < 0) {
                /* Repli : le moteur utilisera la variante Regular. */
                pd_font_library_set_font_family_alias("Noto Sans Bold",
                                                      "Noto Sans");
        }
}

#else

void app_load_embedded_fonts(void)
{
        /* Build Linux : fontconfig fournit deja les polices systeme. */
}

#endif
