# RPG-Inventarsystem

Ein einfaches C++-Konsolenprogramm für die Verwaltung eines Rollenspiel-Inventars.

## Projekt

- Autor: Julian Krauß
- Klasse: ITA 09/25
- Fach: Programmiertechnik
- Sprache: C++
- Stand: 01.10.2026

## Funktionen

Das Programm kann:

- Items anzeigen
- Items hinzufügen und entfernen
- Gesamtgewicht berechnen
- Tragelimit prüfen
- nach Wert sortieren
- nach Gewicht sortieren
- Items nach Typ zählen
- stärkstes Item suchen
- bestes Wert-Gewicht-Verhältnis bestimmen
- Inventar speichern und laden

## Verwendete C++-Techniken

Die Aufgabenstellung verlangt bzw. verwendet:

- `struct Item`
- `vector<Item>`
- `enum class Typ`
- `sort` mit Lambda-Funktion
- `map<string, int>`
- `find_if`
- Funktionen mit Referenzen
- Datei-I/O mit `ifstream` und `ofstream`
- `stringstream`
- `setw`

## Grundidee

Alle Gegenstände liegen in einem `vector<Item>`.

Ein Item besitzt:

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

Die einzelnen Funktionen arbeiten mit diesem Inventar.

Beispiele:

```cpp
void itemHinzufuegen(vector<Item>& inv, Item neu);
void itemEntfernen(vector<Item>& inv, const string& name);
int gesamtGewicht(const vector<Item>& inv);
bool kannTragen(const vector<Item>& inv, int maxGewicht);
Item bestesItem(const vector<Item>& inv);
```

## Sortieren

Mit `sort` und einer Lambda-Funktion wird nach Wert oder Gewicht sortiert.

Beispiel:

```cpp
sort(inv.begin(), inv.end(), [](const Item& a, const Item& b)
{
    return a.wert > b.wert;
});
```

## Stärkstes Item

Zuerst wird die höchste Stärke gesucht. Danach findet `find_if` das passende Item.

## Bestes Item

Das beste Item wird über das Verhältnis

```text
Wert / Gewicht
```

ermittelt.

## Speichern

Das Inventar wird in `inventar.txt` gespeichert.

Beispiel:

```text
Eisenschwert;1;120;8;25
```

Bedeutung:

```text
Name;Typ;Wert;Gewicht;Staerke
```

## Kompilieren

```bash
g++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp -o RPG_Inventarsystem
```

## Kurz erklärt

Für die Vorstellung reicht im Grunde:

1. `Item` speichert die Daten eines Gegenstands.
2. Der `vector` speichert alle Items.
3. Funktionen fügen Items hinzu, entfernen sie oder berechnen Werte.
4. `sort` sortiert das Inventar.
5. `map` zählt die Item-Typen.
6. `find_if` sucht das stärkste Item.
7. Datei-I/O speichert und lädt das Inventar.
8. Das Menü ruft die Funktionen auf.

## Copyright

© 2026 Julian Krauß

## Robuste Eingabe und Dateipruefung (November 2026)

- Zahlen werden als komplette Zeilen eingelesen. Eingaben wie `12abc`, fehlende Zahlen und Werte ausserhalb des gueltigen Bereichs werden erneut abgefragt.
- Ein Item braucht einen Namen ohne Semikolon, einen gueltigen Typ (1 bis 3), einen nichtnegativen Wert, ein **positives Gewicht** und eine nichtnegative Staerke.
- Bei einem leeren Inventar wird kein `inv[0]` mehr angesprochen. Das Wert-Gewicht-Verhaeltnis wird ohne Division berechnet.
- Unvollstaendige oder ungueltige Textdateien werden zurueckgewiesen, ohne das bereits geladene Inventar teilweise zu ersetzen.
- Bei sehr grossen Gewichten wird der angezeigte `int`-Gesamtwert auf `INT_MAX` begrenzt; die Tragelimitpruefung verwendet die tatsaechliche Summe.
- Laufzeitdaten stehen in `inventar.txt`, nicht im Repository. Das Speicherformat bleibt `Name;Typ;Wert;Gewicht;Staerke`.

### Automatisierte Pruefung

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror src/main.cpp -o inventar
python3 tests/smoke_test.py ./inventar
```

GitHub Actions fuehrt Debug- und Release-Builds mit GCC und Clang aus. Die Tests pruefen das Konsolenmenue, Eingabevalidierung, Speichern/Laden, fehlerhafte Dateien und Grenzwerte. Eine erfolgreiche CI-Ausfuehrung ist im Tab **Actions** nachpruefbar. Das Schreiben der Datei ist nicht atomar; bei Stromausfall waehrend des Speicherns kann die Datei unvollstaendig sein.
