#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
using namespace std;

void inputs();
void view();
bool isValidDateFormat(const string &date);

vector<int> usedIDs;
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;
void view() {
    ifstream file("sales.csv");
    if (!file) {
        cerr << "Error: sales.csv not found!" << endl;
        return;
    }

    string line;

    // Print Header
    cout << left << setw(10) << "Sale ID"
         << setw(12) << "Date"
         << setw(15) << "Item Name"
         << setw(12) << "Unit Price"
         << setw(10) << "Quantity"
         << setw(10) << "Total" << endl;
    cout << string(70, '-') << endl;

    while (getline(file, line)) {
        stringstream ss(line);
        string saleID, date, itemName, unitPrice, quantity, total;

        getline(ss, saleID, ',');
        getline(ss, date, ',');
        getline(ss, itemName, ',');
        getline(ss, unitPrice, ',');
        getline(ss, quantity, ',');
        getline(ss, total, ',');

        cout << left << setw(10) << saleID
             << setw(12) << date
             << setw(15) << itemName
             << setw(12) << unitPrice
             << setw(10) << quantity
             << setw(10) << total << endl;
    }

    file.close();
}

int generateUniqueID()
{
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dist(1000, 9999); 

    int id;
    do {
        id = dist(gen);
    } while (find(usedIDs.begin(), usedIDs.end(), id) != usedIDs.end());

    usedIDs.push_back(id);
    return id;
}
bool isValidDateFormat(const string &date)
{
    if (date.length() != 10)
        return false;
    if (date[4] != '/' || date[7] != '/')
        return false;
    for (int i = 0; i < date.length(); ++i)
    {
        if (i == 4 || i == 7)
            continue;
        if (!isdigit(date[i]))
            return false;
    }
    return true;
}

void inputs()
{
    char input;
    string date, item_name;
    double unit_Price, total;
    int item_quantity;

    cout << " Enter Unit Price Date (YYYY/MM/DD) :";
    getline(cin, date);

    if (!isValidDateFormat(date))
    {
        cout << "invalid date";
        return;
    }

    cout << "Enter Item Name: ";
    getline(cin, item_name);
    cout << "Enter Unit Price: ";
    cin >> unit_Price;
    cout << "Enter Item Quantity: ";
    cin >> item_quantity;
    cin.ignore();

    total = item_quantity * unit_Price;
      int sales_id = generateUniqueID();

    ofstream file("sales.csv", ios::app);
    if (!file)
    {
        cerr << "Error opening sales.csv\n";
        return;
    }

    file<< sales_id << "," << date << "," << item_name << "," << unit_Price << "," << item_quantity << "," << total << "\n";
    file.close();

    cout << "do you want to continue?(y/n): ";
    cin >> input;
    cin.ignore();

    if (input == 'Y' || input == 'y')
    {
        inputs();
    }
    else{
        view();
    }
}

void getdata()
{
    ifstream checkFile("sales.csv");
    if (!checkFile)
    {
        ofstream createFile("sales.csv");
        createFile.close();
    }
    checkFile.close();

    inputs();
}

int main()
{
    getdata();
}
