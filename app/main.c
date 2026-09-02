#include "main.h"
#include "backend.h"
#include "fonts.h"

int main(int argc, char *argv[])
{
        app_init();

        // Polices embarquees dans l'executable (Windows) : sans elles,
        // le texte et les icones s'affichent en carres.
        app_load_embedded_fonts();

        // Titre de la fenetre principale
        ui_widget_set_title(ui_root(), L"Velectro");

        // Taille de la fenetre principale
        ui_widget_resize(ui_root(), 1280, 800);

        // Pointe la couche metier (src/core) vers les fichiers .dat
        backend_init();

        // Get app router and route to the root path "/", This means that
        // your app will present the user interface in app/page.tsx
        router_t *router = router_get_by_name("AppRouter");
        router_location_t *location = router_location_create(NULL, "/candidats");
        router_push(router, location);
        router_location_destroy(location);

        // Write code here to initialize your application,
        // such as loading configuration files, initializing functional modules
        // ...

        return app_run();
}
