#include <pebble.h>

//---- Fields ----
static Window *s_mainWindow;


//---- Functions ----
static void loadMainWindow(Window *window){

}

static void unloadMainWindow(Window *window){

}

static void init() {

    s_mainWindow = window_create();
    window_set_window_handlers(s_mainWindow, (WindowHandlers) {

        .load = loadMainWindow,
        .unload = unloadMainWindow
    });
    window_stack_push(s_mainWindow);
}


static void deinit() {

    window_destroy(s_mainWindow);
}


int main(void){

  init();
  app_event_loop();
  deinit();
}
