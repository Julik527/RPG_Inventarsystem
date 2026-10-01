# RPG-Inventarsystem

Ein Konsolenprogramm in **C++**, das ein Rollenspiel-Inventar verwaltet.

Das Projekt wurde im Fach **Programmiertechnik** erstellt und demonstriert den praktischen Einsatz zentraler C++-Grundlagen und STL-Komponenten.

## Projektinformationen

- **Autor:** Julian Krauß
- **Klasse:** ITA 09/25
- **Ausbildung:** Informationstechnischer Assistent
- **Sprache:** C++
- **Standard:** C++17
- **Version:** 1.1
- **Stand:** 01.10.2026

## Projektstruktur

```text
RPG_Inventarsystem/
├── src/
│   └── main.cpp
├── Aufgabenstellung.MD
├── README.md
└── .gitignore
```

Die Datei `inventar.txt` wird beim Ausführen des Programms automatisch erzeugt und nicht in Git eingecheckt.

## Funktionen

Das Programm unterstützt:

1. Inventar anzeigen
2. Item hinzufügen
3. Item entfernen
4. Nach Wert sortieren
5. Nach Gewicht sortieren
6. Items pro Typ anzeigen
7. Stärkstes Item bestimmen
8. Bestes Wert-Gewicht-Verhältnis bestimmen
9. Tragelimit prüfen
10. Inventar speichern
11. Inventar laden
12. Programm beenden

Beim regulären Beenden wird das Inventar automatisch gespeichert.

## Datenstruktur

Ein Gegenstand wird mit einer `struct` beschrieben:

```cpp
struct Item
{
    string name;
    Typ type;
    int wert;
    int gewicht;
    int staerke;
};
```

Damit werden alle Eigenschaften eines Gegenstands gemeinsam gespeichert.

## Item-Typen

Die Gegenstandstypen werden mit `enum class` definiert:

```cpp
enum class Typ
{
    Waffe = 1,
    Traenke,
    Ruestung
};
```

Dadurch stehen nur die vorgesehenen Kategorien zur Verfügung.

## Verwendete C++-Techniken

Im Projekt werden unter anderem eingesetzt:

- `struct`
- `enum class`
- `std::vector`
- `std::map`
- Referenzen mit `&`
- `const`
- Range-based for-Schleifen
- `std::sort`
- `std::find_if`
- Lambda-Ausdrücke
- `ifstream` und `ofstream`
- `stringstream`
- `iomanip`
- `static_cast`

## Sortierung

### Nach Wert

Die Items werden absteigend sortiert, sodass das wertvollste Item zuerst erscheint.

```cpp
sort(
    inv.begin(),
    inv.end(),
    [](const Item& a, const Item& b)
    {
        return a.wert > b.wert;
    }
);
```

### Nach Gewicht

Die Items werden aufsteigend sortiert, sodass das leichteste Item zuerst erscheint.

## Auswertungen

### Gesamtgewicht

Die Funktion `gesamtGewicht()` addiert das Gewicht aller Items.

### Tragelimit

`kannTragen()` vergleicht das Gesamtgewicht mit einem frei eingegebenen Maximalgewicht.

### Stärkstes Item

Zuerst wird die höchste Stärke ermittelt. Anschließend wird das passende Item mit `std::find_if` gesucht.

### Bestes Item

Das beste Item wird anhand des Verhältnisses

```text
Wert / Gewicht
```

bestimmt.

## Datei-I/O

Das Inventar wird in der Datei

```text
inventar.txt
```

gespeichert.

Eine Zeile besitzt dieses Format:

```text
Name;Typ;Wert;Gewicht;Staerke
```

Beispiel:

```text
Eisenschwert;1;120;8;25
```

Zum Schreiben wird `ofstream`, zum Lesen `ifstream` verwendet.

## Bedienung

Beim Start erscheint ein Konsolenmenü:

```text
========================================
          RPG INVENTARSYSTEM
========================================
1  Inventar anzeigen
2  Item hinzufuegen
3  Item entfernen
4  Nach Wert sortieren
5  Nach Gewicht sortieren
6  Items pro Typ anzeigen
7  Staerkstes Item anzeigen
8  Bestes Item anzeigen
9  Tragelimit pruefen
10 Speichern
11 Laden
12 Beenden
----------------------------------------
Auswahl:
```

## Kompilieren

Benötigt wird ein C++17-kompatibler Compiler.

### g++

```bash
g++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp -o RPG_Inventarsystem
```

### Start unter Linux/macOS

```bash
./RPG_Inventarsystem
```

Unter Windows kann das Programm beispielsweise mit g++, MinGW oder Visual Studio kompiliert werden.

## Verbesserungen in Version 1.1

- vollständige Head-Beschreibung ergänzt
- Quellcode strukturiert und ausführlich kommentiert
- C-Style-Casts durch `static_cast` ersetzt
- Ausgabe des Hauptmenüs übersichtlicher gestaltet
- Rückmeldungen beim Hinzufügen und Sortieren ergänzt
- leeres Inventar bei Auswertungen berücksichtigt
- ungültige Menüauswahl abgefangen
- Eingabe des Item-Typs auf Werte 1 bis 3 begrenzt
- Fehlerbehandlung beim Öffnen der Speicherdatei ergänzt
- unvollständige Speicherzeilen werden beim Laden übersprungen

## Kurzbeschreibung für die Vorstellung

1. `Item` beschreibt einen Gegenstand.
2. Das komplette Inventar liegt in einem `vector<Item>`.
3. Funktionen verändern oder analysieren diesen Vector.
4. `sort` übernimmt die Sortierung.
5. `map` zählt die Gegenstandstypen.
6. `find_if` sucht ein Item anhand einer Bedingung.
7. `enum class` definiert die zulässigen Item-Typen.
8. Datei-I/O speichert und lädt das Inventar dauerhaft.
9. Das Hauptmenü verbindet alle Funktionen zu einem vollständigen Konsolenprogramm.

## Copyright

© 2026 Julian Krauß. Alle Rechte vorbehalten.

Verwendung im Rahmen der schulischen Ausbildung.
