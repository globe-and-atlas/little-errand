---
generated_by: "OpenAI Codex (GPT-6.1-sol)"
timestamp: "2026-10-17T22:22:00-05:00"
---
# Little Errand — first installable Time 2 build

## Scene locations — 2026-10-18

### Contract
- Target directive: `directives/build_watchface.md`.
- Intended execution script: `execution/verify_watchface.py`.
- Expected output artifact: `watchface/build/watchface.pbw`.
- Safety: keep generated captures in `.tmp/`.
- Safety: do not commit secrets.
- Safety: do not publish without a separate request.

### Validation Contract
- [x] Settings offer Village, City, Farm, Countryside, Coast, Forest, and Mountains.
- [x] Settings offer a rotating location option.
- [x] A fixed location remains selected after a restart.
- [x] The rotating option changes location every four hours.
- [x] The rotating option avoids repeating a location within one day.
- [x] Weather overlays render for each location.
- [x] The courier remains visible for each location.
- [x] The current time remains readable for each location.
- [x] `python3 execution/verify_watchface.py` succeeds.
- [x] A fresh verifier reviews the built artifact.

### Progress
- [x] Implement location setting and scenes.
- [x] Build and verify.
- [x] Record lasting project knowledge.

## Contract
- Target directive: `directives/build_watchface.md`.
- Intended execution script: `execution/verify_watchface.py`.
- Expected output artifact: `watchface/build/*.pbw`.
- Safety: never read or commit `.env`.
- Safety: never commit `.tmp/` or generated builds.
- Safety: never exfiltrate secrets.
- Safety: do not publish or push without an explicit request.

## Validation Contract
- [x] `watchface/package.json` declares a watch face.
- [x] `watchface/package.json` targets Emery.
- [x] An original courier is visible.
- [x] A neighborhood scene is visible.
- [x] The courier position changes with the minute of the hour.
- [x] A clear condition displays a sun.
- [x] A rain condition displays falling rain.
- [x] A snow condition displays falling snow.
- [x] A weather state older than one hour displays an offline label.
- [x] The face displays a legible time in a 200×228 emulator frame.
- [x] `python3 execution/verify_watchface.py` exits successfully.
- [x] `pebble build` exits successfully.
- [x] A fresh verifier checks the final artifact against these criteria.

## Progress
- [x] Project created from the canonical template.
- [x] Implement the watch face.
- [x] Run verification and inspect emulator rendering.
- [x] Record lasting project knowledge.

## Settings extension — 2026-10-17
- [x] A phone settings page opens for Little Errand.
- [x] The page offers three named color palettes.
- [x] The page offers two clock font choices.
- [x] Saving a choice changes the watch rendering.
- [x] A saved choice survives a watch face restart.
- [x] Invalid settings are rejected.
- [x] `python3 execution/verify_watchface.py` passes.
- [x] A fresh verifier reviews the final settings behavior.

## Listing media — 2026-10-17

### Contract
- Target directive: `directives/build_watchface.md`.
- Intended execution script: `../rain-clock/execution/create_listing_media.py`.
- Expected output artifacts: `listing/emery-01-main.png`, `listing/emery-02-variant.png`, `listing/emery-03-variant.png`, `listing/emery-04-preview.gif`.
- Safety: keep capture intermediates in `.tmp/`.
- Safety: never include secrets or location coordinates in listing media.
- Safety: do not publish the listing.

### Validation Contract
- [x] `listing/emery-01-main.png` is exactly 200×228 pixels.
- [x] `listing/emery-02-variant.png` is exactly 200×228 pixels.
- [x] `listing/emery-03-variant.png` is exactly 200×228 pixels.
- [x] `listing/emery-04-preview.gif` is exactly 200×228 pixels.
- [x] `listing/emery-04-preview.gif` contains at least three distinct frames.
- [x] Each listing image is smaller than 4.4 MB.
- [x] Each frame shows Little Errand rendered by the Emery emulator.
- [x] A fresh verifier reviews the exported media.
## 2026-10-18 Date choices

### Contract
- Target directive: `directives/build_watchface.md`.
- Intended execution script: `execution/verify_watchface.py`.
- Expected artifact: `watchface/build/watchface.pbw`.
- Safety: keep temporary captures in `.tmp/`.
- Safety: exclude secrets from Git.

### Validation Contract
- [x] Date Off preserves the current face layout.
- [x] Date On displays a local calendar date.
- [x] Date order offers year-month-day.
- [x] Date order offers month-day-year.
- [x] Date order offers day-month-year.
- [x] The selected choices survive a watch restart.
- [x] The Emery PBW builds.
- [x] A fresh verifier checks the final behavior.
