# Little Errand

Little Errand is an original pixel-style courier walking across a small town as the minute changes.

## Build and verify

Requires Pebble SDK with Emery support, Node.js, and Python 3.

```bash
python3 execution/verify_watchface.py
cd watchface
pebble install --emulator emery build/watchface.pbw
```

To select an installed face in the emulator, hold Select, open Watchfaces, choose Little Errand, then Select. Installing a PBW alone may leave the prior face active.

## Options

Open Little Errand's settings gear in the Pebble phone app. Choose **Colors**: Storybook day, Twilight violet, or Mint village. Choose **Clock font**: Large bold or Large light. Choose **Location**: Village, City, Farm, Countryside, Coast, Forest, Mountains, or Travel through locations. Choose **Date**: Off, YYYY-MM-DD, MM/DD/YYYY, or DD/MM/YYYY. Travel mode changes locations every four hours and visits six different locations each local day. Storybook day, Large bold, Village, and Date Off are the defaults. Save to sync the choices to the watch; the watch remembers them across restarts. Weather still changes the sky within the selected palette.

The app targets **Pebble Time 2 only**. The source is in `watchface/src/c/` and `watchface/src/pkjs/`. Generated PBWs and screenshots stay uncommitted. No store publication or device acceptance is implied by an emulator build.

## Data

Pebble local time drives the traveler. Phone location and Open-Meteo weather code select clear, cloud, rain, or snow scenery. The phone refresh interval is 30 minutes. The clock and traveler work offline; weather scenery expires after one hour.

[Open-Meteo forecast API](https://open-meteo.com/en/docs) supplies weather without a key for this prototype.
