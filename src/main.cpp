/*
===============================================================================
 RPG-INVENTARSYSTEM
===============================================================================

 Projekt:        RPG-Inventarsystem
 Fach:           Programmiertechnik
 Ausbildung:     Informationstechnischer Assistent (ITA)
 Klasse:         ITA 09/25
 Sprache:        C++
 Version:        1.1
 Stand:          01.10.2026
 Autor:          Julian Krauß

 Beschreibung:
 Dieses Konsolenprogramm verwaltet ein Inventar für ein Rollenspiel.
 Gegenstände können hinzugefügt, entfernt, angezeigt, sortiert, ausgewertet,
 gespeichert und wieder geladen werden.

 Verwendete C++-Techniken:
 - struct
 - enum class
 - std::vector
 - std::map
 - std::sort
 - std::find_if
 - Lambda-Ausdrücke
 - Referenzen und const
 - Datei-Ein-/Ausgabe mit ifstream und ofstream
 - stringstream
 - formatierte Konsolenausgabe mit iomanip

 Hauptfunktionen:
  1. Inventar anzeigen
  2. Item hinzufügen
  3. Item entfernen
  4. Nach Wert sortieren
  5. Nach Gewicht sortieren
  6. Items pro Typ anzeigen
  7. Stärkstes Item anzeigen
  8. Bestes Item anhand Wert/Gewicht anzeigen
  9. Tragelimit prüfen
 10. Inventar speichern
 11. Inventar laden
 12. Programm beenden

 Datenspeicherung:
 Das Inventar wird in der Datei "inventar.txt" gespeichert.
 Die Werte werden mit Semikolon voneinander getrennt.

 Copyright:
 © 2026 Julian Krauß
 Alle Rechte vorbehalten.
 Verwendung im Rahmen der schulischen Ausbildung.

===============================================================================
*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;

// ============================================================================
// ITEM-TYPEN
// ============================================================================

// Legt die erlaubten Kategorien eines Gegenstands fest.
enum class Typ
{
    Waffe = 1,
    Traenke,
    Ruestung
};

// ============================================================================
// DATENSTRUKTUR
// ============================================================================

// Speichert alle Eigenschaften eines einzelnen Inventar-Gegenstands.
struct Item
{
    string name;
    Typ type;
    int wert;
    int gewicht;
    int staerke;
};

// ============================================================================
// HILFSFUNKTIONEN
// ============================================================================

// Wandelt einen Enum-Wert in einen lesbaren Text um.
string typZuText(Typ typ)
{
    if (typ == Typ::Waffe)
        return "Waffe";

    if (typ == Typ::Traenke)
        return "Traenke";

    return "Ruestung";
}

// Fügt ein neues Item am Ende des Inventars ein.
void itemHinzufuegen(vector<Item>& inv, Item neu)
{
    inv.push_back(neu);
}

// Sucht ein Item anhand seines Namens und entfernt den ersten Treffer.
void itemEntfernen(vector<Item>& inv, const string& name)
{
    for (size_t i = 0; i < inv.size(); i++)
    {
        if (inv[i].name == name)
        {
            inv.erase(inv.begin() + i);
            cout << "Item entfernt.\n";
            return;
        }
    }

    cout << "Item nicht gefunden.\n";
}

// Addiert die Gewichte aller Gegenstände im Inventar.
int gesamtGewicht(const vector<Item>& inv)
{
    int gesamt = 0;

    for (const Item& item : inv)
    {
        gesamt += item.gewicht;
    }

    return gesamt;
}

// Prüft, ob das aktuelle Inventar innerhalb eines vorgegebenen Tragelimits liegt.
bool kannTragen(const vector<Item>& inv, int maxGewicht)
{
    return gesamtGewicht(inv) <= maxGewicht;
}

// Ermittelt das Item mit dem besten Verhältnis von Wert zu Gewicht.
// Voraussetzung: Das Inventar darf beim Aufruf nicht leer sein.
Item bestesItem(const vector<Item>& inv)
{
    Item bestes = inv[0];

    for (const Item& item : inv)
    {
        double aktuell;
        double bestesVerhaeltnis;

        // Eine Division durch 0 wird verhindert.
        if (item.gewicht == 0)
            aktuell = item.wert;
        else
            aktuell = static_cast<double>(item.wert) / item.gewicht;

        if (bestes.gewicht == 0)
            bestesVerhaeltnis = bestes.wert;
        else
            bestesVerhaeltnis =
                static_cast<double>(bestes.wert) / bestes.gewicht;

        if (aktuell > bestesVerhaeltnis)
        {
            bestes = item;
        }
    }

    return bestes;
}

// ============================================================================
// AUSGABE UND SORTIERUNG
// ============================================================================

// Gibt alle Gegenstände formatiert als Tabelle aus.
void inventarAnzeigen(const vector<Item>& inv)
{
    if (inv.empty())
    {
        cout << "Inventar ist leer.\n";
        return;
    }

    cout << left
         << setw(20) << "Name"
         << setw(12) << "Typ"
         << setw(8) << "Wert"
         << setw(10) << "Gewicht"
         << setw(10) << "Staerke"
         << '\n';

    cout << string(60, '-') << '\n';

    for (const Item& item : inv)
    {
        cout << left
             << setw(20) << item.name
             << setw(12) << typZuText(item.type)
             << setw(8) << item.wert
             << setw(10) << item.gewicht
             << setw(10) << item.staerke
             << '\n';
    }

    cout << "Gesamtgewicht: " << gesamtGewicht(inv) << "\n";
}

// Sortiert das Inventar absteigend nach Wert.
void nachWertSortieren(vector<Item>& inv)
{
    sort(
        inv.begin(),
        inv.end(),
        [](const Item& a, const Item& b)
        {
            return a.wert > b.wert;
        }
    );
}

// Sortiert das Inventar aufsteigend nach Gewicht.
void nachGewichtSortieren(vector<Item>& inv)
{
    sort(
        inv.begin(),
        inv.end(),
        [](const Item& a, const Item& b)
        {
            return a.gewicht < b.gewicht;
        }
    );
}

// Zählt mit einer map, wie viele Items pro Gegenstandstyp vorhanden sind.
void typenAnzeigen(const vector<Item>& inv)
{
    map<string, int> anzahl;

    for (const Item& item : inv)
    {
        anzahl[typZuText(item.type)]++;
    }

    if (anzahl.empty())
    {
        cout << "Inventar ist leer.\n";
        return;
    }

    for (const auto& eintrag : anzahl)
    {
        cout << eintrag.first << ": " << eintrag.second << '\n';
    }
}

// Ermittelt zunächst die höchste Stärke und sucht anschließend
// mit find_if das erste Item mit genau diesem Wert.
void staerkstesItemAnzeigen(const vector<Item>& inv)
{
    if (inv.empty())
    {
        cout << "Inventar ist leer.\n";
        return;
    }

    int maxStaerke = inv[0].staerke;

    for (const Item& item : inv)
    {
        if (item.staerke > maxStaerke)
        {
            maxStaerke = item.staerke;
        }
    }

    auto gefunden = find_if(
        inv.begin(),
        inv.end(),
        [maxStaerke](const Item& item)
        {
            return item.staerke == maxStaerke;
        }
    );

    cout << "Staerkstes Item: "
         << gefunden->name
         << " ("
         << gefunden->staerke
         << ")\n";
}

// ============================================================================
// DATEIVERARBEITUNG
// ============================================================================

// Speichert das vollständige Inventar in einer Textdatei.
// Format: Name;Typ;Wert;Gewicht;Staerke
void inventarSpeichern(const vector<Item>& inv, const string& dateiname)
{
    ofstream datei(dateiname);

    if (!datei)
    {
        cout << "Fehler: Datei konnte nicht zum Speichern geoeffnet werden.\n";
        return;
    }

    for (const Item& item : inv)
    {
        datei << item.name << ';'
              << static_cast<int>(item.type) << ';'
              << item.wert << ';'
              << item.gewicht << ';'
              << item.staerke << '\n';
    }
}

// Lädt ein zuvor gespeichertes Inventar aus einer Textdatei.
void inventarLaden(vector<Item>& inv, const string& dateiname)
{
    ifstream datei(dateiname);

    // Beim ersten Programmstart existiert die Datei möglicherweise noch nicht.
    if (!datei)
    {
        return;
    }

    inv.clear();

    string zeile;

    while (getline(datei, zeile))
    {
        stringstream ss(zeile);

        string name;
        string typ;
        string wert;
        string gewicht;
        string staerke;

        getline(ss, name, ';');
        getline(ss, typ, ';');
        getline(ss, wert, ';');
        getline(ss, gewicht, ';');
        getline(ss, staerke, ';');

        // Unvollständige Zeilen werden übersprungen.
        if (name.empty() || typ.empty() || wert.empty()
            || gewicht.empty() || staerke.empty())
        {
            continue;
        }

        Item item;

        item.name = name;
        item.type = static_cast<Typ>(stoi(typ));
        item.wert = stoi(wert);
        item.gewicht = stoi(gewicht);
        item.staerke = stoi(staerke);

        inv.push_back(item);
    }
}

// ============================================================================
// BENUTZEREINGABE
// ============================================================================

// Liest die Eigenschaften eines neuen Items über die Konsole ein.
Item itemEingeben()
{
    Item neu;
    int typ;

    cout << "Name: ";
    cin >> ws;
    getline(cin, neu.name);

    cout << "Typ (1=Waffe, 2=Traenke, 3=Ruestung): ";
    cin >> typ;

    // Nur gültige Typ-Werte akzeptieren.
    while (typ < 1 || typ > 3)
    {
        cout << "Ungueltiger Typ. Bitte 1, 2 oder 3 eingeben: ";
        cin >> typ;
    }

    cout << "Wert: ";
    cin >> neu.wert;

    cout << "Gewicht: ";
    cin >> neu.gewicht;

    cout << "Staerke: ";
    cin >> neu.staerke;

    neu.type = static_cast<Typ>(typ);

    return neu;
}

// Zeigt das Hauptmenü an.
void menue()
{
    cout << "\n";
    cout << "========================================\n";
    cout << "          RPG INVENTARSYSTEM\n";
    cout << "========================================\n";
    cout << "1  Inventar anzeigen\n";
    cout << "2  Item hinzufuegen\n";
    cout << "3  Item entfernen\n";
    cout << "4  Nach Wert sortieren\n";
    cout << "5  Nach Gewicht sortieren\n";
    cout << "6  Items pro Typ anzeigen\n";
    cout << "7  Staerkstes Item anzeigen\n";
    cout << "8  Bestes Item anzeigen\n";
    cout << "9  Tragelimit pruefen\n";
    cout << "10 Speichern\n";
    cout << "11 Laden\n";
    cout << "12 Beenden\n";
    cout << "----------------------------------------\n";
    cout << "Auswahl: ";
}

// ============================================================================
// HAUPTPROGRAMM
// ============================================================================

int main()
{
    vector<Item> inventar;

    const string dateiname = "inventar.txt";

    // Gespeicherte Daten beim Programmstart laden.
    inventarLaden(inventar, dateiname);

    int auswahl = 0;

    // Das Hauptmenü läuft, bis der Benutzer Option 12 auswählt.
    while (auswahl != 12)
    {
        menue();
        cin >> auswahl;

        if (auswahl == 1)
        {
            inventarAnzeigen(inventar);
        }
        else if (auswahl == 2)
        {
            Item neuesItem = itemEingeben();
            itemHinzufuegen(inventar, neuesItem);

            cout << "Item hinzugefuegt.\n";
        }
        else if (auswahl == 3)
        {
            string name;

            cout << "Name: ";
            cin >> ws;
            getline(cin, name);

            itemEntfernen(inventar, name);
        }
        else if (auswahl == 4)
        {
            nachWertSortieren(inventar);
            cout << "Inventar nach Wert sortiert.\n";
        }
        else if (auswahl == 5)
        {
            nachGewichtSortieren(inventar);
            cout << "Inventar nach Gewicht sortiert.\n";
        }
        else if (auswahl == 6)
        {
            typenAnzeigen(inventar);
        }
        else if (auswahl == 7)
        {
            staerkstesItemAnzeigen(inventar);
        }
        else if (auswahl == 8)
        {
            if (inventar.empty())
            {
                cout << "Inventar ist leer.\n";
            }
            else
            {
                Item bestes = bestesItem(inventar);

                cout << "Bestes Item: "
                     << bestes.name
                     << '\n';
            }
        }
        else if (auswahl == 9)
        {
            int maxGewicht;

            cout << "Maximales Gewicht: ";
            cin >> maxGewicht;

            if (kannTragen(inventar, maxGewicht))
            {
                cout << "Inventar kann getragen werden.\n";
            }
            else
            {
                cout << "Inventar ist zu schwer.\n";
            }
        }
        else if (auswahl == 10)
        {
            inventarSpeichern(inventar, dateiname);
            cout << "Gespeichert.\n";
        }
        else if (auswahl == 11)
        {
            inventarLaden(inventar, dateiname);
            cout << "Geladen.\n";
        }
        else if (auswahl != 12)
        {
            cout << "Ungueltige Auswahl.\n";
        }
    }

    // Beim regulären Beenden wird der aktuelle Stand automatisch gespeichert.
    inventarSpeichern(inventar, dateiname);

    cout << "Programm beendet.\n";

    return 0;
}
