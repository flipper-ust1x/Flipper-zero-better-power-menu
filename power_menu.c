#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/view.h>
#include <input/input.h>
#include <assets_icons.h>
#include <stdlib.h>

typedef struct {
    ViewDispatcher* view_dispatcher;
    View* view;
    Gui* gui;

    uint8_t selected;
    uint8_t anim_offset;
} PowerMenuApp;

static void power_menu_draw_callback(Canvas* canvas, void* context) {
    PowerMenuApp* app = context;

    canvas_clear(canvas);

    // Header
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 28, 12, "SYSTEM POWER");

    // Divider
    canvas_draw_line(canvas, 8, 16, 120, 16);

    // Battery icon area
    canvas_draw_icon(canvas, 8, 2, &I_BatteryBody_52x28);

    // Restart Button
    uint8_t restart_y = 24;

    if(app->selected == 0) {
        canvas_draw_box(canvas, 6, restart_y, 116, 18);
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_icon(canvas, 12, restart_y + 1, &I_Restart_19x20);
        canvas_draw_str(canvas, 42, restart_y + 12, "Restart Device");
        canvas_set_color(canvas, ColorBlack);
    } else {
        canvas_draw_frame(canvas, 6, restart_y, 116, 18);
        canvas_draw_icon(canvas, 12, restart_y + 1, &I_Restart_19x20);
        canvas_draw_str(canvas, 42, restart_y + 12, "Restart Device");
    }

    // Shutdown Button
    uint8_t shutdown_y = 47;

    if(app->selected == 1) {
        canvas_draw_box(canvas, 6, shutdown_y, 116, 18);
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_icon(canvas, 12, shutdown_y + 1, &I_Power_25x27);
        canvas_draw_str(canvas, 42, shutdown_y + 12, "Shutdown");
        canvas_set_color(canvas, ColorBlack);
    } else {
        canvas_draw_frame(canvas, 6, shutdown_y, 116, 18);
        canvas_draw_icon(canvas, 12, shutdown_y + 1, &I_Power_25x27);
        canvas_draw_str(canvas, 42, shutdown_y + 12, "Shutdown");
    }

    // Footer hints
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 8, 62, "UP/DOWN Navigate");
    canvas_draw_str(canvas, 88, 62, "OK Select");
}

static bool power_menu_input_callback(InputEvent* event, void* context) {
    PowerMenuApp* app = context;

    if(event->type != InputTypePress) {
        return false;
    }

}
