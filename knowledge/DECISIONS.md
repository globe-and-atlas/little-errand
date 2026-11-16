# Decisions

## Initial Decision

- Template profile: `workflow-python`
- Deploy target: `local-only`
- Runtime: `pebble`

## 2026-10-17 — Separate Time 2 watchface
- Decision: ship Little Errand as its own Pebble project and PBW, initially targeting Emery.
- Alternative: one configurable app containing all three concepts.
- Reason: the user chose three separate faces; individual projects also fit the existing CloudPebble import convention.
- Decision: keep weather parsing on the phone and minute-based drawing on the watch.
- Alternative: call the weather API from the watch.
- Reason: PebbleKit JS can use phone location and network access while the watch receives compact validated values.

## 2026-10-17 — Appearance settings
- Decision: offer three coordinated color palettes and two system clock fonts through the local Clay settings page.
- Alternative: arbitrary color pickers and hosted HTML.
- Reason: coordinated presets retain the scene's visual hierarchy; Clay avoids a hosted page.
- Decision: persist palette and font on the watch.
- Alternative: restore solely from phone storage at launch.
- Reason: the chosen appearance remains available while disconnected.
## 2026-10-17 — Listing screenshot fidelity
- Decision: package raw, unframed Emery emulator captures as the listing screenshots and GIF frames.
- Alternative: draw a watch mockup or synthesize a smooth animation.
- Reason: the listing should show the real 200×228 output and the GIF should only preview states the face can render.
## 2026-10-18 — Location rotation
- Decision: Keep Village as the default and add six distinct locations.
- Decision: Travel mode changes on local four-hour boundaries using a day-seeded shuffle of seven locations.
- Reason: Each scene stays long enough to recognize, and six daily changes avoid repeats without network access or persisted random state.
- Alternative considered: unconstrained random choice at each boundary; rejected because it can repeat the same scene.
## 2026-10-18 Date setting

The newer Emery faces use a single Date selector whose Off value preserves the previous display and whose three numeric values select the date order. Type Foundry keeps its existing Off/Date/Battery data line and adds a separate date order selector, preserving existing saved behavior.
