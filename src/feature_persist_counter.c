// Allen, Karthik, Brendon 
#include "pebble.h"
// #include "autoconfig.h"

// This is a custom defined key for saving our count field
#define NUM_DRINKS_PKEY 1

// You can define defaults for values in persistent storage
#define NUM_DRINKS_DEFAULT 0

static Window *window;

static GBitmap *action_icon_plus;
static GBitmap *action_icon_minus;

static ActionBarLayer *action_bar;

static TextLayer *header_text_layer;
static TextLayer *body_text_layer;
static TextLayer *label_text_layer;

// We'll save the count in memory from persistent storage
static int num_drinks = NUM_DRINKS_DEFAULT;

static void update_text() {
  static char body_text[50];
  snprintf(body_text, sizeof(body_text), "%u Drinks", num_drinks);
  text_layer_set_text(body_text_layer, body_text);
}

static void increment_click_handler(ClickRecognizerRef recognizer, void *context) {
  num_drinks++;
  update_text();
  vibes_double_pulse();
}

static void decrement_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (num_drinks <= 0) {
    // Keep the counter at zero
    return;
  }

  num_drinks--;
  update_text();
}

static void click_config_provider(void *context) {
  //const uint16_t repeat_interval_ms = 50;
  window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler) increment_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler) decrement_click_handler);
}

static void window_load(Window *me) {
  action_bar = action_bar_layer_create();
  action_bar_layer_add_to_window(action_bar, me);
  action_bar_layer_set_click_config_provider(action_bar, click_config_provider);

  action_bar_layer_set_icon(action_bar, BUTTON_ID_UP, action_icon_plus);
  action_bar_layer_set_icon(action_bar, BUTTON_ID_DOWN, action_icon_minus);

  Layer *layer = window_get_root_layer(me);
  const int16_t width = layer_get_frame(layer).size.w - ACTION_BAR_WIDTH - 3;

  header_text_layer = text_layer_create(GRect(0, 4, width, 60));
  text_layer_set_font(header_text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_background_color(header_text_layer, GColorClear);
  text_layer_set_text_alignment(header_text_layer, GTextAlignmentLeft);
  layer_add_child(layer, text_layer_get_layer(header_text_layer));

  body_text_layer = text_layer_create(GRect(4, 44, width, 60));
  text_layer_set_font(body_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_background_color(body_text_layer, GColorClear);
  layer_add_child(layer, text_layer_get_layer(body_text_layer));

 
  
  
  label_text_layer = text_layer_create(GRect(4, 44 + 56, width, 60));
  text_layer_set_font(label_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_background_color(label_text_layer, GColorClear);
  text_layer_set_text(label_text_layer, "Dispay Timer: Last Drink");
  layer_add_child(layer, text_layer_get_layer(label_text_layer));

  update_text();
}

static void window_unload(Window *window) {
  text_layer_destroy(header_text_layer);
  text_layer_destroy(body_text_layer);
  text_layer_destroy(label_text_layer);

  action_bar_layer_destroy(action_bar);
}

void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
  //Allocate long-lived storage (required by TextLayer)
  static char buffer[] = "00:00";
  
  //Write the time to the buffer in a safe manner
  strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  
  //Set the TextLayer to display the buffer
  text_layer_set_text(header_text_layer, buffer);
}

static void init(void) {
  action_icon_plus = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ACTION_ICON_PLUS);
  action_icon_minus = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ACTION_ICON_MINUS);

 // autoconfig_init();
  window = window_create();
  window_set_fullscreen(window,true);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });

  // Get the count from persistent storage for use if it exists, otherwise use the default
  num_drinks = persist_exists(NUM_DRINKS_PKEY) ? persist_read_int(NUM_DRINKS_PKEY) : NUM_DRINKS_DEFAULT;
 tick_timer_service_subscribe(MINUTE_UNIT, (TickHandler)tick_handler);
  window_stack_push(window, true /* Animated */);
}

static void deinit(void) {
  // Save the count into persistent storage on app exit
  persist_write_int(NUM_DRINKS_PKEY, num_drinks);

  window_destroy(window);

 //  autoconfig_deinit();
  
  gbitmap_destroy(action_icon_plus);
  gbitmap_destroy(action_icon_minus);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
