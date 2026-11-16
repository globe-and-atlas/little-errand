#!/usr/bin/env python3
"""Build one Pebble face and run its phone-side behavior checks."""
from __future__ import annotations

import json
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
EXPECTED = {
    "rain-clock": "Rain Clock",
    "little-errand": "Little Errand",
    "change-clock": "Change Clock",
}


def main() -> int:
    project = ROOT.name
    if project not in EXPECTED:
        print(f"Unexpected project: {project}", file=sys.stderr)
        return 1
    watchface = ROOT / "watchface"
    manifest = json.loads((watchface / "package.json").read_text(encoding="utf-8"))
    pebble = manifest["pebble"]
    assert pebble["displayName"] == EXPECTED[project]
    assert pebble["targetPlatforms"] == ["emery"]
    assert pebble["watchapp"]["watchface"] is True
    assert "configurable" in pebble["capabilities"]
    assert "SETTING_PALETTE" in pebble["messageKeys"]
    assert "SETTING_FONT" in pebble["messageKeys"]
    assert "SETTING_LOCATION" in pebble["messageKeys"]
    assert "SETTING_DATE" in pebble["messageKeys"]
    config = json.loads((watchface / "src" / "pkjs" / "config.json").read_text(encoding="utf-8"))
    choices = [item for section in config for item in section.get("items", [])
               if item.get("type") == "select"]
    assert {item["messageKey"] for item in choices} == {
        "SETTING_PALETTE", "SETTING_FONT", "SETTING_LOCATION", "SETTING_DATE"
    }
    assert sorted(len(item["options"]) for item in choices) == [2, 3, 4, 8]
    assert len(pebble["uuid"]) == 36
    subprocess.run(["node", str(ROOT / "execution" / "verify_phone.js")],
                   cwd=ROOT, check=True)
    rotation_test = ROOT / ".tmp" / "verify_rotation"
    rotation_test.parent.mkdir(parents=True, exist_ok=True)
    subprocess.run(["cc", "-std=c99", "-Wall", "-Wextra", "-Werror",
                    str(ROOT / "execution" / "verify_rotation.c"), "-o",
                    str(rotation_test)], cwd=ROOT, check=True)
    subprocess.run([str(rotation_test)], cwd=ROOT, check=True)
    subprocess.run(["pebble", "build"], cwd=watchface, check=True,
                   stdout=subprocess.DEVNULL)
    output = watchface / "build" / "watchface.pbw"
    assert output.is_file() and output.stat().st_size > 1000
    subprocess.run(["node", str(ROOT / "execution" / "verify_settings.js")],
                   cwd=ROOT, check=True)
    print(f"PASS {project}: phone logic, Emery manifest, PBW build")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
