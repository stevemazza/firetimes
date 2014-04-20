#include <pebble.h>

static Window *window;
static TextLayer *tlSelected;
static TextLayer *tlRespond;
static TextLayer *tlOnScene;
static TextLayer *tlStop;
static TextLayer *tlReturning;
static TextLayer *tlOnStation;
static int Selected;
static char *RespondText = "Respond: ";
static char *OnSceneText = "On Scene: ";
static char *StopText = "Stop: ";
static char *ReturningText = "Returning: ";
static char *OnStationText = "On Station: ";

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
struct tm *t;
time_t temp;
char TempTime[6];
const char *CurrentTime;
char *TextBuffer;
  
  TextBuffer = "";
  temp = time(NULL);
  t = localtime(&temp);
  strftime(TempTime, 6, "%H:%M", t);
  CurrentTime = TempTime;
  switch(Selected) {
    case 0: {
      strncpy(TextBuffer, RespondText, 20);
      strcat(TextBuffer, CurrentTime);
      text_layer_set_text(tlRespond, TextBuffer);
      break;
    }
    case 1: {
      strncpy(TextBuffer, OnSceneText, 20);
      strcat(TextBuffer, CurrentTime);
      text_layer_set_text(tlOnScene, TextBuffer);
      break;
    }
    case 2: {
      strncpy(TextBuffer, StopText, 20);
      strcat(TextBuffer, CurrentTime);
      text_layer_set_text(tlStop, TextBuffer);
      break;
    }
    case 3: {
      strncpy(TextBuffer, ReturningText, 20);
      strcat(TextBuffer, CurrentTime);
      text_layer_set_text(tlReturning, TextBuffer);
      break;
    }
    case 4: {
      strncpy(TextBuffer, OnStationText, 20);
      strcat(TextBuffer, CurrentTime);
      text_layer_set_text(tlOnStation, TextBuffer);
      break;
    }
  }
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  switch(Selected) {
    case 0: { //Respond is selected
      text_layer_set_text(tlSelected, "Selected: ON STATION");
      Selected = 4;
      break;
    }
    case 1: {
      text_layer_set_text(tlSelected, "Selected: RESPOND");
      Selected = 0;
      break;
    }
    case 2: {
      text_layer_set_text(tlSelected, "Selected: ON SCENE");
      Selected = 1;
      break;
    }
    case 3: {
      text_layer_set_text(tlSelected, "Selected: STOP");
      Selected = 2;
      break;
    }
    case 4: {
      text_layer_set_text(tlSelected, "Selected: RETURNING");
      Selected = 3;
      break;
    }
  }
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  switch(Selected) {
    case 0: { //Respond is selected
      text_layer_set_text(tlSelected, "Selected: ON SCENE");
      Selected = 1;
      break;
    }
    case 1: {
      text_layer_set_text(tlSelected, "Selected: STOP");
      Selected = 2;
      break;
    }
    case 2: {
      text_layer_set_text(tlSelected, "Selected: RETURNING");
      Selected = 3;
      break;
    }
    case 3: {
      text_layer_set_text(tlSelected, "Selected: ON STATION");
      Selected = 4;
      break;
    }
    case 4: {
      text_layer_set_text(tlSelected, "Selected: RESPOND");
      Selected = 0;
      break;
    }
  }
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  tlSelected = text_layer_create((GRect) { .origin = { 0, 10 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(tlSelected, "Selected: RESPOND");
  Selected = 0;
  text_layer_set_text_alignment(tlSelected, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(tlSelected));

  tlRespond = text_layer_create((GRect) { .origin = { 0, 30 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(tlRespond, "Respond:");
  text_layer_set_text_alignment(tlRespond, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(tlRespond));

  tlOnScene = text_layer_create((GRect) { .origin = { 0, 50 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(tlOnScene, "On Scene:");
  text_layer_set_text_alignment(tlOnScene, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(tlOnScene));

  tlStop = text_layer_create((GRect) { .origin = { 0, 70 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(tlStop, "Stop:");
  text_layer_set_text_alignment(tlStop, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(tlStop));

  tlReturning = text_layer_create((GRect) { .origin = { 0, 90 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(tlReturning, "Returning:");
  text_layer_set_text_alignment(tlReturning, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(tlReturning));

  tlOnStation = text_layer_create((GRect) { .origin = { 0, 110 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(tlOnStation, "On Station:");
  text_layer_set_text_alignment(tlOnStation, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(tlOnStation));
}

static void window_unload(Window *window) {
  text_layer_destroy(tlSelected);
  text_layer_destroy(tlRespond);
  text_layer_destroy(tlOnScene);
  text_layer_destroy(tlStop);
  text_layer_destroy(tlReturning);
  text_layer_destroy(tlOnStation);
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