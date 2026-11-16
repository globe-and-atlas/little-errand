#!/usr/bin/env python3
"""Capture the seven Little Errand scenes from an active Emery face."""
from pathlib import Path
import json
import subprocess

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / ".tmp" / "locations"
NAMES = ("village", "city", "farm", "countryside", "coast", "forest", "mountains")


def run(*args: str) -> None:
    subprocess.run(["pebble", *args], cwd=ROOT / "watchface", check=True,
                   stdout=subprocess.DEVNULL)


def main() -> None:
    OUT.mkdir(parents=True, exist_ok=True)
    manifest = json.loads((ROOT / "watchface" / "build" / "appinfo.json").read_text())
    key = manifest["messageKeys"]["SETTING_LOCATION"]
    for index, name in enumerate(NAMES):
        run("send-app-message", "--emulator", "emery", "--int", f"{key}={index}")
        run("screenshot", "--emulator", "emery", "--no-open", str(OUT / f"{name}.png"))
    print(f"Captured {len(NAMES)} scenes in {OUT}")


if __name__ == "__main__":
    main()
