#include <pebble.h>
#include "location_schedule.h"

#define FACE_W 200
#define FACE_H 228
#define STALE_SECONDS 3600
#define PERSIST_PALETTE 1
#define PERSIST_FONT 2
#define PERSIST_LOCATION 3
#define PERSIST_DATE 4

static Window *s_window;
static Layer *s_layer;
static int s_weather = -1; /* -1 unknown, 0 clear, 1 rain, 2 snow, 3 cloud */
static time_t s_updated_at;
static int s_palette;
static int s_font;
static int s_location;
static int s_date;

typedef struct {
  GColor sky, rain_sky, snow_sky, house, roof, window, door;
  GColor tree, trunk, road, road_edge, road_mark, courier;
} Palette;

static Palette colors(void) {
  if (s_palette == 1) return (Palette) {
    GColorFromHEX(0x69547F), GColorFromHEX(0x3E3C5E),
    GColorFromHEX(0x958AA5), GColorFromHEX(0xE5B7A5),
    GColorFromHEX(0x9C5268), GColorFromHEX(0x513E68),
    GColorFromHEX(0x75485A), GColorFromHEX(0x294C52),
    GColorFromHEX(0x513F42), GColorFromHEX(0x333747),
    GColorFromHEX(0xD9B9AD), GColorFromHEX(0xA99FA7),
    GColorFromHEX(0xFFBB70)
  };
  if (s_palette == 2) return (Palette) {
    GColorFromHEX(0x328B88), GColorFromHEX(0x436B73),
    GColorFromHEX(0x9ABFC1), GColorFromHEX(0xF3E7C4),
    GColorFromHEX(0xD27159), GColorFromHEX(0x52757A),
    GColorFromHEX(0x9E6E55), GColorFromHEX(0x316D49),
    GColorFromHEX(0x6B5841), GColorFromHEX(0x345355),
    GColorFromHEX(0xCADBD1), GColorFromHEX(0xB5C9BE),
    GColorFromHEX(0xEED567)
  };
  return (Palette) {
    GColorFromHEX(0x63A5CC), GColorFromHEX(0x52657B),
    GColorFromHEX(0xAABBCD), GColorFromHEX(0xF5DC9A),
    GColorFromHEX(0x9A4F3C), GColorFromHEX(0x44688A),
    GColorFromHEX(0x9A4F3C), GColorFromHEX(0x397D55),
    GColorFromHEX(0x6E4A38), GColorFromHEX(0x4F5762),
    GColorFromHEX(0xC6D3C4), GColorFromHEX(0xB6B9A8),
    GColorFromHEX(0xFFAA00)
  };
}

static void box(GContext *ctx, int x, int y, int w, int h, GColor color) {
  graphics_context_set_fill_color(ctx, color);
  graphics_fill_rect(ctx, GRect(x, y, w, h), 0, GCornerNone);
}

static void text(GContext *ctx, const char *value, const char *font,
                 GRect rect, GColor color, GTextAlignment align) {
  graphics_context_set_text_color(ctx, color);
  graphics_draw_text(ctx, value, fonts_get_system_font(font), rect,
                     GTextOverflowModeTrailingEllipsis, align, NULL);
}

static void courier(GContext *ctx, int x, int y, GColor coat) {
  /* Original 15x28-pixel traveler: cap, head, satchel, coat, boots. */
  box(ctx, x + 5, y, 9, 3, coat);
  box(ctx, x + 7, y + 3, 6, 6, GColorFromHEX(0xFFDD99));
  box(ctx, x + 3, y + 10, 12, 10, coat);
  box(ctx, x, y + 11, 4, 9, GColorFromHEX(0x4A2545));
  box(ctx, x + 5, y + 20, 3, 6, GColorFromHEX(0x26364A));
  box(ctx, x + 11, y + 20, 3, 6, GColorFromHEX(0x26364A));
  box(ctx, x + 3, y + 26, 6, 3, GColorBlack);
  box(ctx, x + 11, y + 26, 6, 3, GColorBlack);
}

static void roof(GContext *ctx, int x, int y, int w, GColor c) {
  graphics_context_set_fill_color(ctx, c);
  graphics_fill_rect(ctx, GRect(x, y, w, 9), 0, GCornerNone);
}

static void tree(GContext *ctx, int x, int y, Palette c) {
  box(ctx, x + 12, y + 21, 6, 31, c.trunk);
  box(ctx, x, y + 9, 30, 22, c.tree);
  box(ctx, x + 6, y, 18, 15, c.tree);
}

static void draw_location(GContext *ctx, int location, Palette c) {
  switch (location) {
    case 1: /* City skyline and lit windows. */
      box(ctx, 4, 128, 30, 56, c.house);
      box(ctx, 36, 107, 36, 77, c.roof);
      box(ctx, 76, 137, 27, 47, c.house);
      box(ctx, 118, 115, 35, 69, c.roof);
      box(ctx, 158, 132, 38, 52, c.house);
      for (int x = 12; x < 190; x += 18)
        for (int y = 145; y < 179; y += 16)
          box(ctx, x, y, 5, 7, c.window);
      break;
    case 2: /* Farm: barn, silo, and field rows. */
      box(ctx, 10, 133, 66, 51, c.roof);
      roof(ctx, 7, 125, 72, c.house);
      box(ctx, 31, 149, 22, 35, c.door);
      box(ctx, 119, 120, 22, 64, c.house);
      roof(ctx, 117, 114, 26, c.roof);
      for (int x = 79; x < 193; x += 23)
        box(ctx, x, 171, 15, 5, c.tree);
      break;
    case 3: /* Countryside: rolling fields, fence, lone tree. */
      box(ctx, 0, 153, FACE_W, 31, c.tree);
      box(ctx, 0, 173, FACE_W, 11, c.house);
      tree(ctx, 21, 122, c);
      for (int x = 101; x < 200; x += 25) {
        box(ctx, x, 151, 3, 30, c.trunk);
        box(ctx, x, 159, 23, 3, c.roof);
      }
      break;
    case 4: /* Coast: water, beach, lighthouse, and boat. */
      box(ctx, 0, 142, FACE_W, 36, c.window);
      for (int x = 0; x < FACE_W; x += 30)
        box(ctx, x, 151 + (x % 3), 17, 2, GColorWhite);
      box(ctx, 0, 178, FACE_W, 6, c.house);
      box(ctx, 27, 116, 21, 68, c.house);
      roof(ctx, 24, 109, 27, c.roof);
      box(ctx, 32, 130, 10, 7, c.window);
      box(ctx, 127, 164, 32, 5, c.roof);
      box(ctx, 143, 145, 2, 19, c.trunk);
      break;
    case 5: /* Forest: overlapping trunks and canopies. */
      tree(ctx, 6, 116, c);
      tree(ctx, 56, 105, c);
      tree(ctx, 110, 114, c);
      tree(ctx, 158, 99, c);
      box(ctx, 0, 177, FACE_W, 7, c.tree);
      break;
    case 6: /* Mountains: two stepped peaks and foothills. */
      for (int i = 0; i < 6; ++i) {
        box(ctx, 8 + i * 8, 149 - i * 7, 90 - i * 16, 35 + i * 7, c.house);
        box(ctx, 106 + i * 9, 157 - i * 8, 88 - i * 16, 27 + i * 8, c.roof);
      }
      box(ctx, 0, 176, FACE_W, 8, c.tree);
      break;
    default: /* Original village. */
      box(ctx, 10, 119, 57, 65, c.house);
      box(ctx, 19, 137, 13, 15, c.window);
      box(ctx, 46, 137, 12, 15, c.window);
      box(ctx, 35, 158, 13, 26, c.door);
      roof(ctx, 7, 111, 63, c.roof);
      tree(ctx, 143, 123, c);
      break;
  }
  box(ctx, 0, 184, FACE_W, 44, c.road);
  box(ctx, 0, 184, FACE_W, 4, c.road_edge);
  for (int i = 0; i < 5; ++i)
    box(ctx, i * 47 + 4, 213, 23, 2, c.road_mark);
}

static void draw(Layer *layer, GContext *ctx) {
  Palette color = colors();
  time_t now = time(NULL);
  struct tm *local = localtime(&now);
  int weather = s_updated_at && now >= s_updated_at &&
                now - s_updated_at <= STALE_SECONDS ? s_weather : -1;
  static char clock_text[12];
  strftime(clock_text, sizeof(clock_text),
           clock_is_24h_style() ? "%H:%M" : "%I:%M", local);
  if (!clock_is_24h_style() && clock_text[0] == '0')
    memmove(clock_text, clock_text + 1, strlen(clock_text));

  GColor sky = weather == 1 ? color.rain_sky :
               weather == 2 ? color.snow_sky : color.sky;
  box(ctx, 0, 0, FACE_W, FACE_H, sky);
  text(ctx, clock_text,
       s_font ? FONT_KEY_BITHAM_42_LIGHT : FONT_KEY_BITHAM_42_BOLD,
       GRect(7, 5, 186, 65),
       GColorWhite, GTextAlignmentCenter);
  text(ctx, "LITTLE ERRAND", FONT_KEY_GOTHIC_14_BOLD, GRect(9, 66, 182, 22),
       GColorWhite, GTextAlignmentCenter);

  draw_location(ctx, location_for_time(s_location, local->tm_year,
                                      local->tm_yday, local->tm_hour), color);

  if (weather == 1 || weather == 2) {
    for (int i = 0; i < 18; ++i) {
      int x = (i * 37 + 11) % FACE_W;
      int y = 96 + (i * 31) % 80;
      if (weather == 1) box(ctx, x, y, 2, 7, GColorFromHEX(0xADD8E6));
      else box(ctx, x, y, 3, 3, GColorWhite);
    }
  } else if (weather == 0) {
    graphics_context_set_fill_color(ctx, GColorFromHEX(0xFFDD55));
    graphics_fill_circle(ctx, GPoint(s_date ? 180 : 162, 111), 13);
  } else {
    box(ctx, 118, 107, 55, 10, GColorFromHEX(0xD8E0E3));
  }

  int x = 16 + local->tm_min * 157 / 59;
  courier(ctx, x, 157, color.courier);
  if (weather < 0)
    text(ctx, "WEATHER OFFLINE", FONT_KEY_GOTHIC_14,
         GRect(13, 192, 174, 18), GColorWhite, GTextAlignmentCenter);
  if (s_date) {
    char date[16];
    const char *format = s_date == 1 ? "%Y-%m-%d" :
                         s_date == 2 ? "%m/%d/%Y" : "%d/%m/%Y";
    strftime(date, sizeof(date), format, local);
    box(ctx, 40, 87, 120, 19, GColorBlack);
    text(ctx, date, FONT_KEY_GOTHIC_14_BOLD, GRect(42, 86, 116, 20),
         GColorWhite, GTextAlignmentCenter);
  }
}

static void tick(struct tm *time_now, TimeUnits units) {
  (void)time_now; (void)units;
  layer_mark_dirty(s_layer);
}

static void inbox(DictionaryIterator *iter, void *context) {
  (void)context;
  layer_mark_dirty(s_layer);
  Tuple *palette = dict_find(iter, MESSAGE_KEY_SETTING_PALETTE);
  if (palette && palette->value->int32 >= 0 && palette->value->int32 <= 2) {
    s_palette = palette->value->int32;
    persist_write_int(PERSIST_PALETTE, s_palette);
    layer_mark_dirty(s_layer);
  }
  Tuple *font = dict_find(iter, MESSAGE_KEY_SETTING_FONT);
  if (font && font->value->int32 >= 0 && font->value->int32 <= 1) {
    s_font = font->value->int32;
    persist_write_int(PERSIST_FONT, s_font);
    layer_mark_dirty(s_layer);
  }
  Tuple *location = dict_find(iter, MESSAGE_KEY_SETTING_LOCATION);
  if (location && location->value->int32 >= 0 && location->value->int32 <= 7) {
    s_location = location->value->int32;
    persist_write_int(PERSIST_LOCATION, s_location);
    layer_mark_dirty(s_layer);
  }
  Tuple *date = dict_find(iter, MESSAGE_KEY_SETTING_DATE);
  if (date && date->value->int32 >= 0 && date->value->int32 <= 3) {
    s_date = date->value->int32;
    persist_write_int(PERSIST_DATE, s_date);
    layer_mark_dirty(s_layer);
  }
  Tuple *state = dict_find(iter, MESSAGE_KEY_WEATHER);
  if (!state || state->value->int32 < -1 || state->value->int32 > 3) return;
  s_weather = state->value->int32;
  s_updated_at = s_weather < 0 ? 0 : time(NULL);
  layer_mark_dirty(s_layer);
}

static void load(Window *window) {
  s_layer = layer_create(GRect(0, 0, FACE_W, FACE_H));
  layer_set_update_proc(s_layer, draw);
  layer_add_child(window_get_root_layer(window), s_layer);
}

static void unload(Window *window) {
  (void)window;
  layer_destroy(s_layer);
}

int main(void) {
  if (persist_exists(PERSIST_PALETTE)) s_palette = persist_read_int(PERSIST_PALETTE);
  if (persist_exists(PERSIST_FONT)) s_font = persist_read_int(PERSIST_FONT);
  if (persist_exists(PERSIST_LOCATION)) s_location = persist_read_int(PERSIST_LOCATION);
  if (persist_exists(PERSIST_DATE)) s_date = persist_read_int(PERSIST_DATE);
  if (s_palette < 0 || s_palette > 2) s_palette = 0;
  if (s_font < 0 || s_font > 1) s_font = 0;
  if (s_location < 0 || s_location > 7) s_location = 0;
  if (s_date < 0 || s_date > 3) s_date = 0;
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {.load = load, .unload = unload});
  window_stack_push(s_window, true);
  tick_timer_service_subscribe(MINUTE_UNIT, tick);
  app_message_register_inbox_received(inbox);
  app_message_open(128, 64);
  app_event_loop();
  app_message_deregister_callbacks();
  tick_timer_service_unsubscribe();
  window_destroy(s_window);
}
