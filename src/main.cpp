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

// Fügt ein Item zum Inventar hinzu.
void itemHinzufuegen(vector<Item>& inv, Item neu)
{
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

// Berechnet das Gesamtgewicht.
int gesamtGewicht(const vector<Item>& inv)
{
    int gesamt = 0;

    for (const Item& item : inv)
        gesamt += item.gewicht;

    return gesamt;
}

// Prüft das Tragelimit.
bool kannTragen(const vector<Item>& inv, int maxGewicht)
{
    return gesamtGewicht(inv) <= maxGewicht;
}

// Sucht das Item mit dem besten Wert-Gewicht-Verhältnis.
Item bestesItem(const vector<Item>& inv)
{
    Item bestes = inv[0];

    for (const Item& item : inv)
    {
        double aktuell = (double)item.wert / item.gewicht;
        double bestesVerhaeltnis = (double)bestes.wert / bestes.gewicht;

        if (aktuell > bestesVerhaeltnis)
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

// Speichert das Inventar in einer Textdatei.
void inventarSpeichern(const vector<Item>& inv, const string& dateiname)
{
    ofstream datei(dateiname);

    for (const Item& item : inv)
    {
        datei << item.name << ';'
              << (int)item.type << ';'
              << item.wert << ';'
              << item.gewicht << ';'
              << item.staerke << '\n';
    }
}

// Lädt das Inventar aus einer Textdatei.
void inventarLaden(vector<Item>& inv, const string& dateiname)
{
    ifstream datei(dateiname);

    if (!datei) return;

    inv.clear();
    string zeile;

    while (getline(datei, zeile))
    {
        stringstream ss(zeile);
        string name, typ, wert, gewicht, staerke;

        getline(ss, name, ';');
        getline(ss, typ, ';');
        getline(ss, wert, ';');
        getline(ss, gewicht, ';');
        getline(ss, staerke, ';');

        Item item;
        item.name = name;
        item.type = (Typ)stoi(typ);
        item.wert = stoi(wert);
        item.gewicht = stoi(gewicht);
        item.staerke = stoi(staerke);

        inv.push_back(item);
    }
}

// Liest ein neues Item ein.
Item itemEingeben()
{
    Item neu;
    int typ;

    cout << "Name: ";
    cin >> ws;
    getline(cin, neu.name);

    cout << "Typ (1=Waffe, 2=Traenke, 3=Ruestung): ";
    cin >> typ;

    cout << "Wert: ";
    cin >> neu.wert;

    cout << "Gewicht: ";
    cin >> neu.gewicht;

    cout << "Staerke: ";
    cin >> neu.staerke;

    neu.type = (Typ)typ;

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
    cout << "Auswahl: ";
}

int main()
{
    vector<Item> inventar;
    const string dateiname = "inventar.txt";
    int auswahl = 0;

    inventarLaden(inventar, dateiname);

    while (auswahl != 12)
    {
        menue();
        cin >> auswahl;

        if (auswahl == 1)
            inventarAnzeigen(inventar);

        else if (auswahl == 2)
            itemHinzufuegen(inventar, itemEingeben());

        else if (auswahl == 3)
        {
            string name;
            cout << "Name: ";
            cin >> ws;
            getline(cin, name);
            itemEntfernen(inventar, name);
        }

        else if (auswahl == 4)
            nachWertSortieren(inventar);

        else if (auswahl == 5)
            nachGewichtSortieren(inventar);

        else if (auswahl == 6)
            typenAnzeigen(inventar);

        else if (auswahl == 7)
            staerkstesItemAnzeigen(inventar);

        else if (auswahl == 8 && !inventar.empty())
            cout << "Bestes Item: " << bestesItem(inventar).name << '\n';

        else if (auswahl == 9)
        {
            int maxGewicht;
            cout << "Maximales Gewicht: ";
            cin >> maxGewicht;

            if (kannTragen(inventar, maxGewicht))
                cout << "Inventar kann getragen werden.\n";
            else
                cout << "Inventar ist zu schwer.\n";
        }

        else if (auswahl == 10)
            inventarSpeichern(inventar, dateiname);

        else if (auswahl == 11)
            inventarLaden(inventar, dateiname);
    }

    inventarSpeichern(inventar, dateiname);
    cout << "Programm beendet.\n";

    return 0;
}
