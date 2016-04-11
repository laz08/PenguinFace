#include <pebble.h>

//---- Fields ----
static Window *sMainWindow;
static TextLayer *sTimeLayer;


//---- Functions ----

static void updateTime() {

    time_t tempTime = time(NULL);
    struct tm *tick_time = localtime(&tempTime);

    //Static, thus making it referenced and persistent across multiple calls to
    //updateTime function.
    //This is to make the time to be long-lived as long as it is to be displayed.
    static char sBuffer[8];
    strftime(sBuffer, sizeof(sBuffer), clock_is_24h_style() ?
                                        "%H:%M" : "%I:%M", tick_time);
    text_layer_set_text(sTimeLayer, sBuffer);
}

static void tickHandler(struct tm *tickTime, TimeUnits unitsChanged){

    updateTime();
}



//Windows
static void loadMainWindow(Window *window){

    //Get window info.
    Layer *windowLayer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(windowLayer);

    //Setup time layer bounds
    sTimeLayer = text_layer_create(
        GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50)
    );

    //Setup time layer contents
    text_layer_set_background_color(sTimeLayer, GColorClear);
    text_layer_set_text_color(sTimeLayer, GColorBlack);
    text_layer_set_font(sTimeLayer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
    text_layer_set_text_alignment(sTimeLayer, GTextAlignmentCenter);

    //Add to the window as a child
    layer_add_child(windowLayer, text_layer_get_layer(sTimeLayer));
}

static void unloadMainWindow(Window *window){

    text_layer_destroy(sTimeLayer);
}


//Init, deinit, main
static void init() {

    sMainWindow = window_create();
    window_set_window_handlers(sMainWindow, (WindowHandlers) {

        .load = loadMainWindow,
        .unload = unloadMainWindow
    });
    window_stack_push(sMainWindow, true);

    updateTime();

    //Register with TickTimerService
    tick_timer_service_subscribe(MINUTE_UNIT, tickHandler);
}


static void deinit() {

    window_destroy(sMainWindow);
}


int main(void){

  init();
  app_event_loop();
  deinit();
}
