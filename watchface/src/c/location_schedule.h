#ifndef LITTLE_ERRAND_LOCATION_SCHEDULE_H
#define LITTLE_ERRAND_LOCATION_SCHEDULE_H

/* Shuffle once per local day; six four-hour slots visit six different stops. */
static inline int location_for_time(int setting, int year, int day, int hour) {
  if (setting != 7) return setting;
  unsigned int key = (unsigned int)(year * 366 + day);
  unsigned int seed = key * 1103515245u + 12345u;
  int order[7] = {0, 1, 2, 3, 4, 5, 6};
  for (int i = 6; i > 0; --i) {
    seed = seed * 1664525u + 1013904223u;
    int j = seed % (unsigned int)(i + 1);
    int swap = order[i];
    order[i] = order[j];
    order[j] = swap;
  }
  return order[hour / 4];
}

#endif
