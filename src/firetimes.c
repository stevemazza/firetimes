#include <pebble.h>

static Window *window;

static int Selected;
static TextLayer *tlSelected;
static TextLayer *textLayers[5];

static char prefixText[5][30] = {
  "Respond: ",
  "On Scene: ",
  "Stop: ",
  "Returning: ",
  "On Station: "
};

static char selectedText[5][30] = {
  "Selected: RESPOND",
  "Selected: ON SCENE",
  "Selected: STOP",
  "Selected: RETURNING",
  "Selected:ON STATION"
};

static char textLayerText[5][40];

static void select_multi_click_handler(ClickRecognizerRef recognizer, void *context) {
  for (int i=0; i<5; i++) {
    text_layer_set_text(textLayers[i], prefixText[i]);
  }
  text_layer_set_text(tlSelected, selectedText[0]);

}

static void long_click_up_handler(ClickRecognizerRef recognizer, void *context) {
}

static void long_click_handler(ClickRecognizerRef recognizer, void *context) {
struct tm *t;
time_t temp;
static char CurrentTime[6];
  
  textLayerText[Selected][0] = '\0';
  temp = time(NULL);
  t = localtime(&temp);
  strftime(CurrentTime, 6, "%H:%M", t);

  strncpy(textLayerText[Selected], prefixText[Selected], sizeof(textLayerText[Selected]));
  strcat(textLayerText[Selected], CurrentTime);
  text_layer_set_text(textLayers[Selected], textLayerText[Selected]);
  Selected = (Selected+1)%5;
  text_layer_set_text(tlSelected, selectedText[Selected]);
  vibes_double_pulse();
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  Selected = (Selected+4)%5;
  text_layer_set_text(tlSelected, selectedText[Selected]);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  Selected = (Selected+1)%5;
  text_layer_set_text(tlSelected, selectedText[Selected]);
}

static void click_config_provider(void *context) {
  window_long_click_subscribe(BUTTON_ID_SELECT, 700, long_click_handler, long_click_up_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  window_multi_click_subscribe(BUTTON_ID_SELECT, 3, 0, 0, false, select_multi_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  Selected = 0;

  tlSelected = text_layer_create((GRect) { .origin = { 0, 10 }, .size = { bounds.size.w, 23 } });
  text_layer_set_text(tlSelected, selectedText[0]);
  text_layer_set_font(tlSelected, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(tlSelected, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(tlSelected));

  for (int i=0; i<5; i++) {
    textLayers[i] = text_layer_create((GRect) { .origin = { 0, 30+23*i }, .size = { bounds.size.w, 23 } });
    text_layer_set_text(textLayers[i], prefixText[i]);
    text_layer_set_text_alignment(textLayers[i], GTextAlignmentLeft);
    text_layer_set_font(textLayers[i], fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
    layer_add_child(window_layer, text_layer_get_layer(textLayers[i]));
  }
}

static void window_unload(Window *window) {
  text_layer_destroy(tlSelected);
  for (int i=0; i<5; i++) {
    text_layer_destroy(textLayers[i]);
  }
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}