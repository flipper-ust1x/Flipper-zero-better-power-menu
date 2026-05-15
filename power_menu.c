#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <gui/elements.h>
#include <gui/view.h>
#include <input/input.h>
#include <stdlib.h>

// Application state
typedef enum {
    PowerMenuItemRestart,
    PowerMenuItemRestartDFU,
    PowerMenuItemShutdown,
    PowerMenuItemCount,
} PowerMenuItem;

typedef enum {
    PowerMenuStateMenu,
    PowerMenuStateConfirmRestart,
    PowerMenuStateConfirmRestartDFU,
    PowerMenuStateConfirmShutdown,
} PowerMenuState;

typedef struct {
    PowerMenuState state;
    PowerMenuItem selected_item;
    View* view;
    ViewDispatcher* view_dispatcher;
    Gui* gui;
} PowerMenuApp;

// Menu item strings
static const char* menu_items[PowerMenuItemCount] = {
    "Restart",
    "Restart in DFU",
    "Shutdown",
};

// Draw callback for menu
static void power_menu_draw_menu(Canvas* canvas, PowerMenuApp* app) {
    canvas_clear(canvas);
    
    // Header
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 2, AlignCenter, AlignTop, "Power Menu");
    
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_line(canvas, 0, 15, 128, 15);
    
    // Menu items
    canvas_set_font(canvas, FontSecondary);
    for (int i = 0; i < PowerMenuItemCount; i++) {
        int y = 22 + (i * 18);
        
        if (i == app->selected_item) {
            // Draw selection box
            canvas_draw_box(canvas, 5, y - 2, 118, 16);
            canvas_set_color(canvas, ColorWhite);
            canvas_draw_str(canvas, 10, y + 10, menu_items[i]);
            canvas_set_color(canvas, ColorBlack);
        } else {
            canvas_draw_str(canvas, 10, y + 10, menu_items[i]);
        }
    }
    
    // Footer
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, 2, 62, AlignLeft, AlignBottom, "UP/DOWN");
    canvas_draw_str_aligned(canvas, 126, 62, AlignRight, AlignBottom, "OK");
}

// Draw callback for confirmation dialogs
static void power_menu_draw_confirm(Canvas* canvas, PowerMenuApp* app, const char* title, const char* message) {
    canvas_clear(canvas);
    
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 10, AlignCenter, AlignTop, title);
    
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_line(canvas, 0, 24, 128, 24);
    
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, 64, 35, AlignCenter, AlignCenter, message);
    
    canvas_draw_str_aligned(canvas, 2, 62, AlignLeft, AlignBottom, "NO");
    canvas_draw_str_aligned(canvas, 126, 62, AlignRight, AlignBottom, "YES");
}

// Main draw callback
static void power_menu_draw_callback(Canvas* canvas, void* ctx) {
    PowerMenuApp* app = (PowerMenuApp*)ctx;
    
    switch (app->state) {
        case PowerMenuStateMenu:
            power_menu_draw_menu(canvas, app);
            break;
        case PowerMenuStateConfirmRestart:
            power_menu_draw_confirm(canvas, app, "Restart?", "Are you sure?");
            break;
        case PowerMenuStateConfirmRestartDFU:
            power_menu_draw_confirm(canvas, app, "Restart in DFU?", "Boot into bootloader?");
            break;
        case PowerMenuStateConfirmShutdown:
            power_menu_draw_confirm(canvas, app, "Shutdown?", "Power off Flipper?");
            break;
    }
}

// Input callback
static void power_menu_input_callback(InputEvent* input_event, void* ctx) {
    furi_assert(ctx);
    PowerMenuApp* app = (PowerMenuApp*)ctx;
    
    if (input_event->type != InputTypePress) {
        return;
    }
    
    if (app->state == PowerMenuStateMenu) {
        // Menu navigation
        if (input_event->key == InputKeyUp) {
            if (app->selected_item > 0) {
                app->selected_item--;
            }
        } else if (input_event->key == InputKeyDown) {
            if (app->selected_item < PowerMenuItemCount - 1) {
                app->selected_item++;
            }
        } else if (input_event->key == InputKeyOk) {
            // Enter confirmation state based on selection
            if (app->selected_item == PowerMenuItemRestart) {
                app->state = PowerMenuStateConfirmRestart;
            } else if (app->selected_item == PowerMenuItemRestartDFU) {
                app->state = PowerMenuStateConfirmRestartDFU;
            } else if (app->selected_item == PowerMenuItemShutdown) {
                app->state = PowerMenuStateConfirmShutdown;
            }
        } else if (input_event->key == InputKeyBack) {
            // Exit app
            view_dispatcher_stop(app->view_dispatcher);
        }
    } else {
        // Confirmation states
        if (input_event->key == InputKeyLeft || input_event->key == InputKeyBack) {
            // Cancel - go back to menu
            app->state = PowerMenuStateMenu;
        } else if (input_event->key == InputKeyRight || input_event->key == InputKeyOk) {
            // Confirm action
            if (app->state == PowerMenuStateConfirmRestart) {
                furi_hal_power_reset();
            } else if (app->state == PowerMenuStateConfirmRestartDFU) {
                furi_hal_bootloader_go_to_dfu_mode();
                furi_hal_power_reset();
            } else if (app->state == PowerMenuStateConfirmShutdown) {
                furi_hal_power_off();
            }
        }
    }
}

// App entry point
int32_t power_menu_app(void* p) {
    UNUSED(p);
    
    PowerMenuApp* app = malloc(sizeof(PowerMenuApp));
    app->state = PowerMenuStateMenu;
    app->selected_item = PowerMenuItemRestart;
    
    // Open GUI
    app->gui = furi_record_open(RECORD_GUI);
    
    // Create view and view dispatcher
    app->view = view_alloc();
    view_set_draw_callback(app->view, power_menu_draw_callback);
    view_set_input_callback(app->view, power_menu_input_callback);
    view_set_context(app->view, app);
    
    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_add_view(app->view_dispatcher, 0, app->view);
    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);
    view_dispatcher_switch_to_view(app->view_dispatcher, 0);
    
    // Run the app
    view_dispatcher_run(app->view_dispatcher);
    
    // Cleanup
    view_dispatcher_remove_view(app->view_dispatcher, 0);
    view_dispatcher_free(app->view_dispatcher);
    view_free(app->view);
    furi_record_close(RECORD_GUI);
    free(app);
    
    return 0;
}
