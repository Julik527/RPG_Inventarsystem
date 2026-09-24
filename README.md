# RPG-Inventarsystem

Ein kleines Konsolenprogramm in C++, das das Inventar eines Rollenspiels verwaltet.

Das Projekt wurde im Fach **Programmiertechnik** erstellt und zeigt den praktischen Einsatz von Strukturen, `std::vector`, STL-Algorithmen, `std::map`, `enum class` und Datei-I/O.

---

## Projektstruktur

```text
RPG_Inventarsystem/
├── src/
│   └── main.cpp
├── Aufgabenstellung.MD
├── README.md
└── .gitignore
```

Die Datei `inventar.txt` wird erst beim Ausführen des Programms erzeugt und deshalb nicht in Git eingecheckt.

---

## Funktionen

Das Programm kann:

- Items anzeigen
- neue Items hinzufügen
- Items anhand ihres Namens entfernen
- das Gesamtgewicht berechnen
- ein maximales Tragelimit prüfen
- nach Wert sortieren
- nach Gewicht sortieren
- Items nach Typ zählen
- das stärkste Item suchen
- das beste Wert-/Gewicht-Verhältnis bestimmen
- das Inventar in `inventar.txt` speichern
- das Inventar wieder aus der Datei laden
- über ein Konsolenmenü bedient werden

---

## Verwendete C++-Techniken

### `struct Item`

Ein Item besteht aus mehreren zusammengehörenden Eigenschaften:

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

Eine `struct` eignet sich hier, weil alle Daten eines Gegenstands gemeinsam gespeichert werden.

---

### `std::vector<Item>`

Das Inventar wird als dynamische Liste gespeichert:

```cpp
vector<Item> inventar;
```

Im Gegensatz zu einem normalen Array kann ein `vector` während der Programmlaufzeit größer oder kleiner werden.

Neue Items werden mit `push_back()` hinzugefügt.

---

### Referenzen mit `&`

Beispiel:

```cpp
void itemHinzufuegen(vector<Item>& inv, Item neu)
```

Das `&` bedeutet, dass die Funktion direkt mit dem ursprünglichen Inventar arbeitet.

Ohne Referenz würde nur eine Kopie des Vectors verändert werden.

---

### `const`

Beispiel:

```cpp
int gesamtGewicht(const vector<Item>& inv)
```

`const` bedeutet, dass die Funktion das Inventar lesen darf, aber nicht verändern kann.

---

### `enum class Typ`

Die möglichen Item-Typen sind fest definiert:

```cpp
enum class Typ
{
    Waffe = 1,
    Traenke,
    Ruestung
};
```

Dadurch können nicht versehentlich beliebige Texte als Typ verwendet werden.

---

### Sortieren mit `std::sort`

Das Inventar wird mit `std::sort` und einer Lambda-Funktion sortiert.

Beispiel nach Wert:

```cpp
sort(inv.begin(), inv.end(),
    [](const Item& a, const Item& b)
    {
        return a.wert > b.wert;
    });
```

Die Lambda-Funktion entscheidet, welches von zwei Items zuerst einsortiert wird.

---

### Zählen mit `std::map`

Mit einer `map` wird gezählt, wie viele Items eines Typs vorhanden sind.

Beispiel:

```cpp
map<string, int> anzahl;

for (const Item& item : inv)
{
    anzahl[typZuText(item.type)]++;
}
```

Mögliches Ergebnis:

```text
Ruestung: 2
Traenke: 3
Waffe: 4
```

---

### Suchen mit `std::find_if`

`std::find_if` sucht das erste Element, das eine bestimmte Bedingung erfüllt.

Im Projekt wird damit das Item gefunden, dessen Stärke dem zuvor ermittelten höchsten Stärke-Wert entspricht.

---

## Gesamtgewicht

Das Gewicht aller Items wird addiert:

```cpp
int gesamtGewicht(const vector<Item>& inv)
{
    int gesamt = 0;

    for (const Item& item : inv)
    {
        gesamt += item.gewicht;
    }

    return gesamt;
}
```

---

## Tragelimit

Die Funktion

```cpp
bool kannTragen(const vector<Item>& inv, int maxGewicht)
```

vergleicht das aktuelle Gesamtgewicht mit dem erlaubten Maximalgewicht.

Das Ergebnis ist entweder `true` oder `false`.

---

## Bestes Item

Das beste Item wird über das Verhältnis

```text
Wert / Gewicht
```

ermittelt.

Beispiel:

```text
Item A: 200 Gold / 10 Gewicht = 20
Item B: 150 Gold / 3 Gewicht  = 50
```

Obwohl Item A einen höheren Gesamtwert hat, besitzt Item B das bessere Wert-/Gewicht-Verhältnis.

---

## Datei speichern und laden

Das Inventar wird in der Datei

```text
inventar.txt
```

gespeichert.

Eine gespeicherte Zeile sieht beispielsweise so aus:

```text
Eisenschwert;1;120;8;25
```

Die Werte bedeuten:

```text
Name ; Typ ; Wert ; Gewicht ; Stärke
```

Zum Schreiben wird `ofstream` verwendet, zum Lesen `ifstream`.

---

## Konsolenmenü

Beim Start erscheint ein Menü, über das die einzelnen Funktionen ausgewählt werden können.

```text
[01] Inventar anzeigen
[02] Item hinzufügen
[03] Item entfernen
[04] Nach Wert sortieren
[05] Nach Gewicht sortieren
[06] Items pro Typ anzeigen
[07] Stärkstes Item anzeigen
[08] Bestes Item anzeigen
[09] Tragelimit prüfen
[10] Inventar speichern
[11] Inventar laden
[12] Programm beenden
```

Beim Beenden wird das Inventar automatisch gespeichert.

---

## Kompilieren

Benötigt wird ein Compiler mit mindestens **C++17**.

### g++

```bash
g++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp -o RPG_Inventarsystem
```

Start unter Linux/macOS:

```bash
./RPG_Inventarsystem
```

Unter Windows kann das Projekt beispielsweise mit Visual Studio kompiliert und gestartet werden.

---

## Kompilierprüfung

Die vorliegende Version wurde mit folgenden Optionen geprüft:

```text
C++17
-Wall
-Wextra
-pedantic
```

Ergebnis:

```text
Kompilierung erfolgreich
0 Fehler
0 Warnungen
```

---

## Kurz erklärt für die Vorstellung

Wenn das Projekt erklärt werden soll, ist dieser Ablauf sinnvoll:

1. `Item` ist die Datenstruktur für einen Gegenstand.
2. Alle Items liegen gemeinsam in einem `vector`.
3. Funktionen verändern oder untersuchen diesen Vector.
4. `sort` ordnet die Items nach Wert oder Gewicht.
5. `map` zählt die verschiedenen Item-Typen.
6. `find_if` sucht ein bestimmtes Item nach einer Bedingung.
7. `enum class` sorgt für fest definierte und typsichere Item-Typen.
8. `ofstream` und `ifstream` speichern bzw. laden das Inventar.
9. Das Hauptmenü verbindet alle Funktionen zu einem bedienbaren Programm.

Damit sind die wesentlichen Anforderungen der Aufgabe im Programm umgesetzt.

---

## Autor

**Julian Krauß**  
ITA 09/25  
Programmiertechnik

© 2026 Julian Krauß
