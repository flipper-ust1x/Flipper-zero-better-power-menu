#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/modules/menu.h>
#include <stdlib.h>
typedef struct {
    ViewDispatcher* view_dispatcher;
    Menu* menu;
    Gui* gui;
} PowerMenuApp;

static void power_menu_menu_callback(void* context, uint32_t index) {
    UNUSED(context);
    
    switch (index) {
        case 0:
            // Restart
            void furi_hal_speaker_start (float 300, float 1);
            furi_hal_power_reset();
            break;
        case 2:
            // Shutdown
            void furi_hal_speaker_start (float 300, float 1);
            furi_hal_power_off();
            break;
    }
}

int32_t power_menu_app(void* p) {
    UNUSED(p);
    
    PowerMenuApp* app = malloc(sizeof(PowerMenuApp));
    
    app->gui = furi_record_open(RECORD_GUI);
    
    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);
    
    app->menu = menu_alloc();
    menu_add_item(app->menu, "RESTART", NULL, 0, power_menu_menu_callback, app);
    menu_add_item(app->menu, "SHUTDOWN", NULL, 2, power_menu_menu_callback, app);
    
    view_dispatcher_add_view(app->view_dispatcher, 0, menu_get_view(app->menu));
    view_dispatcher_switch_to_view(app->view_dispatcher, 0);
    view_dispatcher_run(app->view_dispatcher);
    
    view_dispatcher_remove_view(app->view_dispatcher, 0);
    view_dispatcher_free(app->view_dispatcher);
    menu_free(app->menu);
    furi_record_close(RECORD_GUI);
    free(app);
    
    return 0;
}
