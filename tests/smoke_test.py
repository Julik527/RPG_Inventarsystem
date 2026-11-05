#!/usr/bin/env python3
"""Echte Konsolenablaeufe, ohne Build-Dateien ins Repository zu schreiben."""
import pathlib
import subprocess
import sys
import tempfile


def run(binary: pathlib.Path, folder: pathlib.Path, commands: list[str]) -> str:
    result = subprocess.run(
        [str(binary)], input="\n".join(commands) + "\n",
        cwd=folder, text=True, capture_output=True, encoding="utf-8",
        errors="replace", timeout=15, check=False
    )
    if result.returncode != 0:
        raise AssertionError(
            f"Programmabbruch {result.returncode}: {result.stderr}\n{result.stdout[-1800:]}"
        )
    if "Programm beendet." not in result.stdout:
        raise AssertionError("Kein sauberer Programmabschluss")
    return result.stdout


def check(output: str, *parts: str) -> None:
    for part in parts:
        if part not in output:
            raise AssertionError(f"Fehlender Text: {part!r}\n{output[-2200:]}")


def main() -> int:
    if len(sys.argv) != 2:
        print("Aufruf: smoke_test.py <Pfad-zum-Programm>", file=sys.stderr)
        return 2
    binary = pathlib.Path(sys.argv[1]).resolve()

    with tempfile.TemporaryDirectory(prefix="inventar-test-") as tmp:
        root = pathlib.Path(tmp)

        # Leeres Inventar und fehlerhafte Zahlen duerfen nicht abstuerzen.
        output = run(binary, root, ["8", "abc", "2", "Schwert", "1",
                                    "50", "0", "2", "5", "1", "8",
                                    "9", "1", "10", "12"])
        check(output, "Inventar ist leer.", "Ungueltige Zahl.",
              "Schwert", "Bestes Item: Schwert", "Inventar ist zu schwer.",
              "Inventar gespeichert.")
        saved = root / "inventar.txt"
        if saved.read_text(encoding="utf-8").strip() != "Schwert;1;50;2;5":
            raise AssertionError("Speicherformat oder Werte fehlerhaft")

        # Neustart muss den Wert erhalten.
        output = run(binary, root, ["1", "12"])
        check(output, "Schwert", "Gesamtgewicht: 2")

        # Manipulierte Daten: negative und Null-Gewichte ablehnen.
        saved.write_text("Kaputt;1;50;0;5\n", encoding="utf-8")
        output = run(binary, root, ["11", "1", "12"])
        check(output, "Datei fehlt oder ist fehlerhaft.", "Inventar ist leer.")

        # Ueberlauf der int-Summe darf Tragelimit nicht umgehen.
        saved.write_text(
            "Gross;1;20;2147483647;2\n"
            "Zweit;2;40;2147483647;2\n", encoding="utf-8"
        )
        output = run(binary, root, ["9", "2147483647", "8", "12"])
        check(output, "Inventar ist zu schwer.", "Bestes Item: Zweit")

        # Zusätzliche Spalten und ungueltige enum-Werte ablehnen.
        saved.write_text("A;9;10;1;1\n", encoding="utf-8")
        output = run(binary, root, ["11", "12"])
        check(output, "Datei fehlt oder ist fehlerhaft.")

    print("Inventar: Eingaben, leeres Inventar, Speichern/Laden, "
          "Dateipruefung, Tragelimit und grosse Werte OK.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
