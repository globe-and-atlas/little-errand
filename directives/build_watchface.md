---
generated_by: "OpenAI Codex (GPT-6.1-sol)"
timestamp: "2026-10-17T22:10:00-05:00"
---
# Build Little Errand

Purpose: An original courier moves through a neighborhood as the minutes change.

Input: watch local time and, where needed, phone-provided weather for the current location.

Execution: implement Pebble C rendering in `watchface/src/c/`; keep forecast parsing and validation in `watchface/src/pkjs/`; run `python3 execution/verify_watchface.py`; build with `pebble build` from `watchface/`.

Output: an Emery PBW in `watchface/build/` and an emulator screenshot in `.tmp/`.

Edge case: show a clear unavailable state when the phone, location, or weather feed fails. Do not present expired data as live.

## Validation Contract
- The Pebble manifest targets Emery.
- The courier position changes with the minute of the hour.
- The scene changes visibly for rain, snow, or clear conditions.
- The current time is readable on the emulator.
- The PBW builds successfully.
- A fresh verifier reviews the result.

## Learnings — 2026-10-17
- The SDK builds an Emery PBW from this project's `watchface/` directory.
- Installing a PBW does not always select it as the emulator's active watchface; use the Watchfaces menu and inspect the screenshot.
- Weather data must expire after one hour so a disconnected phone cannot leave an apparently live condition.
## 2026-10-17 — Listing media

Prepare three native Emery screenshots and one native-frame animated GIF under `listing/`. Use emulator captures for every frame, with no watch outline or composited interface. The current listing limit is 200×228 pixels and 4.4 MB per image; check the Repebble preparation guide again before publication. Run `python3 ../rain-clock/execution/create_listing_media.py` from this repository after captures are selected, then inspect the outputs and run the media verifier.

## 2026-10-18 — Scene locations

Offer seven distinct settings locations: Village, City, Farm, Countryside, Coast, Forest, and Mountains. Default to Village for existing installations. Offer a rotating setting that selects a different location every four hours, with no location repeated in a local day. Keep clock, courier, and weather visible in each scene.

For emulator visual review, activate Little Errand, then run `python3 execution/capture_locations.py` from the project root. It writes seven fixed-scene screenshots to `.tmp/locations/`.

### Validation Contract
- The settings page contains seven fixed locations.
- The settings page contains one rotating option.
- A stored location survives restart.
- Rotating mode selects six distinct locations per local day.
- Weather remains visible in each scene.
- The clock remains legible in each scene.
- The courier remains visible in each scene.
- `python3 execution/verify_watchface.py` succeeds.
- A fresh verifier checks the final build.
