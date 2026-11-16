---
generated_by: "OpenAI Codex (GPT-6.1-sol)"
timestamp: "2026-10-17T22:12:00-05:00"
---
# Build and inspect Little Errand

1. Run `python3 execution/verify_watchface.py` from the repository root. This checks phone logic, the Emery manifest, and the PBW build.

After changing `messageKeys` in `watchface/package.json`, first run `pebble clean` from `watchface/`; an incremental build may reuse stale generated C keys.
After changing `src/pkjs/config.json`, clean-build too; the incremental build may reuse a stale JS bundle. `execution/verify_settings.js` checks the packaged settings label.
2. Run `pebble install --emulator emery build/watchface.pbw` from `watchface/`.
3. Hold Select on the emulator, open Watchfaces, select Little Errand, and return to the face. Install success alone does not select a face.
4. Capture with `pebble screenshot --emulator emery --no-open ../.tmp/little-errand-emery.png`.

Observed: After install, selecting the face through the Watchfaces menu displayed the original courier. Injecting message key 10000 with values 1 and 2 displayed rain and snow; setting emulator time to 22:45 moved the courier.

The screenshot is emulator evidence. Device installation and store publication need separate checks.

After more than one hour without a fresh weather message, the scene displayed `WEATHER OFFLINE`; the clock and courier remained visible.

Settings verification: `python3 execution/verify_watchface.py` checks the bundled Clay page, save/cancel behavior, phone storage restore, and PBW build. On Emery, Twilight violet with Large light and Mint village with Large bold both rendered; reinstalling the face retained Mint village.
## Listing media — 2026-10-17

Run `python3 ../rain-clock/execution/create_listing_media.py --project little-errand` from this project to package three previously captured native Emery frames from `.tmp/` into `listing/`. Run the same command with `--verify-only` to check PNG/GIF dimensions, distinct frames, and file sizes. The GIF is a three-state listing preview showing clear Storybook day, rainy Storybook day, and clear Twilight violet; it does not depict a continuous on-watch animation. Upload the three PNGs and one GIF as the Emery screenshots. The Repebble listing guide allows up to five Emery screenshot images, exactly 200×228 pixels, at most 4.4 MB each.
## 2026-10-17 — Repebble search preview

The public search card uses the first Emery screenshot. In the developer editor, save the GIF as screenshot one and retain the three static PNG screenshots after it. Verify the search result card image URL ends in `.gif`.

## Scene locations — 2026-10-18

After adding `SETTING_LOCATION` to `messageKeys`, run `pebble clean` in `watchface/` before running `python3 execution/verify_watchface.py` from the project root. The verifier compiles `execution/verify_rotation.c` to `.tmp/` and tests fixed locations and six four-hour travel slots across a leap year. To inspect every fixed scene on an active Emery face, run `python3 execution/capture_locations.py`; captures go to `.tmp/locations/`.
## 2026-10-18 Date options

Run `python3 execution/verify_watchface.py` after changing the Clay date setting or Pebble message keys. Use `pebble clean` before the first build with a new message key so generated headers refresh. The date choices are Off, YYYY-MM-DD, MM/DD/YYYY, and DD/MM/YYYY; Type Foundry retains its existing Data line switch and weekday date default. In the emulator, installation alone may leave another face selected: open Watchfaces with Select, choose this face, then capture a screenshot.

