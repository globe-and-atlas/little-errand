#include <assert.h>
#include "../watchface/src/c/location_schedule.h"

int main(void) {
  for (int fixed = 0; fixed < 7; ++fixed)
    for (int hour = 0; hour < 24; ++hour)
      assert(location_for_time(fixed, 126, 290, hour) == fixed);
  for (int day = 0; day < 366; ++day) {
    int seen = 0;
    for (int slot = 0; slot < 6; ++slot) {
      int hour = slot * 4;
      int location = location_for_time(7, 126, day, hour);
      assert(location >= 0 && location < 7);
      assert(!(seen & (1 << location)));
      seen |= 1 << location;
      for (int later = 1; later < 4; ++later)
        assert(location_for_time(7, 126, day, hour + later) == location);
    }
  }
  return 0;
}
