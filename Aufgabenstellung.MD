# Aufgabenstellung – RPG-Inventarsystem in C++

## Ziel

Entwickle ein Inventarsystem für ein kleines Rollenspiel in C++.

Gegenstände sollen verwaltet, sortiert, gefiltert und ausgewertet werden können.

---

## 1. `struct` definieren

```cpp
struct Item
{
    std::string name;
    std::string type;   // "Waffe", "Trank", "Rüstung", ...
    int wert;           // Goldwert
    int gewicht;
    int staerke;        // Angriffs-/Verteidigungswert
};
```

---

## 2. Verwendung von `std::vector`

Lege ein Inventar an:

```cpp
std::vector<Item> inventar;
```

Implementiere folgende Funktionen:

```cpp
void itemHinzufuegen(std::vector<Item>& inv, Item neu);
void itemEntfernen(std::vector<Item>& inv, const std::string& name);
int gesamtGewicht(const std::vector<Item>& inv);
```

---

## 3. STL-Collections und Algorithmen

Folgende Bestandteile der C++ Standard Template Library sollen verwendet werden:

- `std::sort` mit einer Lambda-Funktion, um das Inventar nach Wert oder Gewicht zu sortieren
- `std::map<std::string, int>`, um zu zählen, wie viele Items pro Typ vorhanden sind
- `std::find_if`, um das stärkste Item zu finden
- optional `std::set<std::string>` für eindeutige Item-Namen

---

## 4. Spiel-Logik

Implementiere:

```cpp
bool kannTragen(const std::vector<Item>& inv, int maxGewicht);
```

Die Funktion prüft, ob das Gesamtgewicht des Inventars innerhalb des erlaubten Limits liegt.

Außerdem:

```cpp
Item bestesItem(const std::vector<Item>& inv);
```

Diese Funktion gibt das Item mit dem besten Wert-/Gewicht-Verhältnis zurück.

Das Inventar soll zusätzlich übersichtlich in der Konsole ausgegeben werden, zum Beispiel mit `std::setw`.

---

## 5. Bonus

Zusätzliche Anforderungen:

- Inventar in einer Textdatei speichern
- Inventar aus einer Textdatei laden
- einfaches Konsolenmenü einbauen
  - Item anzeigen
  - Item hinzufügen
  - Item entfernen
  - sortieren
  - speichern
  - laden
  - beenden
- `enum class Typ { Waffe, Traenke, Ruestung }` anstelle eines Strings für den Item-Typ verwenden

---

## Abgabe

Folgende Dateien sollen im GitHub-Repository enthalten sein:

```text
src/main.cpp
README.md
Aufgabenstellung.MD
.gitignore
```

Das Programm muss erfolgreich kompilierbar sein und das Ergebnis muss erklärt werden können.
