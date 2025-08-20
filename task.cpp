#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <iomanip>
#include <map>
using namespace std;

void inputs();
void view();
bool isValidDateFormat(const string &date);
vector<int> usedIDs; 

struct Sale {
    string id, date, item;
    double price;
    int qty;
    double total;
};
bool compareByDate(const Sale &a, const Sale &b) {
    return a.date < b.date;  
}
void sortRecords(vector<Sale> &records) {
    sort(records.begin(), records.end(), compareByDate);
}
void generateReport() {
    ifstream file("temp.csv");
    if (!file) {
        cerr << "Error: temp.csv not found!" << endl;
        return;
    }
    map<string, vector<Sale>> salesByDate;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Sale s;
        string priceStr, qtyStr, totalStr;
        getline(ss, s.id, ',');
        getline(ss, s.date, ',');
        getline(ss, s.item, ',');
        getline(ss, priceStr, ',');
        getline(ss, qtyStr, ',');
        getline(ss, totalStr, ',');
        s.price = stod(priceStr);
        s.qty = stoi(qtyStr);
        s.total = stod(totalStr);
        salesByDate[s.date].push_back(s);
    }
    file.close();
    ofstream out("Report.txt");
    if (!out) {
        cerr << "Error creating Report.txt!" << endl;
        return;
    }
    time_t now = time(0);
tm *ltm = localtime(&now);
char today[11];
strftime(today, sizeof(today), "%Y-%m-%d", ltm);
out << "Report Genrated Date " << today << "\n\n";
    out << "Sales Report : Stationary Items Sold\n\n";
    out << left << setw(18) << "Date"
        << setw(15) << "SaleID"
        << setw(20) << "ItemName"
        << setw(12) << "Quantity"
        << setw(12) << "Price"
        << setw(15) << "SalesAmount" << "\n";
    double grandTotal = 0;
    for (const auto& pair : salesByDate) {
        const string& date = pair.first;
        const vector<Sale>& sales = pair.second;
        double subTotal = 0;
        for (const Sale& s : sales) {
            out << left << setw(18) << s.date
                << setw(15) << s.id
                << setw(20) << s.item
                << setw(12) << s.qty
                << setw(12) << fixed << setprecision(2) << s.price
                << setw(15) << fixed << setprecision(2) << s.total << "\n";
            subTotal += s.total;
        }
        out << right << setw(85) << "Subtotal for " << date << " is : " << fixed << setprecision(2) << subTotal << "\n";
        grandTotal += subTotal;
    }
    out << right << setw(85) << "Grand Total: " << fixed << setprecision(2) << grandTotal << "\n";
    out.close();
    cout << "Report generated successfully in Report.txt!\n";
}
void updateOrDelete() {
    string saleID;
    cout << "Enter unique Sale ID: ";
    cin >> saleID;
    ifstream in("sales.csv");
    if (!in) {
        cerr << "Error: sales.csv not found!" << endl;
        return;
    }
    vector<Sale> records;
    string line;
    bool found = false;
    while (getline(in, line)) {
        stringstream ss(line);
        Sale s;
        string priceStr, qtyStr, totalStr;
        getline(ss, s.id, ',');
        getline(ss, s.date, ',');
        getline(ss, s.item, ',');
        getline(ss, priceStr, ',');
        getline(ss, qtyStr, ',');
        getline(ss, totalStr, ',');
        s.price = stod(priceStr);
        s.qty = stoi(qtyStr);
        s.total = stod(totalStr);
        if (s.id == saleID) {
            found = true;
            cout << "Record found: " << line << endl;
            char choice;
            cout << "Do you want to (U)pdate or (D)elete this record? ";
            cin >> choice;
            if (choice == 'U' || choice == 'u') {
                cout << "Enter Date (YYYY/MM/DD): ";
                cin >> s.date;
                cout << "Enter Item Name: ";
                cin >> s.item;
                cout << "Enter Unit Price: ";
                cin >> s.price;
                cout << "Enter Quantity: ";
                cin >> s.qty;
                s.total = s.price * s.qty;
                cout << "Record updated successfully!" << endl;
                records.push_back(s);
            }
            else if (choice == 'D' || choice == 'd') {
                cout << "Record deleted successfully!" << endl;
            }
        } else {
            records.push_back(s);
        }
    }
    in.close();

    if (!found) {
        cout << "Sale ID " << saleID << " not found!" << endl;
        return;
    }  
    sortRecords(records);
    ofstream out("temp.csv");  
    for (auto &s : records) {
        out << s.id << "," << s.date << "," << s.item << ","
            << s.price << "," << s.qty << "," << s.total << "\n";
    }
    out.close();
}
void view() {
    ifstream file("sales.csv");
    if (!file) {
        cerr << "Error: sales.csv not found!" << endl;
        return;
    }
    string line;
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
         view();
        return;
    }
    cout << "Enter Item Name: ";
    getline(cin, item_name);
    cout << "Enter Unit Price: ";
    cin >> unit_Price;
    cout << "Enter Item Quantity: ";
    cin >> item_quantity;
    total = item_quantity * unit_Price;
      int sales_id = generateUniqueID();
    ofstream file("sales.csv", ios::app);
    if (!file)
    {
        cout << "Error opening sales.csv\n";
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
int main() {
    char input;
    getdata();
    cout << "Do you want to update or delete? (y/n): ";
    cin >> input;
    if (input == 'y' || input == 'Y') {
        updateOrDelete();
    }
    cout << "Do you want to generate a report? (y/n): ";
    cin >> input;
    if (input == 'y' || input == 'Y') {
        generateReport();
    }
    cout << "Thank you for purchasing!" << endl;
    return 0;
}
