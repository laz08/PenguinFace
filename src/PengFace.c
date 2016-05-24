#include <pebble.h>

//---- Fields ----
static Window *sMainWindow;
static TextLayer *sTimeLayer;

static GFont sTimeFont;
static BitmapLayer *sBgLayer;
static GBitmap *sBgBitmap;


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
        GRect(0, PBL_IF_ROUND_ELSE(14, 10), bounds.size.w, 50)
    );


    //Setup time layer contents
    text_layer_set_background_color(sTimeLayer, GColorClear);
    text_layer_set_text_color(sTimeLayer, GColorWhite);

    //Load font
    sTimeFont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_OBELIXPRO_22));
    text_layer_set_font(sTimeLayer, sTimeFont);

    text_layer_set_text_alignment(sTimeLayer, GTextAlignmentCenter);


    //Load background
    sBgBitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PENG_BACKGROUND);
    sBgLayer = bitmap_layer_create(bounds);

    bitmap_layer_set_bitmap(sBgLayer, sBgBitmap);
    layer_add_child(windowLayer, bitmap_layer_get_layer(sBgLayer));
    layer_add_child(windowLayer, text_layer_get_layer(sTimeLayer));
}

static void unloadMainWindow(Window *window){

    text_layer_destroy(sTimeLayer);
    fonts_unload_custom_font(sTimeFont);
    gbitmap_destroy(sBgBitmap);
    bitmap_layer_destroy(sBgLayer);
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
