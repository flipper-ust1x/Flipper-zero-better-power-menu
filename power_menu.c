#include <furi.h>
#include <furi_hal.h>

#include <gui/gui.h>
#include <gui/view.h>
#include <gui/view_dispatcher.h>

#include <input/input.h>

#include <notification/notification_messages.h>

#include <stdlib.h>

typedef struct {
    Gui* gui;
    ViewDispatcher* view_dispatcher;
    View* view;

    NotificationApp* notifications;

    uint8_t selected;
} PowerMenuApp;

/* ===================== SOUND ===================== */

static void play_click(NotificationApp* n) {
    if(n) notification_message(n, &sequence_click);
}

static void play_confirm(NotificationApp* n) {
    if(n) notification_message(n, &sequence_success);
}

/* ===================== BATTERY ===================== */

static void draw_battery(Canvas* canvas) {
    uint8_t level = furi_hal_power_get_battery_remaining_capacity();

    if(level > 100) level = 100;

    canvas_draw_frame(canvas, 90, 2, 34, 8);

    uint8_t fill = (level * 32) / 100;
    canvas_draw_box(canvas, 91, 3, fill, 6);
}

/* ===================== DRAW ===================== */

static void power_menu_draw(Canvas* canvas, void* context) {
    PowerMenuApp* app = context;

    if(!app || !canvas) return;

    canvas_clear(canvas);

    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 8, 12, "POWER CONTROL");

    draw_battery(canvas);

    canvas_draw_line(canvas, 0, 16, 127, 16);

    /* ---------- RESTART ---------- */

    if(app->selected == 0) {
        canvas_draw_box(canvas, 4, 22, 120, 18);
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_str(canvas, 12, 35, "> Restart");
        canvas_set_color(canvas, ColorBlack);
    } else {
        canvas_draw_frame(canvas, 4, 22, 120, 18);
        canvas_draw_str(canvas, 12, 35, "  Restart");
    }

    /* ---------- SHUTDOWN ---------- */

    if(app->selected == 1) {
        canvas_draw_box(canvas, 4, 45, 120, 18);
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_str(canvas, 12, 58, "> Shutdown");
        canvas_set_color(canvas, ColorBlack);
    } else {
        canvas_draw_frame(canvas, 4, 45, 120, 18);
        canvas_draw_str(canvas, 12, 58, "  Shutdown");
    }

    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 4, 64, "OK=Select  BACK=Exit");
}

/* ===================== INPUT ===================== */

static bool power_menu_input(InputEvent* event, void* context) {
    PowerMenuApp* app = context;

    if(!app || event->type != InputTypePress) {
        return false;
    }

    switch(event->key) {

    case InputKeyUp:
        if(app->selected > 0) app->selected--;
        play_click(app->notifications);
        break;

    case InputKeyDown:
        if(app->selected < 1) app->selected++;
        play_click(app->notifications);
        break;

    case InputKeyOk:
        play_confirm(app->notifications);

        if(app->selected == 0) {
            furi_hal_power_reset();
        } else {
            furi_hal_power_off();
        }
        break;

    case InputKeyBack:
        view_dispatcher_stop(app->view_dispatcher);
        break;

    default:
        break;
    }

    view_port_update(view_get_view_port(app->view));
    return true;
}

/* ===================== APP ENTRY ===================== */

int32_t power_menu_app(void* p) {
    UNUSED(p);

    PowerMenuApp* app = malloc(sizeof(PowerMenuApp));

    app->selected = 0;

    app->gui = furi_record_open(RECORD_GUI);
    app->notifications = furi_record_open(RECORD_NOTIFICATION);

    app->view_dispatcher = view_dispatcher_alloc();

    view_dispatcher_attach_to_gui(
        app->view_dispatcher,
        app->gui,
        ViewDispatcherTypeFullscreen);

    view_dispatcher_enable_queue(app->view_dispatcher);

    app->view = view_alloc();

    view_set_context(app->view, app);
    view_set_draw_callback(app->view, power_menu_draw);
    view_set_input_callback(app->view, power_menu_input);

    view_dispatcher_add_view(app->view_dispatcher, 0, app->view);
    view_dispatcher_switch_to_view(app->view_dispatcher, 0);

    view_dispatcher_run(app->view_dispatcher);

    /* ===================== CLEANUP ===================== */

    view_dispatcher_remove_view(app->view_dispatcher, 0);
    view_free(app->view);
    view_dispatcher_free(app->view_dispatcher);

    furi_record_close(RECORD_GUI);
    furi_record_close(RECORD_NOTIFICATION);

    free(app);

    return 0;
}
