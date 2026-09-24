/*
================================================================================
JULIAN KRAUSS                                                    JK / ITA 09/25
PROGRAMMIERTECHNIK                                      PROJECT ID: RPGINV-2026
================================================================================

                         RPG-INVENTARSYSTEM
                  C++ | STRUKTUREN | STL | DATEI-I/O

--------------------------------------------------------------------------------
CORPORATE DESIGN
--------------------------------------------------------------------------------

Designstil     : First Class / technisch / klar
Akzentfarbe    : Gold
Sekundärfarbe  : Blau
Basisfarben    : Weiß / Grau
Statusfarben   : Grün = erfolgreich | Rot = Fehler / Beenden

Hinweis:
Die sichtbaren Farben werden während der Programmausführung über ANSI-
Farbcodes erzeugt. Die Darstellung der Kommentare im Quellcode selbst
wird weiterhin vom verwendeten Editor bzw. Visual-Studio-Theme bestimmt.

--------------------------------------------------------------------------------
PROJEKTINFORMATION
--------------------------------------------------------------------------------

Projekt        : RPG-Inventarsystem
Datei          : RPG_Inventarsystem.cpp
Autor          : Julian Krauß
Klasse         : ITA 09/25
Fach           : Programmiertechnik
Datum          : 24.09.2026
Version        : 1.1 / Corporate Design
Projekt-ID     : JK-PT-RPGINV-2026-001

--------------------------------------------------------------------------------
KURZBESCHREIBUNG
--------------------------------------------------------------------------------

Dieses Programm verwaltet das Inventar eines kleinen Rollenspiels.
Gegenstände werden als strukturierte Datensätze gespeichert und innerhalb
eines std::vector verwaltet.

Das System unterstützt das Hinzufügen, Entfernen, Sortieren, Auswerten,
Speichern und Laden von Items. Zum Einsatz kommen unter anderem struct,
enum class, std::vector, std::sort, Lambda-Funktionen, std::map,
std::find_if sowie Datei-I/O.

================================================================================
COPYRIGHT & PROJECT IDENTITY
================================================================================

© 2026 Julian Krauß
Alle Rechte vorbehalten.

JK PROJECT IDENTITY  |  ITA 09/25  |  RPGINV 2026  |  VERSION 1.1

================================================================================
*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <iomanip>
#include <fstream>
#include <limits>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

using namespace std;


// -----------------------------------------------------------------------------
// Konsolen-Design
// -----------------------------------------------------------------------------
namespace Design
{
    const string RESET   = "\033[0m";
    const string GOLD    = "\033[38;5;220m";
    const string BLUE    = "\033[38;5;39m";
    const string WHITE   = "\033[97m";
    const string GREY    = "\033[90m";
    const string GREEN   = "\033[92m";
    const string RED     = "\033[91m";
    const string CYAN    = "\033[96m";
    const string BOLD    = "\033[1m";

    void farbenAktivieren()
    {
#ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

        if (hOut != INVALID_HANDLE_VALUE)
        {
            DWORD mode = 0;

            if (GetConsoleMode(hOut, &mode))
            {
                SetConsoleMode(
                    hOut,
                    mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING
                );
            }
        }
#endif
    }

    void linie(char zeichen = '=')
    {
        cout << GOLD << string(72, zeichen) << RESET << endl;
    }

    void titel()
    {
        cout << endl;
        linie('=');

        cout << GOLD << BOLD
             << "                     JULIAN KRAUSS | IT PROJECT"
             << RESET << endl;

        cout << WHITE << BOLD
             << "                        RPG-INVENTARSYSTEM"
             << RESET << endl;

        cout << GREY
             << "                   C++ | STL | STRUCT | FILE I/O"
             << RESET << endl;

        linie('=');
        cout << endl;
    }
}


// Mögliche Item-Typen im Inventar
enum class Typ
{
    Waffe = 1,
    Traenke,
    Ruestung
};

// Aufbau eines einzelnen Gegenstands
struct Item
{
    string name;
    Typ type;
    int wert;
    int gewicht;
    int staerke;
};

// Wandelt den enum-Typ in einen lesbaren Text um
string typZuText(Typ typ)
{
    switch (typ)
    {
        case Typ::Waffe:
            return "Waffe";

        case Typ::Traenke:
            return "Traenke";

        case Typ::Ruestung:
            return "Ruestung";
    }

    return "Unbekannt";
}

// Wandelt die Menüauswahl in einen Item-Typ um
Typ zahlZuTyp(int zahl)
{
    switch (zahl)
    {
        case 1:
            return Typ::Waffe;

        case 2:
            return Typ::Traenke;

        case 3:
            return Typ::Ruestung;

        default:
            return Typ::Waffe;
    }
}

// Liest eine Zahl ein und prüft den erlaubten Wertebereich
int zahlEinlesen(const string& text, int min, int max)
{
    int zahl;

    while (true)
    {
        cout << text;

        if (cin >> zahl && zahl >= min && zahl <= max)
        {
            return zahl;
        }

        cout << "Ungueltige Eingabe. Bitte erneut versuchen." << endl;

        cin.clear();
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');
    }
}

// Liest eine komplette Textzeile ein
string textEinlesen(const string& text)
{
    string eingabe;

    cout << text;
    cin >> ws;
    getline(cin, eingabe);

    return eingabe;
}

// Prüft, ob ein Item mit diesem Namen bereits existiert
bool nameVorhanden(const vector<Item>& inv, const string& name)
{
    return find_if(inv.begin(), inv.end(),
        [&name](const Item& item)
        {
            return item.name == name;
        }) != inv.end();
}

// Fügt ein neues Item zum Inventar hinzu
void itemHinzufuegen(vector<Item>& inv, Item neu)
{
    if (nameVorhanden(inv, neu.name))
    {
        cout << "Ein Item mit diesem Namen ist bereits vorhanden." << endl;
        return;
    }

    inv.push_back(neu);
    cout << Design::GREEN << "Item wurde hinzugefuegt." << Design::RESET << endl;
}

// Sucht ein Item anhand des Namens und entfernt es
void itemEntfernen(vector<Item>& inv, const string& name)
{
    auto gefunden = find_if(inv.begin(), inv.end(),
        [&name](const Item& item)
        {
            return item.name == name;
        });

    if (gefunden == inv.end())
    {
        cout << "Item wurde nicht gefunden." << endl;
        return;
    }

    inv.erase(gefunden);
    cout << Design::GREEN << "Item wurde entfernt." << Design::RESET << endl;
}

// Berechnet das Gesamtgewicht aller Items
int gesamtGewicht(const vector<Item>& inv)
{
    int gesamt = 0;

    for (const Item& item : inv)
    {
        gesamt += item.gewicht;
    }

    return gesamt;
}

// Prüft, ob das Gewicht noch innerhalb des Limits liegt
bool kannTragen(const vector<Item>& inv, int maxGewicht)
{
    return gesamtGewicht(inv) <= maxGewicht;
}

// Ermittelt das Item mit dem besten Wert-Gewicht-Verhältnis
Item bestesItem(const vector<Item>& inv)
{
    Item bestes = inv[0];

    double bestesVerhaeltnis;

    if (bestes.gewicht == 0)
        bestesVerhaeltnis = bestes.wert;
    else
        bestesVerhaeltnis = static_cast<double>(bestes.wert) / bestes.gewicht;

    for (size_t i = 1; i < inv.size(); i++)
    {
        double verhaeltnis;

        if (inv[i].gewicht == 0)
            verhaeltnis = inv[i].wert;
        else
            verhaeltnis = static_cast<double>(inv[i].wert) / inv[i].gewicht;

        if (verhaeltnis > bestesVerhaeltnis)
        {
            bestes = inv[i];
            bestesVerhaeltnis = verhaeltnis;
        }
    }

    return bestes;
}

// Gibt das Inventar als übersichtliche Tabelle aus
void inventarAusgeben(const vector<Item>& inv)
{
    if (inv.empty())
    {
        cout << "\nDas Inventar ist leer." << endl;
        return;
    }

    cout << "\n";
    cout << left
         << setw(22) << "Name"
         << setw(14) << "Typ"
         << setw(10) << "Wert"
         << setw(12) << "Gewicht"
         << setw(10) << "Staerke"
         << endl;

    cout << string(68, '-') << endl;

    for (const Item& item : inv)
    {
        cout << left
             << setw(22) << item.name
             << setw(14) << typZuText(item.type)
             << setw(10) << item.wert
             << setw(12) << item.gewicht
             << setw(10) << item.staerke
             << endl;
    }

    cout << string(68, '-') << endl;
    cout << "Gesamtgewicht: " << gesamtGewicht(inv) << endl;
}

// Sortiert das Inventar vom höchsten zum niedrigsten Wert
void nachWertSortieren(vector<Item>& inv)
{
    sort(inv.begin(), inv.end(),
        [](const Item& a, const Item& b)
        {
            return a.wert > b.wert;
        });

    cout << "Inventar wurde nach Wert sortiert." << endl;
}

// Sortiert das Inventar vom leichtesten zum schwersten Item
void nachGewichtSortieren(vector<Item>& inv)
{
    sort(inv.begin(), inv.end(),
        [](const Item& a, const Item& b)
        {
            return a.gewicht < b.gewicht;
        });

    cout << "Inventar wurde nach Gewicht sortiert." << endl;
}

// Zählt mit einer map, wie viele Items pro Typ vorhanden sind
void typenZaehlen(const vector<Item>& inv)
{
    map<string, int> anzahl;

    for (const Item& item : inv)
    {
        anzahl[typZuText(item.type)]++;
    }

    cout << "\nItems pro Typ:" << endl;

    if (anzahl.empty())
    {
        cout << "Keine Items vorhanden." << endl;
        return;
    }

    for (const auto& eintrag : anzahl)
    {
        cout << setw(14) << left << eintrag.first
             << ": " << eintrag.second << endl;
    }
}

// Sucht das stärkste Item und gibt es aus
void staerkstesItemAnzeigen(const vector<Item>& inv)
{
    if (inv.empty())
    {
        cout << "Das Inventar ist leer." << endl;
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

    auto gefunden = find_if(inv.begin(), inv.end(),
        [maxStaerke](const Item& item)
        {
            return item.staerke == maxStaerke;
        });

    if (gefunden != inv.end())
    {
        cout << "Staerkstes Item: "
             << gefunden->name
             << " | Staerke: "
             << gefunden->staerke
             << endl;
    }
}

// Gibt das Item mit dem besten Wert-Gewicht-Verhältnis aus
void bestesItemAnzeigen(const vector<Item>& inv)
{
    if (inv.empty())
    {
        cout << "Das Inventar ist leer." << endl;
        return;
    }

    Item bestes = bestesItem(inv);

    double verhaeltnis = 0.0;

    if (bestes.gewicht != 0)
    {
        verhaeltnis =
            static_cast<double>(bestes.wert) / bestes.gewicht;
    }

    cout << fixed << setprecision(2);
    cout << "Bestes Item: " << bestes.name
         << " | Wert/Gewicht: " << verhaeltnis
         << endl;
}

// Prüft ein frei eingegebenes maximales Tragelimit
void tragelimitPruefen(const vector<Item>& inv)
{
    int maxGewicht = zahlEinlesen(
        "Maximales Gewicht eingeben: ", 0, 1000000
    );

    cout << "Aktuelles Gesamtgewicht: "
         << gesamtGewicht(inv)
         << endl;

    if (kannTragen(inv, maxGewicht))
    {
        cout << Design::GREEN << "Das Inventar kann getragen werden." << Design::RESET << endl;
    }
    else
    {
        cout << Design::RED << "Das Inventar ist zu schwer." << Design::RESET << endl;
    }
}

// Speichert alle Items zeilenweise in einer Textdatei
void inventarSpeichern(const vector<Item>& inv, const string& dateiname)
{
    ofstream datei(dateiname);

    if (!datei)
    {
        cout << "Die Datei konnte nicht geoeffnet werden." << endl;
        return;
    }

    for (const Item& item : inv)
    {
        datei << item.name << ";"
              << static_cast<int>(item.type) << ";"
              << item.wert << ";"
              << item.gewicht << ";"
              << item.staerke
              << '\n';
    }

    cout << Design::GREEN << "Inventar wurde gespeichert." << Design::RESET << endl;
}

// Lädt vorhandene Items aus der Textdatei
void inventarLaden(vector<Item>& inv, const string& dateiname)
{
    ifstream datei(dateiname);

    if (!datei)
    {
        cout << "Noch keine Speicherdatei vorhanden." << endl;
        return;
    }

    vector<Item> geladenesInventar;
    string zeile;

    while (getline(datei, zeile))
    {
        if (zeile.empty())
            continue;

        size_t pos1 = zeile.find(';');
        size_t pos2 = zeile.find(';', pos1 + 1);
        size_t pos3 = zeile.find(';', pos2 + 1);
        size_t pos4 = zeile.find(';', pos3 + 1);

        if (pos1 == string::npos ||
            pos2 == string::npos ||
            pos3 == string::npos ||
            pos4 == string::npos)
        {
            continue;
        }

        Item item;

        try
        {
            item.name = zeile.substr(0, pos1);

            int typ = stoi(
                zeile.substr(pos1 + 1, pos2 - pos1 - 1)
            );

            item.wert = stoi(
                zeile.substr(pos2 + 1, pos3 - pos2 - 1)
            );

            item.gewicht = stoi(
                zeile.substr(pos3 + 1, pos4 - pos3 - 1)
            );

            item.staerke = stoi(
                zeile.substr(pos4 + 1)
            );

            if (typ < 1 || typ > 3)
                continue;

            item.type = zahlZuTyp(typ);

            if (!nameVorhanden(geladenesInventar, item.name))
            {
                geladenesInventar.push_back(item);
            }
        }
        catch (...)
        {
            continue;
        }
    }

    inv = geladenesInventar;

    cout << Design::GREEN << "Inventar wurde geladen." << Design::RESET << endl;
}

// Fragt die Daten für ein neues Item über die Konsole ab
void neuesItemEingeben(vector<Item>& inv)
{
    Item neu;

    cout << "\n--- Neues Item ---" << endl;

    neu.name = textEinlesen("Name: ");

    cout << "\nTyp auswaehlen:" << endl;
    cout << "1. Waffe" << endl;
    cout << "2. Traenke" << endl;
    cout << "3. Ruestung" << endl;

    int typAuswahl = zahlEinlesen("Auswahl: ", 1, 3);

    neu.type = zahlZuTyp(typAuswahl);
    neu.wert = zahlEinlesen("Wert in Gold: ", 0, 1000000);
    neu.gewicht = zahlEinlesen("Gewicht: ", 0, 1000000);
    neu.staerke = zahlEinlesen("Staerke: ", 0, 1000000);

    itemHinzufuegen(inv, neu);
}

// Zeigt das Hauptmenü an
void menueAnzeigen()
{
    cout << endl;

    Design::linie('-');

    cout << Design::GOLD << Design::BOLD
         << "  RPG INVENTARSYSTEM"
         << Design::RESET << endl;

    cout << Design::GREY
         << "  Julian Krauss | Programmiertechnik | ITA 09/25"
         << Design::RESET << endl;

    Design::linie('-');

    cout << Design::BLUE  << "  [01] " << Design::WHITE << "Inventar anzeigen"          << Design::RESET << endl;
    cout << Design::BLUE  << "  [02] " << Design::WHITE << "Item hinzufuegen"           << Design::RESET << endl;
    cout << Design::BLUE  << "  [03] " << Design::WHITE << "Item entfernen"             << Design::RESET << endl;
    cout << Design::BLUE  << "  [04] " << Design::WHITE << "Nach Wert sortieren"        << Design::RESET << endl;
    cout << Design::BLUE  << "  [05] " << Design::WHITE << "Nach Gewicht sortieren"     << Design::RESET << endl;
    cout << Design::BLUE  << "  [06] " << Design::WHITE << "Items pro Typ anzeigen"     << Design::RESET << endl;
    cout << Design::BLUE  << "  [07] " << Design::WHITE << "Staerkstes Item anzeigen"   << Design::RESET << endl;
    cout << Design::BLUE  << "  [08] " << Design::WHITE << "Bestes Item anzeigen"       << Design::RESET << endl;
    cout << Design::BLUE  << "  [09] " << Design::WHITE << "Tragelimit pruefen"         << Design::RESET << endl;
    cout << Design::CYAN  << "  [10] " << Design::WHITE << "Inventar speichern"         << Design::RESET << endl;
    cout << Design::CYAN  << "  [11] " << Design::WHITE << "Inventar laden"             << Design::RESET << endl;
    cout << Design::RED   << "  [12] " << Design::WHITE << "Programm beenden"           << Design::RESET << endl;

    Design::linie('-');
}

// Start des Programms
int main()
{
    Design::farbenAktivieren();
    Design::titel();

    vector<Item> inventar;
    const string dateiname = "inventar.txt";

    // Vorhandenen Spielstand beim Start laden
    inventarLaden(inventar, dateiname);

    int auswahl;

    do
    {
        menueAnzeigen();
        auswahl = zahlEinlesen("Auswahl: ", 1, 12);

        cout << endl;

        // Gewählte Menüfunktion ausführen
        switch (auswahl)
        {
            case 1:
                inventarAusgeben(inventar);
                break;

            case 2:
                neuesItemEingeben(inventar);
                break;

            case 3:
            {
                string name = textEinlesen(
                    "Name des Items, das entfernt werden soll: "
                );

                itemEntfernen(inventar, name);
                break;
            }

            case 4:
                nachWertSortieren(inventar);
                break;

            case 5:
                nachGewichtSortieren(inventar);
                break;

            case 6:
                typenZaehlen(inventar);
                break;

            case 7:
                staerkstesItemAnzeigen(inventar);
                break;

            case 8:
                bestesItemAnzeigen(inventar);
                break;

            case 9:
                tragelimitPruefen(inventar);
                break;

            case 10:
                inventarSpeichern(inventar, dateiname);
                break;

            case 11:
                inventarLaden(inventar, dateiname);
                break;

            case 12:
                // Beim Beenden automatisch speichern
                inventarSpeichern(inventar, dateiname);
                cout << Design::GOLD << "Programm wird beendet." << Design::RESET << endl;
                break;
        }

    } while (auswahl != 12);

    return 0;
}
