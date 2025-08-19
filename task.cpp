#include <iostream>
#include <fstream>
using namespace std;

void inputs();

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

    ofstream file("sales.csv", ios::app);
    if (!file)
    {
        cerr << "Error opening sales.csv\n";
        return;
    }

    file << date << "," << item_name << "," << unit_Price << "," << item_quantity << "," << total << "\n";
    file.close();

    cout << "do you want to continue?(y/n): ";
    cin >> input;
    cin.ignore();

    if (input == 'Y' || input == 'y')
    {
        inputs();
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
