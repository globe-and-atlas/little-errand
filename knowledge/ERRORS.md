# Errors

Record deterministic errors, root causes, and fixes here.

## 2026-10-17 — Emery stopped responding during settings render
- Error: PBW install failed and a subsequent screenshot timed out while switching from Rain Clock to Little Errand.
- Cause: unconfirmed emulator state; this followed repeated AppMessage probes in the shared emulator.
- Fix: inspect the emulator process and restart it if it remains unresponsive, then retry the scoped install.
- Graduated-to: infrastructure incident until isolated retry is complete.

## 2026-10-17 — Settings message keys missing from C build
- Error: the first settings build failed because `MESSAGE_KEY_SETTING_PALETTE` and `MESSAGE_KEY_SETTING_FONT` were undeclared.
- Cause: the incremental build reused stale generated message keys from the earlier manifest.
- Fix: run `pebble clean && pebble build` after changing message keys.
- Graduated-to: `knowledge/procedural/build.md` clean-build step after changing message keys.

## 2026-10-17 — Install did not select face
- Error: Emery reported a successful Little Errand install, but the screenshot still showed Rain Clock.
- Cause: installing a watchface does not necessarily make it the active face in the persisted emulator.
- Fix: select the face through the emulator UI before capture; do not equate install success with a render.
- Graduated-to: watchface emulator verification workflow.

## 2026-10-17 — Weather scene could remain stale
- Error: the initial scene retained the last received weather state indefinitely after phone disconnection.
- Cause: Little Errand stored the condition without a receive timestamp.
- Fix: expire the condition after one hour and show the offline state while retaining the clock and courier.
- Graduated-to: weather freshness behavior in the build directive.
## 2026-10-18 — Scene verification assertion
- Error: `python3 execution/verify_watchface.py` failed on the settings-key assertion.
- Cause: the verifier still expected only palette and font after the location setting was added.
- Fix: extend the verifier to include the location key and eight choices.
- Graduated-to: `knowledge/procedural/build.md` after verification passes.

## 2026-10-18 — Pebble message key generation
- Error: C build did not define `MESSAGE_KEY_SETTING_LOCATION` after manifest edit.
- Cause: incremental Pebble build retained generated message-key headers.
- Fix: clean the Pebble build before rebuilding with a new manifest key.
- Graduated-to: `knowledge/procedural/build.md` after clean build passes.

## 2026-10-18 — Phone settings expectation
- Error: `execution/verify_phone.js` expected a two-key settings message.
- Cause: the new location field is always sent, defaulting to Village.
- Fix: update the phone verification expectation for the third field.
- Graduated-to: `knowledge/procedural/build.md` after verification passes.

## 2026-10-18 — Workspace session capture
- Error: `.agent/scripts/session_capture.py` exited while rebuilding the workspace inbox index with `OSError: [Errno 89] Operation canceled` on a Markdown read.
- Cause: unconfirmed filesystem or inbox availability issue outside Little Errand.
- Fix: pending a retry or workspace inbox inspection.
- Graduated-to: none; infrastructure error only.
- Retry: capture file already existed; inbox index refresh then reported `Operation timed out`.
## 2026-10-18 Date setting verification
- Error: existing settings assertion failed after adding the date message key.
- Cause: expected settings object/key set did not include the new date choice.
- Fix: update verifier expectations to cover date defaults and valid/invalid choices.
- Graduated-to: execution verifier for this project.

## 2026-10-18 Phone verifier expectation
- Error: phone verifier expected the old settings object without date.
- Cause: date defaults to Off and is always included in the settings message.
- Fix: extend phone verifier to assert date Off and selected formats.
- Graduated-to: `execution/verify_phone.js`.

## 2026-10-18 Date and sun overlap
- Error: native Emery Date On capture showed the date plate crossing the clear-weather sun.
- Cause: date plate ended at x160 while the sun began at x149.
- Fix: shift the sun to x180 only when a date is visible, preserving the original Off layout.
- Graduated-to: native date layout review.

## 2026-10-18 Diff whitespace
- Error: `git diff --check` reported trailing blank lines in edited Markdown.
- Cause: append patches left extra EOF spacing.
- Fix: trim trailing blank lines before final validation.
- Graduated-to: pre-release diff check.
