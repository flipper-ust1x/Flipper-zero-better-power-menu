#include <furi.h>
#include <furi_hal.h>

#include <gui/gui.h>
#include <gui/view.h>
#include <gui/view_dispatcher.h>

#include <input/input.h>

#include <stdlib.h>

typedef struct {
    Gui* gui;
    ViewDispatcher* view_dispatcher;
    View* view;

    uint8_t selected;
} PowerMenuApp;

static void power_menu_draw(Canvas* canvas, void* context) {
    PowerMenuApp* app = context;

    canvas_clear(canvas);

    // ===== HEADER =====

    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 8, 12, "POWER CONTROL");

    canvas_draw_line(canvas, 0, 16, 127, 16);

    // ===== RESTART =====

    if(app->selected == 0) {
        canvas_draw_box(canvas, 4, 22, 120, 18);

        canvas_set_color(canvas, ColorWhite);
        canvas_draw_str(canvas, 12, 35, "> Restart Device");

        canvas_set_color(canvas, ColorBlack);
    } else {
        canvas_draw_frame(canvas, 4, 22, 120, 18);

        canvas_draw_str(canvas, 12, 35, "  Restart Device");
    }

    // ===== SHUTDOWN =====

    if(app->selected == 1) {
        canvas_draw_box(canvas, 4, 45, 120, 18);

        canvas_set_color(canvas, ColorWhite);
        canvas_draw_str(canvas, 12, 58, "> Shutdown");

        canvas_set_color(canvas, ColorBlack);
    } else {
        canvas_draw_frame(canvas, 4, 45, 120, 18);

        canvas_draw_str(canvas, 12, 58, "  Shutdown");
    }

    // ===== FOOTER =====

    canvas_set_font(canvas, FontSecondary);

    canvas_draw_str(canvas, 4, 64, "UP/DOWN Navigate");
}

static bool power_menu_input(InputEvent* event, void* context) {
    PowerMenuApp* app = context;

    if(event->type != InputTypePress) {
        return false;
    }

    switch(event->key) {
    case InputKeyUp:
        if(app->selected > 0) {
            app->selected--;
        }
        break;

    case InputKeyDown:
        if(app->selected < 1) {
            app->selected++;
        }
        break;

    case InputKeyOk:
        if(app->selected == 0) {
            furi_hal_power_reset();
        } else if(app->selected == 1) {
            furi_hal_power_off();
        }
        break;

    case InputKeyBack:
        view_dispatcher_stop(app->view_dispatcher);
        break;

    default:
        break;
    }

    view_dispatcher_switch_to_view(app->view_dispatcher, 0);

    return true;
}

int32_t power_menu_app(void* p) {
    UNUSED(p);

    PowerMenuApp* app = malloc(sizeof(PowerMenuApp));

    app->selected = 0;

    app->gui = furi_record_open(RECORD_GUI);

    app->view_dispatcher = view_dispatcher_alloc();

    view_dispatcher_attach_to_gui(
        app->view_dispatcher,
        app->gui,
        ViewDispatcherTypeFullscreen);

    app->view = view_alloc();

    view_set_context(app->view, app);

    view_set_draw_callback(app->view, power_menu_draw);

    view_set_input_callback(app->view, power_menu_input);

    view_dispatcher_add_view(app->view_dispatcher, 0, app->view);

    view_dispatcher_switch_to_view(app->view_dispatcher, 0);

    view_dispatcher_run(app->view_dispatcher);

    view_dispatcher_remove_view(app->view_dispatcher, 0);

    view_free(app->view);

    view_dispatcher_free(app->view_dispatcher);

    furi_record_close(RECORD_GUI);

    free(app);

    return 0;
}
