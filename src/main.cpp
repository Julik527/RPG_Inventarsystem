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

string typZuText(Typ typ)
{
    if (typ == Typ::Waffe)
        return "Waffe";
    if (typ == Typ::Traenke)
        return "Traenke";

    return "Ruestung";
}

void itemHinzufuegen(vector<Item>& inv, Item neu)
{
    inv.push_back(neu);
}

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

int gesamtGewicht(const vector<Item>& inv)
{
    int gesamt = 0;

    for (const Item& item : inv)
        gesamt += item.gewicht;

    return gesamt;
}

bool kannTragen(const vector<Item>& inv, int maxGewicht)
{
    return gesamtGewicht(inv) <= maxGewicht;
}

Item bestesItem(const vector<Item>& inv)
{
    Item bestes = inv[0];

    for (const Item& item : inv)
    {
        double aktuell;
        double bestesVerhaeltnis;

        if (item.gewicht == 0)
            aktuell = item.wert;
        else
            aktuell = (double)item.wert / item.gewicht;

        if (bestes.gewicht == 0)
            bestesVerhaeltnis = bestes.wert;
        else
            bestesVerhaeltnis = (double)bestes.wert / bestes.gewicht;

        if (aktuell > bestesVerhaeltnis)
            bestes = item;
    }

    return bestes;
}

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

void nachWertSortieren(vector<Item>& inv)
{
    sort(inv.begin(), inv.end(),
        [](const Item& a, const Item& b)
        {
            return a.wert > b.wert;
        });
}

void nachGewichtSortieren(vector<Item>& inv)
{
    sort(inv.begin(), inv.end(),
        [](const Item& a, const Item& b)
        {
            return a.gewicht < b.gewicht;
        });
}

void typenAnzeigen(const vector<Item>& inv)
{
    map<string, int> anzahl;

    for (const Item& item : inv)
        anzahl[typZuText(item.type)]++;

    for (const auto& eintrag : anzahl)
        cout << eintrag.first << ": " << eintrag.second << '\n';
}

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

void inventarLaden(vector<Item>& inv, const string& dateiname)
{
    ifstream datei(dateiname);

    if (!datei)
        return;

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

    inventarLaden(inventar, dateiname);

    int auswahl = 0;

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
            itemHinzufuegen(inventar, itemEingeben());
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
        }
        else if (auswahl == 5)
        {
            nachGewichtSortieren(inventar);
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
            if (!inventar.empty())
            {
                Item bestes = bestesItem(inventar);
                cout << "Bestes Item: " << bestes.name << '\n';
            }
        }
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
        {
            inventarSpeichern(inventar, dateiname);
            cout << "Gespeichert.\n";
        }
        else if (auswahl == 11)
        {
            inventarLaden(inventar, dateiname);
            cout << "Geladen.\n";
        }
    }

    inventarSpeichern(inventar, dateiname);
    cout << "Programm beendet.\n";

    return 0;
}
