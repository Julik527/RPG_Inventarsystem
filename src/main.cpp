/*
    RPG-Inventarsystem
    Autor: Julian Krauß
    Fach: Programmiertechnik
    Klasse: ITA 09/25
    Stand: 01.10.2026

    Beschreibung:
    Einfaches C++-Inventarsystem für ein Rollenspiel.
    Items können angezeigt, hinzugefügt, entfernt, sortiert,
    gespeichert und geladen werden.

    © 2026 Julian Krauß
*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <limits>
#include <stdexcept>

using namespace std;

enum class Typ
{
    Waffe = 1,
    Traenke,
    Ruestung
};

struct Item
{
    string name;
    Typ type;
    int wert;
    int gewicht;
    int staerke;
};

// Wandelt den Item-Typ in Text um.
string typZuText(Typ typ)
{
    if (typ == Typ::Waffe) return "Waffe";
    if (typ == Typ::Traenke) return "Traenke";
    return "Ruestung";
}

// Nur gueltige Gegenstaende aufnehmen: so bleibt das Inventar konsistent.
void itemHinzufuegen(vector<Item>& inv, Item neu)
{
    if (neu.name.find_first_not_of(" \t\r\n") == string::npos ||
        neu.name.find(';') != string::npos ||
        neu.wert < 0 || neu.gewicht <= 0 || neu.staerke < 0 ||
        (neu.type != Typ::Waffe && neu.type != Typ::Traenke &&
         neu.type != Typ::Ruestung))
    {
        throw invalid_argument("Ungueltige Item-Daten.");
    }
    inv.push_back(neu);
}

// Entfernt ein Item anhand seines Namens.
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

// Berechnet das Gewicht ohne Ueberlauf des int-Rueckgabewertes.
int gesamtGewicht(const vector<Item>& inv)
{
    long long summe = 0;
    for (const Item& item : inv)
    {
        summe += item.gewicht;
        if (summe > numeric_limits<int>::max())
            return numeric_limits<int>::max(); // Anzeige auf int begrenzt.
    }
    return static_cast<int>(summe);
}

// Fuer das Tragelimit die echte Summe pruefen, auch ueber INT_MAX hinaus.
bool kannTragen(const vector<Item>& inv, int maxGewicht)
{
    if (maxGewicht < 0) return false;
    long long summe = 0;
    for (const Item& item : inv)
    {
        summe += item.gewicht;
        if (summe > maxGewicht) return false;
    }
    return true;
}

// Das beste Wert-Gewicht-Verhaeltnis ohne Division durch null bestimmen.
Item bestesItem(const vector<Item>& inv)
{
    if (inv.empty())
        throw invalid_argument("Das Inventar ist leer.");

    Item bestes = inv.front();
    for (const Item& item : inv)
    {
        // Kreuzmultiplikation vermeidet Rundungsfehler bei Division.
        // Gewicht ist durch itemHinzufuegen immer positiv.
        const long long links = static_cast<long long>(item.wert) * bestes.gewicht;
        const long long rechts = static_cast<long long>(bestes.wert) * item.gewicht;
        if (links > rechts)
            bestes = item;
    }
    return bestes;
}

// Zeigt das Inventar als Tabelle an.
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
         << setw(10) << "Staerke" << '\n';

    cout << string(60, '-') << '\n';

    for (const Item& item : inv)
    {
        cout << left
             << setw(20) << item.name
             << setw(12) << typZuText(item.type)
             << setw(8) << item.wert
             << setw(10) << item.gewicht
             << setw(10) << item.staerke << '\n';
    }

    cout << "Gesamtgewicht: " << gesamtGewicht(inv) << "\n";
}

// Sortiert nach Wert, größter Wert zuerst.
void nachWertSortieren(vector<Item>& inv)
{
    sort(inv.begin(), inv.end(), [](const Item& a, const Item& b)
    {
        return a.wert > b.wert;
    });
}

// Sortiert nach Gewicht, kleinstes Gewicht zuerst.
void nachGewichtSortieren(vector<Item>& inv)
{
    sort(inv.begin(), inv.end(), [](const Item& a, const Item& b)
    {
        return a.gewicht < b.gewicht;
    });
}

// Zählt die Items pro Typ.
void typenAnzeigen(const vector<Item>& inv)
{
    map<string, int> anzahl;

    for (const Item& item : inv)
        anzahl[typZuText(item.type)]++;

    for (const auto& eintrag : anzahl)
        cout << eintrag.first << ": " << eintrag.second << '\n';
}

// Sucht das stärkste Item mit find_if.
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
            maxStaerke = item.staerke;
    }

    auto gefunden = find_if(inv.begin(), inv.end(),
        [maxStaerke](const Item& item)
        {
            return item.staerke == maxStaerke;
        });

    cout << "Staerkstes Item: " << gefunden->name
         << " (" << gefunden->staerke << ")\n";
}

// Laesst nur vollstaendige Ganzzahlen im int-Wertebereich zu.
bool parseGanzzahl(const string& text, int& zahl)
{
    if (text.empty()) return false;
    try
    {
        size_t gelesen = 0;
        long long wert = stoll(text, &gelesen);
        if (gelesen != text.size() ||
            wert < numeric_limits<int>::min() ||
            wert > numeric_limits<int>::max())
            return false;
        zahl = static_cast<int>(wert);
        return true;
    }
    catch (const exception&)
    {
        return false;
    }
}

// Speichert nur dann erfolgreich, wenn auch das Schreiben gelungen ist.
bool inventarSpeichern(const vector<Item>& inv, const string& dateiname)
{
    ofstream datei(dateiname, ios::trunc);
    if (!datei) return false;

    for (const Item& item : inv)
    {
        datei << item.name << ';'
              << static_cast<int>(item.type) << ';'
              << item.wert << ';'
              << item.gewicht << ';'
              << item.staerke << '\n';
    }
    datei.flush();
    return static_cast<bool>(datei);
}

// Fehlerhafte Dateien duerfen das vorhandene Inventar nicht veraendern.
bool inventarLaden(vector<Item>& inv, const string& dateiname)
{
    ifstream datei(dateiname);
    if (!datei) return false;

    vector<Item> geladen;
    string zeile;
    while (getline(datei, zeile))
    {
        stringstream stream(zeile);
        string felder[5];
        for (string& feld : felder)
            if (!getline(stream, feld, ';')) return false;
        if (stream.peek() != char_traits<char>::eof()) return false;

        int typ = 0, wert = 0, gewicht = 0, staerke = 0;
        if (!parseGanzzahl(felder[1], typ) ||
            !parseGanzzahl(felder[2], wert) ||
            !parseGanzzahl(felder[3], gewicht) ||
            !parseGanzzahl(felder[4], staerke) ||
            typ < 1 || typ > 3)
            return false;

        try
        {
            itemHinzufuegen(geladen, {felder[0], static_cast<Typ>(typ),
                                      wert, gewicht, staerke});
        }
        catch (const invalid_argument&)
        {
            return false;
        }
    }
    if (datei.bad()) return false;

    inv = std::move(geladen);
    return true;
}

// Liest die ganze Eingabezeile, damit "12abc" kein gueltiger Wert ist.
int frageGanzzahl(const string& frage, int minimum, int maximum)
{
    for (;;)
    {
        cout << frage;
        string eingabe;
        if (!getline(cin, eingabe))
            throw runtime_error("Die Eingabe wurde beendet.");

        int zahl = 0;
        if (parseGanzzahl(eingabe, zahl) && zahl >= minimum && zahl <= maximum)
            return zahl;
        cout << "Ungueltige Zahl. Bitte erneut eingeben.\n";
    }
}

string frageName(const string& frage)
{
    for (;;)
    {
        cout << frage;
        string name;
        if (!getline(cin, name))
            throw runtime_error("Die Eingabe wurde beendet.");
        if (name.find_first_not_of(" \t\r\n") != string::npos &&
            name.find(';') == string::npos)
            return name;
        cout << "Name darf nicht leer sein oder ein Semikolon enthalten.\n";
    }
}

// Neues Item mit geprueften Eingaben einlesen.
Item itemEingeben()
{
    Item neu;
    neu.name = frageName("Name: ");
    const int typ = frageGanzzahl("Typ (1=Waffe, 2=Traenke, 3=Ruestung): ", 1, 3);
    neu.type = static_cast<Typ>(typ);
    neu.wert = frageGanzzahl("Wert (ab 0): ", 0, numeric_limits<int>::max());
    neu.gewicht = frageGanzzahl("Gewicht (mindestens 1): ", 1, numeric_limits<int>::max());
    neu.staerke = frageGanzzahl("Staerke (ab 0): ", 0, numeric_limits<int>::max());
    return neu;
}

// Zeigt das Menü an.
void menue()
{
    cout << "\n--- RPG Inventarsystem ---\n";
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
}

int main()
{
    vector<Item> inventar;
    const string dateiname = "inventar.txt";
    inventarLaden(inventar, dateiname); // Noch keine Datei: leeres Inventar.

    try
    {
        for (;;)
        {
            menue();
            const int auswahl = frageGanzzahl("Auswahl: ", 1, 12);

            if (auswahl == 1)
                inventarAnzeigen(inventar);
            else if (auswahl == 2)
                itemHinzufuegen(inventar, itemEingeben());
            else if (auswahl == 3)
                itemEntfernen(inventar, frageName("Name: "));
            else if (auswahl == 4)
                nachWertSortieren(inventar);
            else if (auswahl == 5)
                nachGewichtSortieren(inventar);
            else if (auswahl == 6)
                typenAnzeigen(inventar);
            else if (auswahl == 7)
                staerkstesItemAnzeigen(inventar);
            else if (auswahl == 8)
            {
                if (inventar.empty())
                    cout << "Inventar ist leer.\n";
                else
                    cout << "Bestes Item: " << bestesItem(inventar).name << '\n';
            }
            else if (auswahl == 9)
            {
                const int maxGewicht = frageGanzzahl(
                    "Maximales Gewicht: ", 0, numeric_limits<int>::max());
                cout << (kannTragen(inventar, maxGewicht)
                    ? "Inventar kann getragen werden.\n"
                    : "Inventar ist zu schwer.\n");
            }
            else if (auswahl == 10 || auswahl == 12)
            {
                cout << (inventarSpeichern(inventar, dateiname)
                    ? "Inventar gespeichert.\n"
                    : "Warnung: Inventar konnte nicht gespeichert werden.\n");
                if (auswahl == 12) break;
            }
            else if (auswahl == 11)
            {
                cout << (inventarLaden(inventar, dateiname)
                    ? "Inventar geladen.\n"
                    : "Datei fehlt oder ist fehlerhaft. Inventar bleibt unveraendert.\n");
            }
        }
    }
    catch (const exception& fehler)
    {
        cerr << "Fehler: " << fehler.what() << '\n';
        return 1;
    }

    cout << "Programm beendet.\n";
    return 0;
}
