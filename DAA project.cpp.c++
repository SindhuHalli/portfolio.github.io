#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <limits>
#include <sstream> // Add this include for std::istringstream
#include <ctime>
#include <set>
#include <queue>
#include <conio.h>
using namespace std;

struct Node
{
    string key;
    string value;
    Node *next;
    Node(const string &k, const string &v) : key(k), value(v), next(nullptr) {}
};

class HashMap
{
    vector<Node *> table;
    int capacity;

    int hash(const string &key)
    {
        int hashValue = 0;
        for (char ch : key)
        {
            hashValue = (hashValue * 31 + ch) % capacity;
        }
        return hashValue;
    }

public:
    HashMap(int cap = 10) : capacity(cap)
    {
        table.resize(capacity, nullptr);
    }

    void insert(const string &key, const string &value)
    {
        int index = hash(key);
        Node *newNode = new Node(key, value);

        if (!table[index])
        {
            table[index] = newNode;
        }
        else
        {
            Node *temp = table[index];
            while (temp->next)
            {
                if (temp->key == key)
                {
                    temp->value = value; // Update value if key exists
                    delete newNode;
                    return;
                }
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    string get(const string &key)
    {
        int index = hash(key);
        Node *temp = table[index];
        while (temp)
        {
            if (temp->key == key)
                return temp->value;
            temp = temp->next;
        }
        return "";
    }

    void erase(const string &key)
    {
        int index = hash(key);
        Node *temp = table[index];
        Node *prev = nullptr;
        while (temp)
        {
            if (temp->key == key)
            {
                if (prev)
                    prev->next = temp->next;
                else
                    table[index] = temp->next;
                delete temp;
                return;
            }
            prev = temp;
            temp = temp->next;
        }
    }
};

// Customer Class
class Customer
{
public:
    string file;
    unordered_map<int, vector<string>> customers;

    Customer(const string &fileName);
    void loadCustomers();
    vector<string> getCustomerDetails(int customerID);
    int login(const string &name, const string &password);
    int signUp(const string &name, const string &password);
    int getNextCustomerID();
    void updateCustomerFile();
    bool forgotPassword(const string &name, const string &newPassword);
};

Customer::Customer(const string &fileName) : file(fileName)
{
    loadCustomers();
}

void Customer::loadCustomers()
{
    ifstream inFile(file);
    if (!inFile)
    {
        cout << "Error opening customer file: " << file << "\n";
        return;
    }

    string line;
    getline(inFile, line); // Skip the header line
    while (getline(inFile, line))
    {
        vector<string> customerDetails;
        stringstream ss(line);
        string item;
        int customerID;

        try
        {
            getline(ss, item, ',');
            customerID = stoi(item);
            while (getline(ss, item, ','))
            {
                customerDetails.push_back(item);
            }

            customers[customerID] = customerDetails;
        }
        catch (const std::invalid_argument &e)
        {
            cout << "Error parsing customer data: " << line << "\n";
        }
    }
    inFile.close();
}

vector<string> Customer::getCustomerDetails(int customerID)
{
    if (customers.find(customerID) != customers.end())
    {
        return customers[customerID];
    }
    else
    {
        return {};
    }
}

int Customer::login(const string &name, const string &password)
{
    for (const auto &customer : customers)
    {
        if (customer.second[0] == name)
        {
            if (customer.second[1] == password)
            {
                cout << "Do you want to change password? Enter [Y/N]: ";
                char c;
                cin >> c;
                if (c == 'Y' || c == 'y')
                {
                    string newPassword;
                    cout << "Enter new password: ";
                    char ch;
                    while (true)
                    {
                        ch = _getch(); // Read a single character without echoing it to the console

                        if (ch == '\r')
                        {
                            break;
                        }

                        newPassword += ch; // Append character to input string
                        std::cout << '*';  // Display an asterisk
                    }
                    cout << '\n';
                    customers[customer.first][1] = newPassword;
                    updateCustomerFile();
                    cout << "Password changed successfully.\n";
                }
                return customer.first; // Return customer ID
            }
            else
            {
                cout << "Invalid password...\n";
                cout << "Do you want to reset your password? Enter [Y/N]: ";
                char c;
                cin >> c;
                if (c == 'Y' || c == 'y')
                {
                    string newPassword;
                    cout << "Enter new password: ";
                    char ch;
                    while (true)
                    {
                        ch = _getch(); // Read a single character without echoing it to the console

                        if (ch == '\r')
                        {
                            break;
                        }

                        newPassword += ch; // Append character to input string
                        std::cout << '*';  // Display an asterisk
                    }
                    cout << '\n';
                    customers[customer.first][1] = newPassword;
                    updateCustomerFile();
                    cout << "Password reset successfully. You can now log in with your new password.\n";
                    return customer.first; // Return customer ID
                }
                return -1; // Indicate invalid password
            }
        }
    }
    cout << "Invalid name...\n";
    return -1; // Indicate invalid name
}

int Customer::signUp(const string &name, const string &password)
{
    int newCustomerID = getNextCustomerID();
    customers[newCustomerID] = {name, password};

    ofstream outFile(file, ios::app);
    if (outFile)
    {
        outFile << newCustomerID << "," << name << "," << password << "\n";
        outFile.close();
        return newCustomerID; // Return new customer ID
    }
    else
    {
        cout << "Error opening customer file for writing: " << file << "\n";
        return -1; // Indicate error in file writing
    }
}

int Customer::getNextCustomerID()
{
    int maxID = 0;
    for (const auto &customer : customers)
    {
        if (customer.first > maxID)
        {
            maxID = customer.first;
        }
    }
    return maxID + 1;
}

void Customer::updateCustomerFile()
{
    ofstream outFile(file, ios::trunc);
    if (outFile)
    {
        outFile << "CustomerID,CustomerName,Password\n"; // Write header
        for (const auto &customer : customers)
        {
            outFile << customer.first << "," << customer.second[0] << "," << customer.second[1] << "\n";
        }
        outFile.close();
    }
    else
    {
        cout << "Error opening customer file for writing: " << file << "\n";
    }
}

bool Customer::forgotPassword(const string &name, const string &newPassword)
{
    for (auto &customer : customers)
    {
        if (customer.second[0] == name)
        {
            customer.second[1] = newPassword;
            updateCustomerFile();
            return true;
        }
    }
    return false;
}

void userMenu()
{
    cout << "Welcome to the user menu!\n";
    // Add user menu options here
}

// GraphCreator Class
class GraphCreator
{
private:
    struct StateData
    {
        int id;
        std::vector<std::vector<int>> data;
    };

    std::unordered_map<int, StateData> stateMap;

public:
    // Constructor to initialize predefined hash values
    GraphCreator()
    {
        stateMap[1] = {1, {{0, 1, 1}, {1, 0, 0}, {1, 0, 0}}}; // Example graph for state 1
        stateMap[2] = {2, {{0, 0, 1}, {0, 0, 1}, {1, 1, 0}}}; // Example graph for state 2
        stateMap[3] = {3, {{0, 1}, {1, 0}}};                  // Example graph for state 3
    }

    // Function to create and display a graph for a given key (state ID)
    void createGraph(int key)
    {
        if (stateMap.find(key) == stateMap.end())
        {
            std::cout << "State with key " << key << " does not exist.\n";
            return;
        }

        const auto &state = stateMap[key];

        std::cout << "Graph for state ID " << state.id << ":\n";
        for (const auto &row : state.data)
        {
            for (int val : row)
            {
                std::cout << val << " ";
            }
            std::cout << "\n";
        }
    }

    // Helper function to perform Prim's algorithm
    std::vector<std::vector<int>> prim(const std::vector<std::vector<int>> &graph)
    {
        int n = graph.size();
        std::vector<bool> inMST(n, false);
        std::vector<int> key(n, std::numeric_limits<int>::max());
        std::vector<int> parent(n, -1);
        key[0] = 0;

        for (int count = 0; count < n - 1; ++count)
        {
            int u = -1;
            for (int i = 0; i < n; ++i)
            {
                if (!inMST[i] && (u == -1 || key[i] < key[u]))
                {
                    u = i;
                }
            }

            inMST[u] = true;

            for (int v = 0; v < n; ++v)
            {
                if (graph[u][v] && !inMST[v] && graph[u][v] < key[v])
                {
                    key[v] = graph[u][v];
                    parent[v] = u;
                }
            }
        }

        std::vector<std::vector<int>> mst(n, std::vector<int>(n, 0));
        for (int i = 1; i < n; ++i)
        {
            mst[parent[i]][i] = graph[parent[i]][i];
            mst[i][parent[i]] = graph[parent[i]][i];
        }

        return mst;
    }

    // Function to create MST for a specific key and display it
    void createMSTForKey(int key)
    {
        if (stateMap.find(key) == stateMap.end())
        {
            std::cout << "State with key " << key << " does not exist.\n";
            return;
        }

        const auto &graph = stateMap[key].data;
        if (graph.empty())
        {
            std::cout << "Empty graph for state " << key << ".\n";
            return;
        }

        std::vector<std::vector<int>> mst = prim(graph);

        std::cout << "MST for state ID " << key << ":\n";
        for (const auto &row : mst)
        {
            for (int val : row)
            {
                std::cout << val << " ";
            }
            std::cout << "\n";
        }
    }
};
// Travel Booking Class
class TravelBooking
{
public:
    string userFile;
    string agencyFile;
    unordered_map<string, vector<string>> bookingDetails; // unordered_map to store booking details
    unordered_map<string, vector<string>> packageDetails; // unordered_map to store package details
    Customer &customerData;

    TravelBooking(const string &userFileName, const string &agencyFileName, Customer &customerData);
    void loadBookings();
    void saveBookings();
    void addBooking();
    void updateBooking();
    void cancelBooking();
    void viewBookings();
    int getNextBookingID();
    void viewAgentFile(const string &fileName);
    void loadAgentFile();
    void saveAgentFile();
    void addAgentData();
    void updateAgentData();
    void removeAgentData();
    int getNextPackageID();
    void displayBooking(const string &id);
    void displayAgentData(const string &id);
    void viewPackagesByRate();
    void viewPackagesByNumberOfPeople();
    void viewPackagesByRating();
    void viewPackagesBySeason();
    void viewOwnBookings(const string &customerId);

    // Sorting Algorithms
    void quickSortPackages(vector<pair<string, double>> &packages, int start, int end);
    int partitionPackages(vector<pair<string, double>> &packages, int start, int end);
    void mergeSortPackages(vector<pair<string, double>> &packages, int left, int right);
    void mergePackages(vector<pair<string, double>> &packages, int left, int mid, int right);

    // Utility function to print packages
    void printPackages(const vector<pair<string, double>> &packages);

    // String Search Algorithms
    void kmpSearch(const string &text, const string &pattern);
    vector<int> computeLPSArray(const string &pattern);
    void rabinKarpSearch(const string &text, const string &pattern);
};

// Quick Sort: Main function
void TravelBooking::quickSortPackages(vector<pair<string, double>> &packages, int start, int end) {
    if (start < end) {
        int pivotIndex = partitionPackages(packages, start, end);
        quickSortPackages(packages, start, pivotIndex - 1); // Recursively sort left subarray
        quickSortPackages(packages, pivotIndex + 1, end);   // Recursively sort right subarray
    }
}

// Partition function
int TravelBooking::partitionPackages(vector<pair<string, double>> &packages, int start, int end) {
    double pivot = packages[end].second; // Choose the last element as pivot
    int i = start - 1;                   // Index of smaller element

    for (int j = start; j < end; j++) {
        if (packages[j].second <= pivot) { // Check if current element is less than or equal to pivot
            i++;
            swap(packages[i], packages[j]); // Swap the elements
        }
    }

    swap(packages[i + 1], packages[end]); // Place pivot in the correct position
    return i + 1;                         // Return pivot index
}

// Merge Sort: Main function
void TravelBooking::mergeSortPackages(vector<pair<string, double>> &packages, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2; // Find the middle point

        mergeSortPackages(packages, left, mid);        // Recursively sort the left half
        mergeSortPackages(packages, mid + 1, right);   // Recursively sort the right half

        mergePackages(packages, left, mid, right);     // Merge the two halves
    }
}

// Merge function
void TravelBooking::mergePackages(vector<pair<string, double>> &packages, int left, int mid, int right) {
    int n1 = mid - left + 1; // Size of the left subarray
    int n2 = right - mid;    // Size of the right subarray

    // Create temporary arrays
    vector<pair<string, double>> leftArray(n1);
    vector<pair<string, double>> rightArray(n2);

    // Copy data to temporary arrays
    for (int i = 0; i < n1; i++) {
        leftArray[i] = packages[left + i];
    }
    for (int j = 0; j < n2; j++) {
        rightArray[j] = packages[mid + 1 + j];
    }

    // Merge the temporary arrays
    int i = 0, j = 0;
    int k = left;

    while (i < n1 && j < n2) {
        if (leftArray[i].second <= rightArray[j].second) {
            packages[k] = leftArray[i];
            i++;
        } else {
            packages[k] = rightArray[j];
            j++;
        }
        k++;
    }

    // Copy remaining elements of leftArray[]
    while (i < n1) {
        packages[k] = leftArray[i];
        i++;
        k++;
    }

    // Copy remaining elements of rightArray[]
    while (j < n2) {
        packages[k] = rightArray[j];
        j++;
        k++;
    }
}

// Utility function to print packages
void TravelBooking::printPackages(const vector<pair<string, double>> &packages) {
    for (const auto &package : packages) {
        cout << "Package ID: " << package.first << ", Rate: " << package.second << endl;
    }
}

// KMP Search Algorithm
void TravelBooking::kmpSearch(const string &text, const string &pattern) {
    vector<int> lps = computeLPSArray(pattern);
    int i = 0, j = 0; // Pointers for text and pattern

    while (i < text.length()) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }

        if (j == pattern.length()) {
            cout << "Pattern found at index " << (i - j) << endl;
            j = lps[j - 1];
        } else if (i < text.length() && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
}

// Compute LPS Array for KMP
vector<int> TravelBooking::computeLPSArray(const string &pattern) {
    vector<int> lps(pattern.length());
    int length = 0;
    int i = 1;

    while (i < pattern.length()) {
        if (pattern[i] == pattern[length]) {
            length++;
            lps[i] = length;
            i++;
        } else {
            if (length != 0) {
                length = lps[length - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

// Rabin-Karp Algorithm
void TravelBooking::rabinKarpSearch(const string &text, const string &pattern) {
    int d = 256; // Number of characters in the input alphabet
    int q = 101; // A prime number
    int m = pattern.length();
    int n = text.length();
    int p = 0; // Hash value for pattern
    int t = 0; // Hash value for text
    int h = 1;

    for (int i = 0; i < m - 1; i++) {
        h = (h * d) % q;
    }

    for (int i = 0; i < m; i++) {
        p = (d * p + pattern[i]) % q;
        t = (d * t + text[i]) % q;
    }

    for (int i = 0; i <= n - m; i++) {
        if (p == t) {
            bool match = true;
            for (int j = 0; j < m; j++) {
                if (text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                cout << "Pattern found at index " << i << endl;
            }
        }

        if (i < n - m) {
            t = (d * (t - text[i] * h) + text[i + m]) % q;
            if (t < 0) {
                t += q;
            }
        }
    }
}

TravelBooking::TravelBooking(const string &userFileName, const string &agencyFileName, Customer &customerData)
    : userFile(userFileName), agencyFile(agencyFileName), customerData(customerData)
{
    loadBookings();
    loadAgentFile();
}
void TravelBooking::viewOwnBookings(const string &customerId)
{
    cout << "--- Your Bookings ---\n";
    for (const auto &booking : bookingDetails)
    {
        if (booking.second[0] == customerId) // Assuming the customer ID is at index 0
        {
            cout << "Booking ID: " << booking.first << "\n";
            cout << "Customer ID: " << booking.second[0] << "\n";
            cout << "Package ID: " << booking.second[1] << "\n";
            cout << "------------------------\n";
        }
    }
}
void TravelBooking::loadBookings()
{
    ifstream inFile(userFile);
    if (!inFile)
    {
        cout << "Error opening booking file: " << userFile << "\n";
        return;
    }

    string line;
    getline(inFile, line); // Skip the header line
    while (getline(inFile, line))
    {
        vector<string> details;
        size_t pos = 0;
        string bookingID;
        bool isFirstField = true;
        while ((pos = line.find(',')) != string::npos)
        {
            if (isFirstField)
            {
                bookingID = line.substr(0, pos);
                isFirstField = false;
            }
            else
            {
                details.push_back(line.substr(0, pos));
            }
            line.erase(0, pos + 1);
        }
        details.push_back(line);
        bookingDetails[bookingID] = details;
    }
    inFile.close();
}

void TravelBooking::saveBookings()
{
    ofstream outFile(userFile, ios::trunc);
    if (!outFile)
    {
        cout << "Error opening booking file for writing: " << userFile << "\n";
        return;
    }

    outFile << "BookingID,CustomerID,PackageID\n"; // Write header
    for (const auto &booking : bookingDetails)
    {
        outFile << booking.first;
        for (const auto &detail : booking.second)
        {
            outFile << "," << detail;
        }
        outFile << "\n";
    }
    outFile.close();
}

int TravelBooking::getNextBookingID()
{
    int maxID = 0;
    for (const auto &booking : bookingDetails)
    {
        int currentID = stoi(booking.first);
        if (currentID > maxID)
        {
            maxID = currentID;
        }
    }
    return maxID + 1;
}

void TravelBooking::addBooking()
{
    string customerId, packageId;
    int bookingID = getNextBookingID();
    cout << "Generated Booking ID: " << bookingID << "\n";
    cin.ignore();
    cout << "Enter Customer ID: ";
    getline(cin, customerId);
    cout << "Enter Package ID: ";
    getline(cin, packageId);

    vector<string> details = {customerId, packageId};
    bookingDetails[to_string(bookingID)] = details;
    saveBookings();
    cout << "Booking added successfully.\n";
    cout << "Package selected:\n";
    displayAgentData(packageId);
}

void TravelBooking::updateBooking()
{
    string id, newCustomerId, newPackageId;
    cout << "Enter Booking ID to update: ";
    cin >> id;
    cin.ignore();
    cout << "Enter new Customer ID: ";
    getline(cin, newCustomerId);
    cout << "Enter new Package ID: ";
    getline(cin, newPackageId);

    vector<string> details = {newCustomerId, newPackageId};
    bookingDetails[id] = details;
    saveBookings();
    cout << "Booking updated successfully.\n";
    cout << "Package selected:\n";
    displayAgentData(newPackageId);
}

void TravelBooking::cancelBooking()
{
    string id;
    cout << "Enter Booking ID to cancel: ";
    cin >> id;
    cin.ignore();
    if (bookingDetails.erase(id))
    {
        saveBookings();
        cout << "Booking cancelled successfully.\n";
    }
    else
    {
        cout << "Booking not found.\n";
    }
}

void TravelBooking::viewBookings()
{
    for (const auto &booking : bookingDetails)
    {
        displayBooking(booking.first);
    }
}

void TravelBooking::viewAgentFile(const string &fileName)
{
    ifstream inFile(fileName);
    if (!inFile)
    {
        cout << "Error opening file: " << fileName << "\n";
        return;
    }

    string line;
    while (getline(inFile, line))
    {
        cout << line << "\n";
    }
    inFile.close();
}

void TravelBooking::loadAgentFile()
{
    ifstream inFile(agencyFile);
    if (!inFile)
    {
        cout << "Error opening agent file: " << agencyFile << "\n";
        return;
    }

    string line;
    getline(inFile, line); // Skip the header line
    while (getline(inFile, line))
    {
        vector<string> details;
        size_t pos = 0;
        string packageID;
        bool isFirstField = true;
        while ((pos = line.find(',')) != string::npos)
        {
            if (isFirstField)
            {
                packageID = line.substr(0, pos);
                isFirstField = false;
            }
            else
            {
                details.push_back(line.substr(0, pos));
            }
            line.erase(0, pos + 1);
        }
        details.push_back(line); // Add the last field
        packageDetails[packageID] = details;
    }
    inFile.close();
}

void TravelBooking::saveAgentFile()
{
    ofstream outFile(agencyFile, ios::trunc);
    if (!outFile)
    {
        cout << "Error opening agent file for writing: " << agencyFile << "\n";
        return;
    }

    outFile << "PackageID,PackageName,AgentID,Mode,NoOfPeople,From,To,Rate,Season\n"; // Write header
    for (const auto &package : packageDetails)
    {
        outFile << package.first;
        for (const auto &detail : package.second)
        {
            outFile << "," << detail;
        }
        outFile << "\n";
    }
    outFile.close();
}

int TravelBooking::getNextPackageID()
{
    int maxID = 0;
    for (const auto &package : packageDetails)
    {
        int currentID = stoi(package.first);
        if (currentID > maxID)
        {
            maxID = currentID;
        }
    }
    return maxID + 1;
}

void TravelBooking::addAgentData()
{
    string packageName, agentID, mode, noOfPeople, from, to, rate, season,rating;
    int packageID = getNextPackageID();
    cout << "Generated Package ID: " << packageID << "\n";
    cin.ignore();
    cout << "Enter Package Name: ";
    getline(cin, packageName);
    cout << "Enter Agent ID: ";
    getline(cin, agentID);
    cout << "Enter Mode: ";
    getline(cin, mode);
    cout << "Enter Number of People: ";
    getline(cin, noOfPeople);
    cout << "Enter From Location: ";
    getline(cin, from);
    cout << "Enter To Location: ";
    getline(cin, to);
    cout << "Enter Rate: ";
    getline(cin, rate);
    cout << "Enter Season: ";
    getline(cin, season);
    cout<<"Enter Rating: ";
    getline(cin, rating);

    vector<string> details = {packageName, agentID, mode, noOfPeople, from, to, rate, season,rating};
    packageDetails[to_string(packageID)] = details;
    saveAgentFile();
    cout << "Package data added successfully.\n";
    displayAgentData(to_string(packageID));
}

void TravelBooking::updateAgentData()
{
    string id, packageName, agentID, mode, noOfPeople, from, to, rate, season,rating;
    cout << "Enter Package ID to update: ";
    cin >> id;
    cin.ignore();
    cout << "Enter new Package Name: ";
    getline(cin, packageName);
    cout << "Enter new Agent ID: ";
    getline(cin, agentID);
    cout << "Enter new Mode: ";
    getline(cin, mode);
    cout << "Enter new Number of People: ";
    getline(cin, noOfPeople);
    cout << "Enter new From Location: ";
    getline(cin, from);
    cout << "Enter new To Location: ";
    getline(cin, to);
    cout << "Enter new Rate: ";
    getline(cin, rate);
    cout << "Enter new Season: ";
    getline(cin, season);
    cout<<"Enter new Rating: ";
    getline(cin, rating);

    vector<string> details = {packageName, agentID, mode, noOfPeople, from, to, rate, season,rating};
    packageDetails[id] = details;
    saveAgentFile();
    cout << "Package data updated successfully.\n";
    displayAgentData(id);
}

void TravelBooking::removeAgentData()
{
    string id;
    cout << "Enter Package ID to remove: ";
    cin >> id;
    cin.ignore();
    if (packageDetails.erase(id))
    {
        saveAgentFile();
        cout << "Package data removed successfully.\n";
    }
    else
    {
        cout << "Package data not found.\n";
    }
}

void TravelBooking::displayBooking(const string &id)
{
    if (bookingDetails.find(id) != bookingDetails.end())
    {
        cout << "Booking ID: " << id << "\n";
        cout << "Customer ID: " << bookingDetails[id][0] << "\n";
        cout << "Package ID: " << bookingDetails[id][1] << "\n";
    }
    else
    {
        cout << "Booking ID not found.\n";
    }
}
void TravelBooking::displayAgentData(const string &id)
{
    if (packageDetails.find(id) != packageDetails.end())
    {
        cout << "Package ID: " << id << "\n";
        cout << "Package Name: " << packageDetails[id][0] << "\n";
        cout << "Agent ID: " << packageDetails[id][1] << "\n";
        cout << "Mode: " << packageDetails[id][2] << "\n";
        cout << "Number of People: " << packageDetails[id][3] << "\n";
        cout << "From: " << packageDetails[id][4] << "\n";
        cout << "To: " << packageDetails[id][5] << "\n";
        cout << "Rate: " << packageDetails[id][6] << "\n";
        cout << "Season: " << packageDetails[id][7] << "\n";
    }
    else
    {
        cout << "Package ID not found.\n";
    }
}
void TravelBooking::viewPackagesByRate()
{
    double minRate, maxRate;
    cout << "Enter the minimum rate: ";
    cin >> minRate;
    cout << "Enter the maximum rate: ";
    cin >> maxRate;

    vector<pair<string, double>> packagesInRange;

    // Collect packages within the specified price range
    for (const auto &package : packageDetails)
    {
        double rate = stod(package.second[6]); // Assuming the rate is at index 6
        if (rate >= minRate && rate <= maxRate)
        {
            packagesInRange.push_back(make_pair(package.first, rate));
        }
    }

    // Sort packages by rate in ascending order
    sort(packagesInRange.begin(), packagesInRange.end(), [](const pair<string, double> &a, const pair<string, double> &b)
         { return a.second < b.second; });

    // Display sorted packages
    cout << "--- Packages by Rate (Ascending Order) ---\n";
    for (const auto &package : packagesInRange)
    {
        cout << "Package ID: " << package.first << "\n";
        cout << "Package Name: " << packageDetails[package.first][0] << "\n";
        cout << "Rate: " << package.second << "\n";
        cout << "------------------------\n";
    }
}
void TravelBooking::viewPackagesByNumberOfPeople()
{
    int numPeople;
    cout << "Enter the number of people: ";
    cin >> numPeople;

    cout << "--- Packages by Number of People ---\n";
    for (const auto &package : packageDetails)
    {
        int packagePeople = 0;

        for (char ch : package.second[3])
        {
            packagePeople = packagePeople * 10 + (ch - '0'); // Accumulate numeric value
        }
        if (packagePeople <= numPeople)
        {
            cout << "Package ID: " << package.first << "\n";
            cout << "Package Name: " << package.second[0] << "\n";
            cout << "Number of People: " << package.second[3] << "\n";
            cout << "------------------------\n";
        }
    }
}

void TravelBooking::viewPackagesByRating()
{
    vector<pair<string, double>> packagesWithRatings;

    // Collect packages with their ratings
    for (const auto &package : packageDetails)
    {

        double rating = 0;
        cout << package.second[0] << endl;

        for (char ch : package.second[8])
        {
            if (ch != '.')
                rating = rating * 10 + (ch - '0'); // Accumulate numeric value
        }
        packagesWithRatings.push_back(make_pair(package.first, rating));
    }

    // Sort packages by rating in descending order
    sort(packagesWithRatings.begin(), packagesWithRatings.end(), [](const pair<string, double> &a, const pair<string, double> &b)
         { return b.second < a.second; });

    // Display sorted packages
    cout << "--- Packages by Rating (Descending Order) ---\n";
    for (const auto &package : packagesWithRatings)
    {
        cout << "Package ID: " << package.first << "\n";
        cout << "Package Name: " << packageDetails[package.first][0] << "\n";
        cout << "Rating: " << packageDetails[package.first][8] << "\n";
        cout << "------------------------\n";
    }
}
void TravelBooking::viewPackagesBySeason()
{
    string season;
    cout << "Enter the season (e.g., Winter, Summer, Spring, Autumn): ";
    cin >> season;

    cout << "--- Packages for Season: " << season << " ---\n";
    for (const auto &package : packageDetails)
    {
        if (package.second[7] == season) // Assuming the season is at index 7
        {
            cout << "Package ID: " << package.first << "\n";
            cout << "Package Name: " << package.second[1] << "\n";
            cout << "Season: " << package.second[7] << "\n";
            cout << "------------------------\n";
        }
    }
}
// Hotel Booking Class
class HotelBooking
{
public:
    string userFile;
    string agencyFile;
    unordered_map<string, vector<string>> bookingDetails; // unordered_map to store booking details
    unordered_map<string, vector<string>> packageDetails; // unordered_map to store package details
    Customer &customerData;

    HotelBooking(const string &userFileName, const string &agencyFileName, Customer &customerData);
    void loadBookings();
    void saveBookings();
    void addBooking();
    void updateBooking();
    void cancelBooking();
    void viewBookings();
    int getNextBookingID();
    void viewAgentFile(const string &fileName);
    void loadAgentFile();
    void saveAgentFile();
    void addAgentData();
    void updateAgentData();
    void removeAgentData();
    int getNextPackageID();
    void displayBooking(const string &id);
    void displayAgentData(const string &id);
    void viewPackagesByRate();
    void viewPackagesByNumberOfPeople();
    void viewPackagesByRating();
    void viewPackagesBySeason();
    void viewOwnBookings(const string &customerId);

    // Sorting Algorithms
    void quickSortPackages(vector<pair<string, double>> &packages, int start, int end);
    int partitionPackages(vector<pair<string, double>> &packages, int start, int end);
    void mergeSortPackages(vector<pair<string, double>> &packages, int left, int right);
    void mergePackages(vector<pair<string, double>> &packages, int left, int mid, int right);

    // Utility function to print packages
    void printPackages(const vector<pair<string, double>> &packages);

    // String Search Algorithms
    void kmpSearch(const string &text, const string &pattern);
    vector<int> computeLPSArray(const string &pattern);
    void rabinKarpSearch(const string &text, const string &pattern);
};

void HotelBooking::viewOwnBookings(const string &customerId)
{
    cout << "--- Your Bookings ---\n";
    for (const auto &booking : bookingDetails)
    {
        if (booking.second[0] == customerId) // Assuming the customer ID is at index 0
        {
            cout << "Booking ID: " << booking.first << "\n";
            cout << "Customer ID: " << booking.second[0] << "\n";
            cout << "Package ID: " << booking.second[1] << "\n";
            cout << "------------------------\n";
        }
    }
}
// Quick Sort: Main function
void HotelBooking::quickSortPackages(vector<pair<string, double>> &packages, int start, int end) {
    if (start < end) {
        int pivotIndex = partitionPackages(packages, start, end);
        quickSortPackages(packages, start, pivotIndex - 1); // Recursively sort left subarray
        quickSortPackages(packages, pivotIndex + 1, end);   // Recursively sort right subarray
    }
}

// Partition function
int HotelBooking::partitionPackages(vector<pair<string, double>> &packages, int start, int end) {
    double pivot = packages[end].second; // Choose the last element as pivot
    int i = start - 1;                   // Index of smaller element

    for (int j = start; j < end; j++) {
        if (packages[j].second <= pivot) { // Check if current element is less than or equal to pivot
            i++;
            swap(packages[i], packages[j]); // Swap the elements
        }
    }

    swap(packages[i + 1], packages[end]); // Place pivot in the correct position
    return i + 1;                         // Return pivot index
}

// Merge Sort: Main function
void HotelBooking::mergeSortPackages(vector<pair<string, double>> &packages, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2; // Find the middle point

        mergeSortPackages(packages, left, mid);        // Recursively sort the left half
        mergeSortPackages(packages, mid + 1, right);   // Recursively sort the right half

        mergePackages(packages, left, mid, right);     // Merge the two halves
    }
}

// Merge function
void HotelBooking::mergePackages(vector<pair<string, double>> &packages, int left, int mid, int right) {
    int n1 = mid - left + 1; // Size of the left subarray
    int n2 = right - mid;    // Size of the right subarray

    // Create temporary arrays
    vector<pair<string, double>> leftArray(n1);
    vector<pair<string, double>> rightArray(n2);

    // Copy data to temporary arrays
    for (int i = 0; i < n1; i++) {
        leftArray[i] = packages[left + i];
    }
    for (int j = 0; j < n2; j++) {
        rightArray[j] = packages[mid + 1 + j];
    }

    // Merge the temporary arrays
    int i = 0, j = 0;
    int k = left;

    while (i < n1 && j < n2) {
        if (leftArray[i].second <= rightArray[j].second) {
            packages[k] = leftArray[i];
            i++;
        } else {
            packages[k] = rightArray[j];
            j++;
        }
        k++;
    }

    // Copy remaining elements of leftArray[]
    while (i < n1) {
        packages[k] = leftArray[i];
        i++;
        k++;
    }

    // Copy remaining elements of rightArray[]
    while (j < n2) {
        packages[k] = rightArray[j];
        j++;
        k++;
    }
}

// Utility function to print packages
void HotelBooking::printPackages(const vector<pair<string, double>> &packages) {
    for (const auto &package : packages) {
        cout << "Package ID: " << package.first << ", Rate: " << package.second << endl;
    }
}

// KMP Search Algorithm
void HotelBooking::kmpSearch(const string &text, const string &pattern) {
    vector<int> lps = computeLPSArray(pattern);
    int i = 0, j = 0; // Pointers for text and pattern

    while (i < text.length()) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }

        if (j == pattern.length()) {
            cout << "Pattern found at index " << (i - j) << endl;
            j = lps[j - 1];
        } else if (i < text.length() && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
}

// Compute LPS Array for KMP
vector<int> HotelBooking::computeLPSArray(const string &pattern) {
    vector<int> lps(pattern.length());
    int length = 0;
    int i = 1;

    while (i < pattern.length()) {
        if (pattern[i] == pattern[length]) {
            length++;
            lps[i] = length;
            i++;
        } else {
            if (length != 0) {
                length = lps[length - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

// Rabin-Karp Algorithm
void HotelBooking::rabinKarpSearch(const string &text, const string &pattern) {
    int d = 256; // Number of characters in the input alphabet
    int q = 101; // A prime number
    int m = pattern.length();
    int n = text.length();
    int p = 0; // Hash value for pattern
    int t = 0; // Hash value for text
    int h = 1;

    for (int i = 0; i < m - 1; i++) {
        h = (h * d) % q;
    }

    for (int i = 0; i < m; i++) {
        p = (d * p + pattern[i]) % q;
        t = (d * t + text[i]) % q;
    }

    for (int i = 0; i <= n - m; i++) {
        if (p == t) {
            bool match = true;
            for (int j = 0; j < m; j++) {
                if (text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                cout << "Pattern found at index " << i << endl;
            }
        }

        if (i < n - m) {
            t = (d * (t - text[i] * h) + text[i + m]) % q;
            if (t < 0) {
                t += q;
            }
        }
    }
}

HotelBooking::HotelBooking(const string &userFileName, const string &agencyFileName, Customer &customerData)
    : userFile(userFileName), agencyFile(agencyFileName), customerData(customerData)
{
    loadBookings();
    loadAgentFile();
}

void HotelBooking::loadBookings()
{
    ifstream inFile(userFile);
    if (!inFile)
    {
        cout << "Error opening booking file: " << userFile << "\n";
        return;
    }

    string line;
    getline(inFile, line); // Skip the header line
    while (getline(inFile, line))
    {
        vector<string> details;
        size_t pos = 0;
        string bookingID;
        bool isFirstField = true;
        while ((pos = line.find(',')) != string::npos)
        {
            if (isFirstField)
            {
                bookingID = line.substr(0, pos);
                isFirstField = false;
            }
            else
            {
                details.push_back(line.substr(0, pos));
            }
            line.erase(0, pos + 1);
        }
        details.push_back(line); // Add the last field
        bookingDetails[bookingID] = details;
    }
    inFile.close();
}

void HotelBooking::saveBookings()
{
    ofstream outFile(userFile, ios::trunc);
    if (!outFile)
    {
        cout << "Error opening booking file for writing: " << userFile << "\n";
        return;
    }

    outFile << "BookingID,CustomerID,PackageID\n"; // Write header
    for (const auto &booking : bookingDetails)
    {
        outFile << booking.first;
        for (const auto &detail : booking.second)
        {
            outFile << "," << detail;
        }
        outFile << "\n";
    }
    outFile.close();
}

int HotelBooking::getNextBookingID()
{
    int maxID = 0;
    for (const auto &booking : bookingDetails)
    {
        int currentID = stoi(booking.first);
        if (currentID > maxID)
        {
            maxID = currentID;
        }
    }
    return maxID + 1;
}

void HotelBooking::addBooking()
{
    string customerId, packageId;
    int bookingID = getNextBookingID();
    cout << "Generated Booking ID: " << bookingID << "\n";
    cin.ignore();
    cout << "Enter Customer ID: ";
    getline(cin, customerId);
    cout << "Enter Package ID: ";
    getline(cin, packageId);

    vector<string> details = {customerId, packageId};
    bookingDetails[to_string(bookingID)] = details;
    saveBookings();
    cout << "Booking added successfully.\n";
    cout << "Package selected:\n";
    displayAgentData(packageId);
}

void HotelBooking::updateBooking()
{
    string id, newCustomerId, newPackageId;
    cout << "Enter Booking ID to update: ";
    cin >> id;
    cin.ignore();
    cout << "Enter new Customer ID: ";
    getline(cin, newCustomerId);
    cout << "Enter new Package ID: ";
    getline(cin, newPackageId);

    vector<string> details = {newCustomerId, newPackageId};
    bookingDetails[id] = details;
    saveBookings();
    cout << "Booking updated successfully.\n";
    cout << "Package selected:\n";
    displayAgentData(newPackageId);
}

void HotelBooking::cancelBooking()
{
    string id;
    cout << "Enter Booking ID to cancel: ";
    cin >> id;
    cin.ignore();
    if (bookingDetails.erase(id))
    {
        saveBookings();
        cout << "Booking cancelled successfully.\n";
    }
    else
    {
        cout << "Booking not found.\n";
    }
}

void HotelBooking::viewBookings()
{
    for (const auto &booking : bookingDetails)
    {
        displayBooking(booking.first);
    }
}

void HotelBooking::viewAgentFile(const string &fileName)
{
    ifstream inFile(fileName);
    if (!inFile)
    {
        cout << "Error opening file: " << fileName << "\n";
        return;
    }

    string line;
    while (getline(inFile, line))
    {
        cout << line << "\n";
    }
    inFile.close();
}

void HotelBooking::loadAgentFile()
{
    ifstream inFile(agencyFile);
    if (!inFile)
    {
        cout << "Error opening agent file: " << agencyFile << "\n";
        return;
    }

    string line;
    getline(inFile, line); // Skip the header line
    while (getline(inFile, line))
    {
        vector<string> details;
        size_t pos = 0;
        string packageID;
        bool isFirstField = true;
        while ((pos = line.find(',')) != string::npos)
        {
            if (isFirstField)
            {
                packageID = line.substr(0, pos);
                isFirstField = false;
            }
            else
            {
                details.push_back(line.substr(0, pos));
            }
            line.erase(0, pos + 1);
        }
        details.push_back(line); // Add the last field
        packageDetails[packageID] = details;
    }
    inFile.close();
}

void HotelBooking::saveAgentFile()
{
    ofstream outFile(agencyFile, ios::trunc);
    if (!outFile)
    {
        cout << "Error opening agent file for writing: " << agencyFile << "\n";
        return;
    }

    outFile << "PackageID,PackageName,AgentID,Season,NoOfPeople,RoomType,Money\n"; // Write header
    for (const auto &package : packageDetails)
    {
        outFile << package.first;
        for (const auto &detail : package.second)
        {
            outFile << "," << detail;
        }
        outFile << "\n";
    }
    outFile.close();
}

int HotelBooking::getNextPackageID()
{
    int maxID = 0;
    for (const auto &package : packageDetails)
    {
        int currentID = stoi(package.first);
        if (currentID > maxID)
        {
            maxID = currentID;
        }
    }
    return maxID + 1;
}

void HotelBooking::addAgentData()
{
    string packageName, agentID, season, noOfPeople, roomType, money,rating;
    int packageID = getNextPackageID();
    cout << "Generated Package ID: " << packageID << "\n";
    cin.ignore();
    cout << "Enter Package Name: ";
    getline(cin, packageName);
    cout << "Enter Agent ID: ";
    getline(cin, agentID);
    cout << "Enter Season: ";
    getline(cin, season);
    cout << "Enter Number of People: ";
    getline(cin, noOfPeople);
    cout << "Enter Room Type: ";
    getline(cin, roomType);
    cout << "Enter Money: ";
    getline(cin, money);
    cout<<"Enter Rating: ";
    getline(cin, rating);

    vector<string> details = {packageName, agentID, season, noOfPeople, roomType, money,rating};
    packageDetails[to_string(packageID)] = details;
    saveAgentFile();
    cout << "Package data added successfully.\n";
    displayAgentData(to_string(packageID));
}

void HotelBooking::updateAgentData()
{
    string id, packageName, agentID, season, noOfPeople, roomType, money,rating;
    cout << "Enter Package ID to update: ";
    cin >> id;
    cin.ignore();
    cout << "Enter new Package Name: ";
    getline(cin, packageName);
    cout << "Enter new Agent ID: ";
    getline(cin, agentID);
    cout << "Enter new Season: ";
    getline(cin, season);
    cout << "Enter new Number of People: ";
    getline(cin, noOfPeople);
    cout << "Enter new Room Type: ";
    getline(cin, roomType);
    cout << "Enter new Money: ";
    getline(cin, money);
    cout<<"Enter new Rating: ";
    getline(cin, rating);
    vector<string> details = {packageName, agentID, season, noOfPeople, roomType, money,rating};
    packageDetails[id] = details;
    saveAgentFile();
    cout << "Package data updated successfully.\n";
    displayAgentData(id);
}

void HotelBooking::removeAgentData()
{
    string id;
    cout << "Enter Package ID to remove: ";
    cin >> id;
    cin.ignore();
    if (packageDetails.erase(id))
    {
        saveAgentFile();
        cout << "Package data removed successfully.\n";
    }
    else
    {
        cout << "Package data not found.\n";
    }
}

void HotelBooking::displayBooking(const string &id)
{
    if (bookingDetails.find(id) != bookingDetails.end())
    {
        cout << "Booking ID: " << id << "\n";
        cout << "Customer ID: " << bookingDetails[id][0] << "\n";
        cout << "Package ID: " << bookingDetails[id][1] << "\n";
    }
    else
    {
        cout << "Booking ID not found.\n";
    }
}

void HotelBooking::displayAgentData(const string &id)
{
    if (packageDetails.find(id) != packageDetails.end())
    {
        cout << "Package ID: " << id << "\n";
        cout << "Package Name: " << packageDetails[id][0] << "\n";
        cout << "Agent ID: " << packageDetails[id][1] << "\n";
        cout << "Season: " << packageDetails[id][2] << "\n";
        cout << "Number of People: " << packageDetails[id][3] << "\n";
        cout << "Room Type: " << packageDetails[id][4] << "\n";
        cout << "Money: " << packageDetails[id][5] << "\n";
    }
    else
    {
        cout << "Package ID not found.\n";
    }
}
void HotelBooking::viewPackagesByRate()
{
    double minRate, maxRate;
    cout << "Enter the minimum rate: ";
    cin >> minRate;
    cout << "Enter the maximum rate: ";
    cin >> maxRate;

    vector<pair<string, double>> packagesInRange;

    // Collect packages within the specified price range
    for (const auto &package : packageDetails)
    {
        double rate = stod(package.second[5]); // Assuming the rate is at index 5
        if (rate >= minRate && rate <= maxRate)
        {
            packagesInRange.push_back(make_pair(package.first, rate));
        }
    }

    // Sort packages by rate in ascending order
    sort(packagesInRange.begin(), packagesInRange.end(), [](const pair<string, double> &a, const pair<string, double> &b)
         { return a.second < b.second; });

    // Display sorted packages
    cout << "--- Packages by Rate (Ascending Order) ---\n";
    for (const auto &package : packagesInRange)
    {
        cout << "Package ID: " << package.first << "\n";
        cout << "Package Name: " << packageDetails[package.first][0] << "\n";
        cout << "Rate: " << package.second << "\n";
        cout << "------------------------\n";
    }
}

void HotelBooking::viewPackagesByNumberOfPeople()
{
    int numPeople;
    cout << "Enter the number of people: ";
    cin >> numPeople;

    cout << "--- Packages by Number of People ---\n";
    for (const auto &package : packageDetails)
    {
        int packagePeople = 0;
        for (char ch : package.second[3])
        {
            packagePeople = packagePeople * 10 + (ch - '0'); // Accumulate numeric value
        }
        if (packagePeople <= numPeople)
        {
            cout << "Package ID: " << package.first << "\n";
            cout << "Package Name: " << package.second[0] << "\n";
            cout << "Number of People: " << packagePeople << "\n";
            cout << "------------------------\n";
        }
    }
}
void HotelBooking::viewPackagesByRating()
{
    vector<pair<string, double>> packagesWithRatings;

    // Collect packages with their ratings
    for (const auto &package : packageDetails)
    {

        double rating = 0;
        cout << package.second[0] << endl;

        for (char ch : package.second[6])
        {
            if (ch != '.')
                rating = rating * 10 + (ch - '0'); // Accumulate numeric value
        }
        packagesWithRatings.push_back(make_pair(package.first, rating));
    }

    // Sort packages by rating in descending order
    sort(packagesWithRatings.begin(), packagesWithRatings.end(), [](const pair<string, double> &a, const pair<string, double> &b)
         { return b.second < a.second; });

    // Display sorted packages
    cout << "--- Packages by Rating (Descending Order) ---\n";
    for (const auto &package : packagesWithRatings)
    {
        cout << "Package ID: " << package.first << "\n";
        cout << "Package Name: " << packageDetails[package.first][0] << "\n";
        cout << "Rating: " << packageDetails[package.first][6] << "\n";
        cout << "------------------------\n";
    }
}
void HotelBooking::viewPackagesBySeason()
{
    string season;
    cout << "Enter the season (e.g., Winter, Summer, Spring, Autumn): ";
    cin >> season;

    cout << "--- Packages for Season: " << season << " ---\n";
    for (const auto &package : packageDetails)
    {
        if (package.second[2] == season) // Assuming the season is at index 3
        {
            cout << "Package ID: " << package.first << "\n";
            cout << "Package Name: " << package.second[0] << "\n";
            cout << "Season: " << package.second[2] << "\n";
            cout << "------------------------\n";
        }
    }
}

// Local Travel Booking Class
class LocalTravelBooking
{
public:
    string userFile;
    string agencyFile;
    unordered_map<string, vector<string>> bookingDetails; // unordered_map to store booking details
    unordered_map<string, vector<string>> packageDetails; // unordered_map to store package details
    Customer &customerData;

    LocalTravelBooking(const string &userFileName, const string &agencyFileName, Customer &customerData);
    void loadBookings();
    void saveBookings();
    void addBooking();
    void updateBooking();
    void cancelBooking();
    void viewBookings();
    int getNextBookingID();
    void viewAgentFile(const string &fileName);
    void loadAgentFile();
    void saveAgentFile();
    void addAgentData();
    void updateAgentData();
    void removeAgentData();
    int getNextPackageID();
    void displayBooking(const string &id);
    void viewOwnBookings(const string &customerId);
    void viewPackagesByRate();
    void displayAgentData(const string &id);

    // Sorting Algorithms
    void quickSortPackages(vector<pair<string, double>> &packages, int start, int end);
    int partitionPackages(vector<pair<string, double>> &packages, int start, int end);
    void mergeSortPackages(vector<pair<string, double>> &packages, int left, int right);
    void mergePackages(vector<pair<string, double>> &packages, int left, int mid, int right);

    // Utility function to print packages
    void printPackages(const vector<pair<string, double>> &packages);

    // String Search Algorithms
    void kmpSearch(const string &text, const string &pattern);
    vector<int> computeLPSArray(const string &pattern);
    void rabinKarpSearch(const string &text, const string &pattern);
    
};

void LocalTravelBooking::viewPackagesByRate()
{
    double minRate, maxRate;
    cout << "Enter the minimum rate: ";
    cin >> minRate;
    cout << "Enter the maximum rate: ";
    cin >> maxRate;

    vector<pair<string, double>> packagesInRange;

    // Collect packages within the specified price range
    for (const auto &package : packageDetails)
    {
        double rate = stod(package.second[4]); // Assuming the rate is at index 5
        if (rate >= minRate && rate <= maxRate)
        {
            packagesInRange.push_back(make_pair(package.first, rate));
        }
    }

    // Sort packages by rate in ascending order
    sort(packagesInRange.begin(), packagesInRange.end(), [](const pair<string, double> &a, const pair<string, double> &b)
         { return a.second < b.second; });

    // Display sorted packages
    cout << "--- Packages by Rate (Ascending Order) ---\n";
    for (const auto &package : packagesInRange)
    {
        cout << "Package ID: " << package.first << "\n";
        cout << "Package Name: " << packageDetails[package.first][0] << "\n";
        cout << "Rate: " << package.second << "\n";
        cout << "------------------------\n";
    }
}

void LocalTravelBooking::viewOwnBookings(const string &customerId)
{
    cout << "--- Your Bookings ---\n";
    for (const auto &booking : bookingDetails)
    {
        if (booking.second[0] == customerId) // Assuming the customer ID is at index 0
        {
            cout << "Booking ID: " << booking.first << "\n";
            cout << "Customer ID: " << booking.second[0] << "\n";
            cout << "Package ID: " << booking.second[1] << "\n";
            cout << "------------------------\n";
        }
    }
}

// Quick Sort: Main function
void LocalTravelBooking::quickSortPackages(vector<pair<string, double>> &packages, int start, int end) {
    if (start < end) {
        int pivotIndex = partitionPackages(packages, start, end);
        quickSortPackages(packages, start, pivotIndex - 1); // Recursively sort left subarray
        quickSortPackages(packages, pivotIndex + 1, end);   // Recursively sort right subarray
    }
}

// Partition function
int LocalTravelBooking::partitionPackages(vector<pair<string, double>> &packages, int start, int end) {
    double pivot = packages[end].second; // Choose the last element as pivot
    int i = start - 1;                   // Index of smaller element

    for (int j = start; j < end; j++) {
        if (packages[j].second <= pivot) { // Check if current element is less than or equal to pivot
            i++;
            swap(packages[i], packages[j]); // Swap the elements
        }
    }

    swap(packages[i + 1], packages[end]); // Place pivot in the correct position
    return i + 1;                         // Return pivot index
}

// Merge Sort: Main function
void LocalTravelBooking::mergeSortPackages(vector<pair<string, double>> &packages, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2; // Find the middle point

        mergeSortPackages(packages, left, mid);        // Recursively sort the left half
        mergeSortPackages(packages, mid + 1, right);   // Recursively sort the right half

        mergePackages(packages, left, mid, right);     // Merge the two halves
    }
}

// Merge function
void LocalTravelBooking::mergePackages(vector<pair<string, double>> &packages, int left, int mid, int right) {
    int n1 = mid - left + 1; // Size of the left subarray
    int n2 = right - mid;    // Size of the right subarray

    // Create temporary arrays
    vector<pair<string, double>> leftArray(n1);
    vector<pair<string, double>> rightArray(n2);

    // Copy data to temporary arrays
    for (int i = 0; i < n1; i++) {
        leftArray[i] = packages[left + i];
    }
    for (int j = 0; j < n2; j++) {
        rightArray[j] = packages[mid + 1 + j];
    }

    // Merge the temporary arrays
    int i = 0, j = 0;
    int k = left;

    while (i < n1 && j < n2) {
        if (leftArray[i].second <= rightArray[j].second) {
            packages[k] = leftArray[i];
            i++;
        } else {
            packages[k] = rightArray[j];
            j++;
        }
        k++;
    }

    // Copy remaining elements of leftArray[]
    while (i < n1) {
        packages[k] = leftArray[i];
        i++;
        k++;
    }

    // Copy remaining elements of rightArray[]
    while (j < n2) {
        packages[k] = rightArray[j];
        j++;
        k++;
    }
}

// Utility function to print packages
void LocalTravelBooking::printPackages(const vector<pair<string, double>> &packages) {
    for (const auto &package : packages) {
        cout << "Package ID: " << package.first << ", Rate: " << package.second << endl;
    }
}

// KMP Search Algorithm
void LocalTravelBooking::kmpSearch(const string &text, const string &pattern) {
    vector<int> lps = computeLPSArray(pattern);
    int i = 0, j = 0; // Pointers for text and pattern

    while (i < text.length()) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }

        if (j == pattern.length()) {
            cout << "Pattern found at index " << (i - j) << endl;
            j = lps[j - 1];
        } else if (i < text.length() && pattern[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
}

// Compute LPS Array for KMP
vector<int> LocalTravelBooking::computeLPSArray(const string &pattern) {
    vector<int> lps(pattern.length());
    int length = 0;
    int i = 1;

    while (i < pattern.length()) {
        if (pattern[i] == pattern[length]) {
            length++;
            lps[i] = length;
            i++;
        } else {
            if (length != 0) {
                length = lps[length - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

// Rabin-Karp Algorithm
void LocalTravelBooking::rabinKarpSearch(const string &text, const string &pattern) {
    int d = 256; // Number of characters in the input alphabet
    int q = 101; // A prime number
    int m = pattern.length();
    int n = text.length();
    int p = 0; // Hash value for pattern
    int t = 0; // Hash value for text
    int h = 1;

    for (int i = 0; i < m - 1; i++) {
        h = (h * d) % q;
    }

    for (int i = 0; i < m; i++) {
        p = (d * p + pattern[i]) % q;
        t = (d * t + text[i]) % q;
    }

    for (int i = 0; i <= n - m; i++) {
        if (p == t) {
            bool match = true;
            for (int j = 0; j < m; j++) {
                if (text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                cout << "Pattern found at index " << i << endl;
            }
        }

        if (i < n - m) {
            t = (d * (t - text[i] * h) + text[i + m]) % q;
            if (t < 0) {
                t += q;
            }
        }
    }
}

LocalTravelBooking::LocalTravelBooking(const string &userFileName, const string &agencyFileName, Customer &customerData)
    : userFile(userFileName), agencyFile(agencyFileName), customerData(customerData)
{
    loadBookings();
    loadAgentFile();
}

void LocalTravelBooking::loadBookings()
{
    ifstream inFile(userFile);
    if (!inFile)
    {
        cout << "Error opening booking file: " << userFile << "\n";
        return;
    }

    string line;
    getline(inFile, line); // Skip the header line
    while (getline(inFile, line))
    {
        vector<string> details;
        size_t pos = 0;
        string bookingID;
        bool isFirstField = true;
        while ((pos = line.find(',')) != string::npos)
        {
            if (isFirstField)
            {
                bookingID = line.substr(0, pos);
                isFirstField = false;
            }
            else
            {
                details.push_back(line.substr(0, pos));
            }
            line.erase(0, pos + 1);
        }
        details.push_back(line); // Add the last field
        bookingDetails[bookingID] = details;
    }
    inFile.close();
}

void LocalTravelBooking::saveBookings()
{
    ofstream outFile(userFile, ios::trunc);
    if (!outFile)
    {
        cout << "Error opening booking file for writing: " << userFile << "\n";
        return;
    }

    outFile << "BookingID,CustomerID,PackageID\n"; // Write header
    for (const auto &booking : bookingDetails)
    {
        outFile << booking.first;
        for (const auto &detail : booking.second)
        {
            outFile << "," << detail;
        }
        outFile << "\n";
    }
    outFile.close();
}

int LocalTravelBooking::getNextBookingID()
{
    int maxID = 0;
    for (const auto &booking : bookingDetails)
    {
        int currentID = stoi(booking.first);
        if (currentID > maxID)
        {
            maxID = currentID;
        }
    }
    return maxID + 1;
}

void LocalTravelBooking::addBooking()
{
    string customerId, packageId;
    int bookingID = getNextBookingID();
    cout << "Generated Booking ID: " << bookingID << "\n";
    cin.ignore();
    cout << "Enter Customer ID: ";
    getline(cin, customerId);
    cout << "Enter Package ID: ";
    getline(cin, packageId);

    vector<string> details = {customerId, packageId};
    bookingDetails[to_string(bookingID)] = details;
    saveBookings();
    cout << "Booking added successfully.\n";
    cout << "\nPackage selected:\n";
    displayAgentData(packageId);
}

void LocalTravelBooking::updateBooking()
{
    string id, newCustomerId, newPackageId;
    cout << "Enter Booking ID to update: ";
    cin >> id;
    cin.ignore();
    cout << "Enter new Customer ID: ";
    getline(cin, newCustomerId);
    cout << "Enter new Package ID: ";
    getline(cin, newPackageId);

    vector<string> details = {newCustomerId, newPackageId};
    bookingDetails[id] = details;
    saveBookings();
    cout << "Booking updated successfully.\n";
    cout << "Package selected:\n";
    displayAgentData(newPackageId);
}

void LocalTravelBooking::cancelBooking()
{
    string id;
    cout << "Enter Booking ID to cancel: ";
    cin >> id;
    cin.ignore();
    if (bookingDetails.erase(id))
    {
        saveBookings();
        cout << "Booking cancelled successfully.\n";
    }
    else
    {
        cout << "Booking not found.\n";
    }
}

void LocalTravelBooking::viewBookings()
{
    for (const auto &booking : bookingDetails)
    {
        displayBooking(booking.first);
    }
}

void LocalTravelBooking::viewAgentFile(const string &fileName)
{
    ifstream inFile(fileName);
    if (!inFile)
    {
        cout << "Error opening file: " << fileName << "\n";
        return;
    }

    string line;
    while (getline(inFile, line))
    {
        cout << line << "\n";
    }
    inFile.close();
}

void LocalTravelBooking::loadAgentFile()
{
    ifstream inFile(agencyFile);
    if (!inFile)
    {
        cout << "Error opening agent file: " << agencyFile << "\n";
        return;
    }

    string line;
    getline(inFile, line); // Skip the header line
    while (getline(inFile, line))
    {
        vector<string> details;
        size_t pos = 0;
        string packageID;
        bool isFirstField = true;
        while ((pos = line.find(',')) != string::npos)
        {
            if (isFirstField)
            {
                packageID = line.substr(0, pos);
                isFirstField = false;
            }
            else
            {
                details.push_back(line.substr(0, pos));
            }
            line.erase(0, pos + 1);
        }
        details.push_back(line); // Add the last field
        packageDetails[packageID] = details;
    }
    inFile.close();
}

void LocalTravelBooking::saveAgentFile()
{
    ofstream outFile(agencyFile, ios::trunc);
    if (!outFile)
    {
        cout << "Error opening agent file for writing: " << agencyFile << "\n";
        return;
    }

    outFile << "PackageID,Mode,NoOfPeople,From,To,Rate,City,State\n"; // Write header
    for (const auto &package : packageDetails)
    {
        outFile << package.first;
        for (const auto &detail : package.second)
        {
            outFile << "," << detail;
        }
        outFile << "\n";
    }
    outFile.close();
}

int LocalTravelBooking::getNextPackageID()
{
    int maxID = 0;
    for (const auto &package : packageDetails)
    {
        int currentID = stoi(package.first);
        if (currentID > maxID)
        {
            maxID = currentID;
        }
    }
    return maxID + 1;
}

void LocalTravelBooking::addAgentData()
{
    string mode, noOfPeople, from, to, rate, city, state,rating;
    int packageID = getNextPackageID();
    cout << "Generated Package ID: " << packageID << "\n";
    cin.ignore();
    cout << "Enter Mode: ";
    getline(cin, mode);
    cout << "Enter Number of People: ";
    getline(cin, noOfPeople);
    cout << "Enter From Location: ";
    getline(cin, from);
    cout << "Enter To Location: ";
    getline(cin, to);
    cout << "Enter Rate: ";
    getline(cin, rate);
    cout << "Enter City: ";
    getline(cin, city);
    cout << "Enter State: ";
    getline(cin, state);
    cout<<"Enter Rating: ";
    getline(cin, rating);

    vector<string> details = {mode, noOfPeople, from, to, rate, city, state,rating};
    packageDetails[to_string(packageID)] = details;
    saveAgentFile();
    cout << "Package data added successfully.\n";
    displayAgentData(to_string(packageID));
}

void LocalTravelBooking::updateAgentData()
{
    string id, mode, noOfPeople, from, to, rate, city, state,rating;
    cout << "Enter Package ID to update: ";
    cin >> id;
    cin.ignore();
    cout << "Enter new Mode: ";
    getline(cin, mode);
    cout << "Enter new Number of People: ";
    getline(cin, noOfPeople);
    cout << "Enter new From Location: ";
    getline(cin, from);
    cout << "Enter new To Location: ";
    getline(cin, to);
    cout << "Enter new Rate: ";
    getline(cin, rate);
    cout << "Enter new City: ";
    getline(cin, city);
    cout << "Enter new State: ";
    getline(cin, state);
    cout<<"Enter new Rating: ";
    getline(cin, rating);

    vector<string> details = {mode, noOfPeople, from, to, rate, city, state,rating};
    packageDetails[id] = details;
    saveAgentFile();
    cout << "Package data updated successfully.\n";
    displayAgentData(id);
}

void LocalTravelBooking::removeAgentData()
{
    string id;
    cout << "Enter Package ID to remove: ";
    cin >> id;
    cin.ignore();
    if (packageDetails.erase(id))
    {
        saveAgentFile();
        cout << "Package data removed successfully.\n";
    }
    else
    {
        cout << "Package data not found.\n";
    }
}

void LocalTravelBooking::displayBooking(const string &id)
{
    if (bookingDetails.find(id) != bookingDetails.end())
    {
        cout << "Booking ID: " << id << "\n";
        cout << "Customer ID: " << bookingDetails[id][0] << "\n";
        cout << "Package ID: " << bookingDetails[id][1] << "\n";
    }
    else
    {
        cout << "Booking ID not found.\n";
    }
}

void LocalTravelBooking::displayAgentData(const string &id)
{
    if (packageDetails.find(id) != packageDetails.end())
    {
        cout << "Package ID: " << id << "\n";
        cout << "Mode: " << packageDetails[id][0] << "\n";
        cout << "Number of People: " << packageDetails[id][1] << "\n";
        cout << "From: " << packageDetails[id][2] << "\n";
        cout << "To: " << packageDetails[id][3] << "\n";
        cout << "Rate: " << packageDetails[id][4] << "\n";
        cout << "City: " << packageDetails[id][5] << "\n";
        cout << "State: " << packageDetails[id][6] << "\n";
    }
    else
    {
        cout << "Package ID not found.\n";
    }
}

struct Recommendations
{
    std::string name;
    double money;
    double review;
    double distance;
};

class RecommendationSystem
{
    
private:
    std::vector<Recommendations> data;

public:
    void loadDataFromFile(const std::string &filename);
    std::vector<Recommendations> filterByState(const std::string &state);
    void sortByCriterion(std::vector<Recommendations> &recommendations, const std::string &criterion);
    void displayRecommendations(const std::vector<Recommendations> &recommendations);
};


void RecommendationSystem::loadDataFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream ss(line);
        Recommendations rec;
        ss >> rec.name >> rec.money >> rec.review >> rec.distance;
        data.push_back(rec);
    }
    file.close();
}


std::vector<Recommendations> RecommendationSystem::filterByState(const std::string &state)
{
    std::vector<Recommendations> filtered;
    for (const auto &rec : data)
    {
        if (rec.name == state)
        {
            filtered.push_back(rec);
        }
    }
    return filtered;
}

void RecommendationSystem::sortByCriterion(std::vector<Recommendations> &recommendations, const std::string &criterion)
{
    if (criterion == "money")
    {
        std::sort(recommendations.begin(), recommendations.end(), [](const Recommendations &a, const Recommendations &b)
                  { return a.money < b.money; });
    }
    else if (criterion == "review")
    {
        std::sort(recommendations.begin(), recommendations.end(), [](const Recommendations &a, const Recommendations &b)
                  { return a.review > b.review; });
    }
    else if (criterion == "distance")
    {
        std::sort(recommendations.begin(), recommendations.end(), [](const Recommendations &a, const Recommendations &b)
                  { return a.distance < b.distance; });
    }
}

void RecommendationSystem::displayRecommendations(const std::vector<Recommendations> &recommendations)
{
    for (const auto &rec : recommendations)
    {
        std::cout << "Name: " << rec.name
                  << ", Money: " << rec.money
                  << ", Review: " << rec.review
                  << ", Distance: " << rec.distance << "\n";
    }
}
struct Review
{

    string place_name;
    int rating;
    string review_text;
};

struct Dish
{

    string dish_name;
    string description;
    vector<string> ingredients;
    vector<string> restaurants;
};

struct Event
{

    string event_name;
    string event_type;
    string date;
    int rating;
    string review_text;
};

struct Attraction
{

    string attraction_name;
    string description;
    string location;
    int rating;
};

struct Itinerary
{

    vector<string> daily_plan;
};
struct User
{

    string username;
    string password;
    unordered_map<string, vector<string>> itineraries;
    set<string> wishlists;
    int points = 0;
};

struct Recommendation
{

    string name;
    string reason;
};

struct Notification
{

    string message;
    time_t timestamp;
};

// Global data
unordered_map<string, User> users;

vector<Review> reviews;

std::vector<Attraction> attractions =
    {
        {"connaught place", "Delhi", "A popular tourist spot with Moderate crowd density.", 4},
        {"chandni chowk", "Delhi", "A popular tourist spot with High crowd density.", 3},
        {"karol bagh", "Delhi", "A popular tourist spot with Low crowd density.", 5},
        {"india gate", "Delhi", "A popular tourist spot with Low crowd density.", 5},
        {"red fort", "Delhi", "A popular tourist spot with Moderate crowd density.", 4},
        {"lotus temple", "Delhi", "A popular tourist spot with Low crowd density.", 5},
        {"jantar mantar", "Delhi", "A popular tourist spot with Moderate crowd density.", 4},
        {"humayun tomb", "Delhi", "A popular tourist spot with Low crowd density.", 5},
        {"qutub minar", "Delhi", "A popular tourist spot with Low crowd density.", 5},
        {"rashtrapati bhavan", "Delhi", "A popular tourist spot with Moderate crowd density.", 4},
        {"lodi garden", "Delhi", "A popular tourist spot with Moderate crowd density.", 4},
        {"janpath", "Delhi", "A popular tourist spot with High crowd density.", 3},
        {"dilli haat", "Delhi", "A popular tourist spot with Low crowd density.", 5},
        {"pragati maidan", "Delhi", "A popular tourist spot with Moderate crowd density.", 4}

};

// Global variables (replace with database or proper storage)
std::vector<Review> hotelReviews =
    {
        {"Hotel A", 4, "Good location, friendly staff"},
        {"Hotel B", 3, "Clean rooms, average service"},
        {"Hotel C", 5, "Excellent amenities, great value for money"},
        {"Hotel D", 2, "Poor maintenance, unresponsive staff"},
        {"Hotel E", 4, "Spacious rooms, convenient access to transport"},
        {"Hotel F", 1, "Unhygienic, worst experience ever"},
        {"Hotel G", 3, "Decent facilities, a bit overpriced"},
        {"Hotel H", 5, "Luxury experience, exceptional service"},
        {"Hotel I", 3, "Good for short stays, food could be better"},
        {"Hotel J", 4, "Modern decor, good customer support"}

};

std::vector<Review> attractionReviews =
    {
        {"Connaught Place", 4, "A bustling hub with great shops and restaurants."},
        {"Chandni Chowk", 3, "Historic marketplace, but very crowded."},
        {"Karol Bagh", 5, "Great for affordable shopping and local food."},
        {"India Gate", 5, "Iconic landmark with a serene atmosphere at night."},
        {"Red Fort", 4, "Beautiful architecture with rich historical significance."},
        {"Lotus Temple", 5, "Peaceful and unique design, ideal for meditation."},
        {"Jantar Mantar", 4, "Fascinating for astronomy enthusiasts, well-maintained."},
        {"Humayun's Tomb", 5, "A masterpiece of Mughal architecture, very peaceful."},
        {"Qutub Minar", 5, "An iconic structure with impressive history."},
        {"Rashtrapati Bhavan", 4, "Majestic building, a must-see during the changing of guards."},
        {"Lodi Garden", 5, "Perfect for a relaxing stroll and picnics."},
        {"Janpath", 3, "Good for street shopping, but very crowded."},
        {"Dilli Haat", 5, "Fantastic cultural experience with food and crafts."},
        {"Pragati Maidan", 4, "Great venue for exhibitions and events."}

};

std::vector<Attraction> localAttractions =
    {
        {"Connaught Place", "A vibrant hub with shopping, dining, and entertainment.", "Central Delhi", 4},
        {"Chandni Chowk", "A historic marketplace known for its food and shopping.", "Old Delhi", 3},
        {"Karol Bagh", "A bustling area with affordable shopping options.", "West Delhi", 5},
        {"India Gate", "An iconic war memorial surrounded by gardens.", "Central Delhi", 5},
        {"Red Fort", "A UNESCO World Heritage Site with rich history.", "Old Delhi", 4},
        {"Lotus Temple", "A unique temple known for its flower-like design.", "South Delhi", 5},
        {"Jantar Mantar", "An astronomical observatory with historical significance.", "Central Delhi", 4},
        {"Humayun's Tomb", "A stunning example of Mughal architecture.", "South Delhi", 5},
        {"Qutub Minar", "A historic minaret and UNESCO World Heritage Site.", "South Delhi", 5},
        {"Rashtrapati Bhavan", "The official residence of the President of India.", "Central Delhi", 4},
        {"Lodi Garden", "A peaceful park with historic tombs and lush greenery.", "South Delhi", 5},
        {"Janpath", "A lively market known for souvenirs and handicrafts.", "Central Delhi", 3},
        {"Dilli Haat", "An open-air market showcasing Indian culture and cuisine.", "South Delhi", 5},
        {"Pragati Maidan", "A major venue for exhibitions and trade fairs.", "Central Delhi", 4}

};

std::vector<Dish> localDishes =
    {
        {"Paneer Butter Masala", "A creamy and rich curry made with paneer cubes.", {"Paneer", "Tomatoes", "Butter"}, {"Royal Veg Kitchen", "Curry Delight"}},
        {"Masala Dosa", "A crispy rice crepe filled with spiced mashed potatoes.", {"Rice", "Potatoes", "Spices"}, {"South Indian Corner", "Dosa Express"}},
        {"Aloo Paratha", "A stuffed flatbread filled with spiced mashed potatoes.", {"Flour", "Potatoes", "Butter"}, {"Punjabi Dhaba", "Flatbread Haven"}},
        {"Chole Bhature", "Spiced chickpeas served with deep-fried bread.", {"Chickpeas", "Flour", "Spices"}, {"Street Flavors", "North Indian Bites"}},
        {"Rajma Chawal", "Comforting red kidney beans curry served with rice.", {"Kidney Beans", "Rice", "Spices"}, {"Home Comforts", "Veggie Delights"}},
        {"Pav Bhaji", "A spicy mashed vegetable curry served with buttered rolls.", {"Potatoes", "Tomatoes", "Spices"}, {"Mumbai Bistro", "Spicy Treats"}},
        {"Dhokla", "A fluffy steamed savory cake made with chickpeas and rice.", {"Chickpeas", "Rice", "Yogurt"}, {"Gujarati Treats", "Healthy Snacks"}},
        {"Aloo Tikki", "Crispy potato patties served with tangy chutneys.", {"Potatoes", "Spices", "Chutneys"}, {"Snack Shack", "Chaat Bazaar"}},
        {"Samosa", "A crispy fried pastry filled with spiced potatoes and peas.", {"Flour", "Potatoes", "Peas"}, {"Crispy Bites", "Street Snacks"}},
        {"Pani Puri", "Crispy hollow balls filled with tangy and spicy water.", {"Flour", "Tamarind", "Potatoes"}, {"Chaat Corner", "Flavor Street"}},
        {"Idli", "Soft steamed rice cakes served with coconut chutney.", {"Rice", "Lentils", "Yogurt"}, {"South Indian Delights", "Healthy Plate"}},
        {"Upma", "A savory semolina dish cooked with vegetables and spices.", {"Semolina", "Vegetables", "Spices"}, {"Breakfast Spot", "Veggie Comforts"}},
        {"Rasgulla", "Soft and spongy cottage cheese balls soaked in sugar syrup.", {"Milk", "Sugar", "Cardamom"}, {"Bengali Sweets", "Dessert Heaven"}},
        {"Jalebi", "Crispy deep-fried spirals soaked in sugar syrup.", {"Flour", "Sugar", "Saffron"}, {"Golden Syrup", "Sweet Treats"}},
        {"Kulfi", "Traditional Indian ice cream made with thickened milk and cardamom.", {"Milk", "Sugar", "Pistachios"}, {"Frozen Delights", "Sweet Haven"}}

};

std::vector<Event> localEvents =
    {
        {"Diwali Festival", "Cultural", "2024-11-12", 5, "A vibrant celebration with fireworks, lights, and cultural performances."},
        {"Qutub Minar Light & Sound Show", "Cultural", "2024-12-15", 4, "An amazing light and sound show that brings history to life."},
        {"Rath Yatra", "Cultural", "2024-12-20", 5, "A colorful procession with vibrant floats, music, and dances."},
        {"International Dance Festival", "Art", "2024-12-22", 4, "A showcase of traditional and modern dance forms from around the world."},
        {"Lodi Garden Music Fest", "Music", "2024-12-24", 4, "An open-air music festival with performances by local and international artists."},
        {"Cultural Food Festival", "Culinary", "2024-12-26", 5, "A food lover’s paradise with dishes from various regions of India."},
        {"Mela at Chandni Chowk", "Cultural", "2024-12-30", 3, "A traditional fair with games, street food, and local handicrafts."},
        {"Winter Art Show", "Art", "2024-12-31", 4, "An art exhibition featuring contemporary Indian artists and sculptures."},
        {"Humayun's Tomb Heritage Walk", "Heritage", "2025-01-02", 4, "A guided tour through one of Delhi’s most iconic Mughal monuments."},
        {"Street Photography Exhibition", "Art", "2025-01-05", 3, "A collection of stunning street photography depicting life in Delhi."},
        {"Pragati Maidan Trade Show", "Business", "2025-01-10", 4, "A trade show showcasing the latest innovations and trends across various industries."},
        {"Lodi Garden Yoga Fest", "Health", "2025-01-12", 5, "A peaceful yoga and wellness event surrounded by nature in Lodi Garden."},
        {"Sufi Night at Dargah", "Music", "2025-01-15", 4, "A soulful evening of Sufi music and poetry under the stars."},
        {"Red Fort Historical Tour", "Heritage", "2025-01-20", 4, "An immersive tour exploring the history and architecture of the Red Fort."}

};

unordered_map<string, Itinerary> userItineraries;

unordered_map<string, set<string>> userWishlists;

vector<Notification> notifications;

// Helper functions
bool userExists(const string &username)
{

    return users.find(username) != users.end();
}

// Function to display reviews
void displayReviews(const vector<Review> &reviews)
{

    if (reviews.empty())

    {
        cout << "No reviews available.\n";
        return;
    }

    for (const Review &review : reviews)

    {
        cout << "Place: " << review.place_name << ", Rating: " << review.rating << endl;
        cout << "Review: " << review.review_text << endl
             << endl;
    }
}

// Function to leave feedback
void leaveFeedback(vector<Review> &reviews)
{

    string place_name, review_text;
    int rating;

    cin.ignore(); // Clear input buffer
    cout << "Enter the place name: ";
    getline(cin, place_name);
    cout << "Enter your rating (1-5): ";
    cin >> rating;

    if (rating < 1 || rating > 5)
    {

        cout << "Invalid rating. Please enter a number between 1 and 5.\n";
        return;
    }

    cin.ignore(); // Clear input buffer
    cout << "Enter your review: ";
    getline(cin, review_text);

    reviews.push_back({place_name, rating, review_text});
    cout << "Thank you for your feedback!\n";
}

// Function to search and filter reviews by rating
void searchReviewsByRating(const vector<Review> &reviews, int minRating)
{

    bool found = false;
    for (const Review &review : reviews)

    {

        if (review.rating >= minRating)
        {
            cout << "Place: " << review.place_name << ", Rating: " << review.rating << endl;
            cout << "Review: " << review.review_text << endl
                 << endl;
            found = true;
        }
    }

    if (!found)
    {

        cout << "No reviews found with a rating of " << minRating << " or higher.\n";
    }
}

// Function to display local dishes
void viewLocalDishes()
{

    if (localDishes.empty())
    {
        cout << "No local dishes available.\n";
        return;
    }

    for (const Dish &dish : localDishes)

    {
        cout << "Dish: " << dish.dish_name << endl;
        cout << "Description: " << dish.description << endl;
        cout << "Ingredients: ";

        for (const string &ingredient : dish.ingredients)
        {
            cout << ingredient << ", ";
        }
        cout << "\nRestaurants specializing in this dish: ";

        for (const string &restaurant : dish.restaurants)
        {
            cout << restaurant << ", ";
        }
        cout << "\n\n";
    }
}

// Function to display local attractions
void viewLocalAttractions()
{

    if (localAttractions.empty())
    {
        cout << "No local attractions available.\n";
        return;
    }

    for (const Attraction &attraction : localAttractions)
    {

        cout << "Attraction: " << attraction.attraction_name << endl;
        cout << "Description: " << attraction.description << endl;
        cout << "Location: " << attraction.location << endl;
        cout << "Rating: " << attraction.rating << endl
             << endl;
    }
}

// Function to display local events
void viewLocalEvents()
{

    if (localEvents.empty())
    {
        cout << "No local events available.\n";
        return;
    }

    for (const Event &event : localEvents)
    {

        cout << "Event: " << event.event_name << endl;
        cout << "Type: " << event.event_type << endl;
        cout << "Date: " << event.date << endl;
        cout << "Rating: " << event.rating << endl;
        cout << "Review: " << event.review_text << endl
             << endl;
    }
}

// Function to recommend dishes near attractions
void recommendDishesNearAttraction()
{

    if (localDishes.empty())
    {
        cout << "No dishes available to recommend.\n";
        return;
    }

    for (const Dish &dish : localDishes)
    {

        cout << "Dish: " << dish.dish_name << " | Available at: ";

        for (const string &restaurant : dish.restaurants)
        {
            cout << restaurant << ", ";
        }

        cout << endl;
    }
}

// Function to manage the itinerary
void manageItinerary(const string &username)
{

    if (userItineraries.find(username) == userItineraries.end())
    {
        userItineraries[username] = {{"Visit Monument X", "Try Biryani at Restaurant A"}};
    }

    Itinerary &itinerary = userItineraries[username];
    cout << "Current Itinerary:\n";

    for (size_t i = 0; i < itinerary.daily_plan.size(); ++i)
    {
        cout << i + 1 << ". " << itinerary.daily_plan[i] << endl;
    }

    int choice;
    cout << "1. Add Activity\n2. Remove Activity\n3. Clear Itinerary\n4. Exit\n";
    cin >> choice;

    if (choice == 1)
    {
        cin.ignore();
        string activity;
        cout << "Enter the activity to add: ";
        getline(cin, activity);
        itinerary.daily_plan.push_back(activity);
        cout << "Activity added.\n";
    }

    else if (choice == 2)

    {
        int index;
        cout << "Enter the index of the activity to remove: ";
        cin >> index;
        if (index > 0 && index <= itinerary.daily_plan.size())

        {
            itinerary.daily_plan.erase(itinerary.daily_plan.begin() + index - 1);
            cout << "Activity removed.\n";
        }

        else

        {
            cout << "Invalid index.\n";
        }
    }
    else if (choice == 3)
    {
        itinerary.daily_plan.clear();
        cout << "Itinerary cleared.\n";
    }
}

// Function to manage wishlist
void manageWishlist(const string &username)
{

    int choice;
    cout << "1. View Wishlist\n2. Add to Wishlist\n3. Exit\n";
    cin >> choice;

    if (choice == 1)
    {

        if (userWishlists[username].empty())
        {
            cout << "Wishlist is empty.\n";
        }

        else
        {
            cout << "Wishlist for " << username << ":\n";

            for (const string &item : userWishlists[username])
            {
                cout << "- " << item << endl;
            }
        }
    }

    else if (choice == 2)
    {

        cin.ignore();
        string item;
        cout << "Enter item to add to wishlist: ";
        getline(cin, item);
        userWishlists[username].insert(item);
        cout << "Item added to wishlist.\n";
    }
}

void sendNotification(const string &message)
{

    time_t now = time(0);
    notifications.push_back({message, now});
    cout << "Notification: " << message << endl;
}

// 1. Advanced Recommendation System
void recommendAttractions(const string &username)
{
    cout << "Recommended Attractions for " << username << ":\n";

    for (const Attraction &attraction : attractions)
    {

        if (attraction.rating >= 4)
        {
            cout << "- " << attraction.attraction_name << " (" << attraction.location << ")\n";
        }
    }
}

// 3. Real-time Notifications
void displayNotifications()
{

    cout << "\nNotifications:\n";

    for (const Notification &notif : notifications)
    {
        cout << "- " << notif.message << " at " << ctime(&notif.timestamp);
    }
}

// 4. Detailed Reviews
void viewDetailedReviews()
{

    for (const Review &review : reviews)
    {
        cout << "Place: " << review.place_name << ", Rating: " << review.rating << "/5\n";
        cout << "Review: " << review.review_text << "\n";
    }
}

// 6. Social Features
void viewUserWishlists(const string &username)
{
    if (users[username].wishlists.empty())
    {
        cout << "Your wishlist is empty.\n";
        return;
    }

    cout << "Wishlist for " << username << ":\n";

    for (const string &item : users[username].wishlists)
    {
        cout << "- " << item << "\n";
    }
}

// 8. Location-based Filtering
void searchAttractionsByLocation(const string &location)
{

    cout << "Attractions in " << location << ":\n";

    for (const Attraction &attraction : attractions)
    {
        if (attraction.location == location)
        {
            cout << "- " << attraction.attraction_name << "\n";
        }
    }
}

// 9. Multi-language Support (simplified as prompts)
void selectLanguage()
{
    cout << "Select your language: 1. English 2. French 3. Spanish\n";
    int choice;
    cin >> choice;

    if (choice == 1)
    {
        cout << "Language set to English.\n";
    }

    else if (choice == 2)
    {
        cout << "Langue réglée sur le français.\n";
    }

    else if (choice == 3)
    {
        cout << "Idioma configurado a español.\n";
    }

    else
    {
        cout << "Invalid choice. Defaulting to English.\n";
    }
}

// 10. Admin Dashboard
void adminDashboard()
{
    cout << "Admin Dashboard:\n";
    cout << "Total Users: " << users.size() << "\n";
    cout << "Total Reviews: " << reviews.size() << "\n";
    cout << "Total Attractions: " << attractions.size() << "\n";
}

// 11. Improved Queue Management (simplified)
vector<string> activityQueue;
void manageActivityQueue()
{
    cout << "1. Add to Queue 2. View Queue 3. Remove from Queue\n";
    int choice;
    cin >> choice;
    cin.ignore();

    if (choice == 1)
    {
        string activity;
        cout << "Enter activity: ";
        getline(cin, activity);
        activityQueue.push_back(activity);
        cout << "Activity added!\n";
    }

    else if (choice == 2)
    {
        cout << "Activity Queue:\n";

        for (const string &activity : activityQueue)
        {
            cout << "- " << activity << "\n";
        }
    }

    else if (choice == 3)
    {
        if (!activityQueue.empty())
        {
            activityQueue.erase(activityQueue.begin());
            cout << "Activity removed!\n";
        }

        else
        {
            cout << "Queue is empty.\n";
        }
    }
}

// 12. Advanced Search
void advancedSearch(const string &query)
{
    cout << "Search Results for '" << query << "':\n";

    for (const Attraction &attraction : attractions)
    {
        if (attraction.attraction_name.find(query) != string::npos || attraction.description.find(query) != string::npos)
        {
            cout << "- " << attraction.attraction_name << ", " << attraction.location << "\n";
        }
    }
}

// 13. User Achievements and Points
void addPoints(const string &username, int points)
{
    users[username].points += points;
    cout << username << " earned " << points << " points! Total: " << users[username].points << "\n";
}

// 14. Feedback System
void collectFeedback()
{
    string feedback;
    cin.ignore();
    cout << "Enter your feedback: ";

    getline(cin, feedback);
    cout << "Thank you for your feedback!\n";
}

int read_other()
{
    // Select language
    selectLanguage();

    while (true)
    {
        cout << "\nMain Menu:\n";
        cout << "1. Login/Sign Up\n";
        cout << "2. Admin Dashboard\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1)
        {
            string username;
            cout << "Enter username: ";
            cin >> username;
            cin.ignore();

            if (!userExists(username))
            {
                string password;
                cout << "New user detected. Enter password to create account: ";
                getline(cin, password);
                users[username] = {username, password};
                cout << "Account created successfully!\n";
            }

            else
            {
                cout << "Welcome back, " << username << "!\n";
            }

            // User-specific menu
            while (true)
            {
                cout << "\nUser Menu:\n";
                cout << "1. View Recommended Attractions\n";
                cout << "2. Manage Itinerary\n";
                cout << "3. View Notifications\n";
                cout << "4. Search Attractions by Location\n";
                cout << "5. View Wishlist\n";
                cout << "6. Add Review\n";
                cout << "7. Exit to Main Menu\n";
                cout << "Enter your choice: ";
                int userChoice;
                cin >> userChoice;
                cin.ignore();

                if (userChoice == 1)
                {
                    recommendAttractions(username);
                }

                else if (userChoice == 2)
                {
                    manageItinerary(username);
                }

                else if (userChoice == 3)
                {
                    displayNotifications();
                }

                else if (userChoice == 4)
                {
                    cout << "Enter location: ";
                    string location;
                    getline(cin, location);
                    searchAttractionsByLocation(location);
                }

                else if (userChoice == 5)
                {
                    viewUserWishlists(username);
                }

                else if (userChoice == 6)
                {

                    cout << "Enter place name: ";
                    string place;
                    getline(cin, place);
                    cout << "Enter rating (1-5): ";
                    int rating;
                    cin >> rating;
                    cin.ignore();
                    cout << "Enter your review: ";
                    string reviewText;
                    getline(cin, reviewText);
                    reviews.push_back({place, rating, reviewText});
                    cout << "Review added successfully!\n";
                    addPoints(username, 10); // Reward points for adding reviews
                }

                else if (userChoice == 7)
                {
                    break; // Exit to main menu
                }

                else
                {
                    cout << "Invalid choice. Please try again.\n";
                }
            }
        }

        else if (choice == 2)
        {
            adminDashboard();
        }

        else if (choice == 3)
        {
            cout << "Thank you for using the Travel Planner Application. Goodbye!\n";
            break;
        }

        else
        {
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}

void planner()
{
    string username;
    int choice;

    cout << "Welcome to the Travel Planner!" << endl;
    cout << "Enter your username: ";
    cin >> username;

    cin.ignore(); // Clear input buffer

    do
    {
        // Display the main menu
        cout << "\nMain Menu:\n";
        cout << "1. View Hotel Reviews\n";
        cout << "2. View Attraction Reviews\n";
        cout << "3. View Local Attractions\n";
        cout << "4. View Local Dishes\n";
        cout << "5. Recommend Dishes Near Attraction\n";
        cout << "6. View Local Events\n";
        cout << "7. Leave Feedback\n";
        cout << "8. Search Reviews by Rating\n";
        cout << "9. Manage Itinerary\n";
        cout << "10. Manage Wishlist\n";
        cout << "11.Other\n";
        cout << "12. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        // Handle user choice
        switch (choice)

        {
        case 1:
            displayReviews(hotelReviews); // View hotel reviews
            break;

        case 2:
            displayReviews(attractionReviews); // View attraction reviews
            break;

        case 3:
            viewLocalAttractions(); // View local attractions
            break;

        case 4:
            viewLocalDishes(); // View local dishes
            break;

        case 5:
            recommendDishesNearAttraction(); // Recommend dishes near an attraction
            break;

        case 6:
            viewLocalEvents(); // View local events
            break;

        case 7:

        {
            int feedbackType;
            cout << "Leave Feedback for:\n1. Hotels\n2. Attractions\nEnter your choice: ";
            cin >> feedbackType;

            if (feedbackType == 1)
            {
                leaveFeedback(hotelReviews);
            }

            else if (feedbackType == 2)
            {
                leaveFeedback(attractionReviews);
            }

            else
            {
                cout << "Invalid option. Please choose either 1 or 2.\n";
            }

            break;
        }

        case 8:

        {
            int minRating;
            cout << "Enter the minimum rating to filter reviews by: ";
            cin >> minRating;

            if (minRating >= 1 && minRating <= 5)
            {
                searchReviewsByRating(attractionReviews, minRating);
            }

            else
            {
                cout << "Invalid rating. Please enter a number between 1 and 5.\n";
            }

            break;
        }

        case 9:
            manageItinerary(username); // Manage the user's itinerary
            break;

        case 10:
            manageWishlist(username); // Manage the user's wishlist
            break;

        case 11:
            read_other();
            break;

        case 12:
            cout << "Thank you for using the Travel Planner! Goodbye.\n";
            break;

        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }

    }

    while (choice != 11);
}

unordered_map<string, unordered_map<string, int>> cityRoutes = {
    // Famous locations in Delhi
    {"connaught place", {{"chandni chowk", 2}, {"karol bagh", 3}, {"india gate", 3}, {"red fort", 5}, {"lotus temple", 8}, {"qutub minar", 14}, {"humayun's tomb", 12}, {"akshardham temple", 16}, {"parliament house", 4}, {"janpath", 1}}},
    {"chandni chowk", {{"connaught place", 2}, {"karol bagh", 6}, {"india gate", 5}, {"red fort", 3}, {"lotus temple", 9}, {"qutub minar", 15}, {"humayun's tomb", 13}, {"akshardham temple", 18}, {"parliament house", 8}, {"janpath", 7}}},
    {"karol bagh", {{"connaught place", 3}, {"chandni chowk", 6}, {"india gate", 4}, {"red fort", 5}, {"lotus temple", 7}, {"qutub minar", 11}, {"humayun's tomb", 9}, {"akshardham temple", 13}, {"parliament house", 7}, {"janpath", 4}}},
    {"india gate", {{"connaught place", 3}, {"chandni chowk", 5}, {"karol bagh", 4}, {"red fort", 6}, {"lotus temple", 8}, {"qutub minar", 12}, {"humayun's tomb", 10}, {"akshardham temple", 14}, {"parliament house", 5}, {"janpath", 6}}},
    {"red fort", {{"connaught place", 5}, {"chandni chowk", 3}, {"karol bagh", 5}, {"india gate", 6}, {"lotus temple", 10}, {"qutub minar", 15}, {"humayun's tomb", 13}, {"akshardham temple", 17}, {"parliament house", 9}, {"janpath", 7}}},
    {"lotus temple", {{"connaught place", 8}, {"chandni chowk", 9}, {"karol bagh", 7}, {"india gate", 8}, {"red fort", 10}, {"qutub minar", 18}, {"humayun's tomb", 16}, {"akshardham temple", 19}, {"parliament house", 8}, {"janpath", 6}}},
    {"qutub minar", {{"connaught place", 14}, {"chandni chowk", 15}, {"karol bagh", 11}, {"india gate", 12}, {"red fort", 15}, {"lotus temple", 18}, {"humayun's tomb", 17}, {"akshardham temple", 22}, {"parliament house", 16}, {"janpath", 13}}},
    {"humayun's tomb", {{"connaught place", 12}, {"chandni chowk", 13}, {"karol bagh", 9}, {"india gate", 10}, {"red fort", 13}, {"lotus temple", 16}, {"qutub minar", 17}, {"akshardham temple", 20}, {"parliament house", 12}, {"janpath", 10}}},
    {"akshardham temple", {{"connaught place", 16}, {"chandni chowk", 18}, {"karol bagh", 13}, {"india gate", 14}, {"red fort", 17}, {"lotus temple", 19}, {"qutub minar", 22}, {"humayun's tomb", 20}, {"parliament house", 17}, {"janpath", 14}}},
    {"parliament house", {{"connaught place", 4}, {"chandni chowk", 8}, {"karol bagh", 7}, {"india gate", 5}, {"red fort", 9}, {"lotus temple", 8}, {"qutub minar", 16}, {"humayun's tomb", 12}, {"akshardham temple", 17}, {"janpath", 4}}},
    {"janpath", {{"connaught place", 1}, {"chandni chowk", 7}, {"karol bagh", 4}, {"india gate", 6}, {"red fort", 7}, {"lotus temple", 6}, {"qutub minar", 13}, {"humayun's tomb", 10}, {"akshardham temple", 14}, {"parliament house", 4}}},
    {"jantar mantar", {{"connaught place", 2}, {"chandni chowk", 5}, {"karol bagh", 4}, {"india gate", 3}, {"red fort", 6}, {"lotus temple", 8}, {"qutub minar", 15}, {"humayun's tomb", 12}, {"akshardham temple", 17}, {"parliament house", 3}}},
    {"lodhi garden", {{"connaught place", 5}, {"chandni chowk", 9}, {"karol bagh", 6}, {"india gate", 4}, {"red fort", 8}, {"lotus temple", 10}, {"qutub minar", 14}, {"humayun's tomb", 8}, {"akshardham temple", 13}, {"parliament house", 6}}},
    {"raj ghat", {{"connaught place", 6}, {"chandni chowk", 3}, {"karol bagh", 5}, {"india gate", 2}, {"red fort", 3}, {"lotus temple", 9}, {"qutub minar", 13}, {"humayun's tomb", 11}, {"akshardham temple", 16}, {"parliament house", 7}}},
    {"india habitat centre", {{"connaught place", 4}, {"chandni chowk", 7}, {"karol bagh", 5}, {"india gate", 3}, {"red fort", 6}, {"lotus temple", 9}, {"qutub minar", 11}, {"humayun's tomb", 8}, {"akshardham temple", 13}, {"parliament house", 5}}},
    {"safdarjung tomb", {{"connaught place", 9}, {"chandni chowk", 12}, {"karol bagh", 10}, {"india gate", 6}, {"red fort", 9}, {"lotus temple", 7}, {"qutub minar", 16}, {"humayun's tomb", 11}, {"akshardham temple", 14}, {"parliament house", 10}}},
    {"purana qila", {{"connaught place", 6}, {"chandni chowk", 10}, {"karol bagh", 7}, {"india gate", 5}, {"red fort", 7}, {"lotus temple", 9}, {"qutub minar", 17}, {"humayun's tomb", 14}, {"akshardham temple", 19}, {"parliament house", 8}}},
    {"iskcon temple", {{"connaught place", 11}, {"chandni chowk", 15}, {"karol bagh", 13}, {"india gate", 14}, {"red fort", 16}, {"lotus temple", 12}, {"qutub minar", 20}, {"humayun's tomb", 16}, {"akshardham temple", 22}, {"parliament house", 14}}},
    {"garden of five senses", {{"connaught place", 15}, {"chandni chowk", 18}, {"karol bagh", 16}, {"india gate", 12}, {"red fort", 17}, {"lotus temple", 11}, {"qutub minar", 22}, {"humayun's tomb", 17}, {"akshardham temple", 25}, {"parliament house", 15}}},
    {"sultan garhi tomb", {{"connaught place", 18}, {"chandni chowk", 22}, {"karol bagh", 19}, {"india gate", 14}, {"red fort", 20}, {"lotus temple", 16}, {"qutub minar", 23}, {"humayun's tomb", 19}, {"akshardham temple", 26}, {"parliament house", 17}}},
    {"nizamuddin dargah", {{"connaught place", 10}, {"chandni chowk", 14}, {"karol bagh", 11}, {"india gate", 9}, {"red fort", 13}, {"lotus temple", 15}, {"qutub minar", 19}, {"humayun's tomb", 16}, {"akshardham temple", 22}, {"parliament house", 12}}},
    {"shankar's international dolls museum", {{"connaught place", 7}, {"chandni chowk", 10}, {"karol bagh", 8}, {"india gate", 6}, {"red fort", 9}, {"lotus temple", 11}, {"qutub minar", 16}, {"humayun's tomb", 14}, {"akshardham temple", 19}, {"parliament house", 9}}}};

// Crowd density for each location
unordered_map<string, string> crowdDensity = {
    {"connaught place", "Moderate"},
    {"chandni chowk", "High"},
    {"karol bagh", "Low"},
    {"india gate", "Low"},
    {"red fort", "Moderate"},
    {"lotus temple", "Low"},
    {"qutub minar", "Moderate"},
    {"humayun's tomb", "Low"},
    {"akshardham temple", "High"},
    {"parliament house", "Low"},
    {"janpath", "High"},
    {"jantar mantar", "Moderate"},
    {"lodhi garden", "Low"},
    {"raj ghat", "Moderate"},
    {"india habitat centre", "Low"},
    {"safdarjung tomb", "Low"},
    {"purana qila", "Moderate"},
    {"iskcon temple", "Moderate"},
    {"garden of five senses", "Low"},
    {"sultan garhi tomb", "Low"},
    {"nizamuddin dargah", "Moderate"},
    {"shankar's international dolls museum", "Low"}};

// Weather conditions for each location
unordered_map<string, string> weatherConditions = {
    {"connaught place", "Clear"},
    {"chandni chowk", "Cloudy"},
    {"karol bagh", "Sunny"},
    {"india gate", "Clear"},
    {"red fort", "Partly cloudy"},
    {"lotus temple", "Sunny"},
    {"qutub minar", "Clear"},
    {"humayun's tomb", "Partly cloudy"},
    {"akshardham temple", "Rainy"},
    {"parliament house", "Sunny"},
    {"janpath", "Clear"},
    {"jantar mantar", "Clear"},
    {"lodhi garden", "Sunny"},
    {"raj ghat", "Clear"},
    {"india habitat centre", "Clear"},
    {"safdarjung tomb", "Cloudy"},
    {"purana qila", "Clear"},
    {"iskcon temple", "Clear"},
    {"garden of five senses", "Sunny"},
    {"sultan garhi tomb", "Clear"},
    {"nizamuddin dargah", "Partly cloudy"},
    {"shankar's international dolls museum", "Clear"}};

// Location conditions (Road Status and Similar Factors)
unordered_map<string, string> locationConditions = {
    {"connaught place", "Normal road, busy area"},
    {"chandni chowk", "Under construction, traffic jams"},
    {"karol bagh", "Normal road, slightly congested"},
    {"india gate", "Clear roads, pedestrian-friendly"},
    {"red fort", "Normal road, moderate traffic"},
    {"lotus temple", "Clear roads, peaceful surroundings"},
    {"qutub minar", "Clear roads, historical site"},
    {"humayun's tomb", "Moderate traffic, busy area"},
    {"akshardham temple", "Under construction, crowded"},
    {"parliament house", "Normal road, government area"},
    {"janpath", "Moderate traffic, shopping area"},
    {"jantar mantar", "Normal road, busy area"},
    {"lodhi garden", "Clear roads, peaceful park"},
    {"raj ghat", "Clear roads, memorial area"},
    {"india habitat centre", "Clear roads, cultural venue"},
    {"safdarjung tomb", "Normal road, quiet area"},
    {"purana qila", "Clear roads, historical site"},
    {"iskcon temple", "Normal road, religious area"},
    {"garden of five senses", "Clear roads, nature park"},
    {"sultan garhi tomb", "Clear roads, quiet area"},
    {"nizamuddin dargah", "Moderate traffic, religious site"},
    {"shankar's international dolls museum", "Clear roads, museum area"}};

// Function Prototypes
bool cityguide();
bool administratorguide();
bool userguide();
void updateWeatherConditions();
void updateCrowdDensity();
void updateLocationConditions();
void viewCityRoutes();
void findShortestPath();
void viewSpecificDetails();
void showRouteAndDistance(const string &start, const string &end);
int findShortestDistance(const string &start, const string &end, vector<string> &path);
string toLowerCase(const string &str);
string normalizeInput(const string &input);
bool isValidLocation(const string &location);
void addLocation();
void deleteLocation();

// Helper Functions
string toLowerCase(const string &str)
{
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

string normalizeInput(const string &input)
{
    return toLowerCase(input);
}

bool isValidLocation(const string &location)
{
    string locLower = normalizeInput(location);
    return cityRoutes.find(locLower) != cityRoutes.end();
}

// Add Location Function
void addLocation()
{
    string locationName;
    cout << "Enter the name of the location to add: ";
    cin.ignore();
    getline(cin, locationName);
    string normalizedLocation = normalizeInput(locationName);

    if (cityRoutes.find(normalizedLocation) != cityRoutes.end())
    {
        cout << "This location already exists." << endl;
        return;
    }

    unordered_map<string, int> routes;
    cout << "Enter routes from this location to other locations (enter 'done' to stop):" << endl;

    string destination;
    int distance;

    while (true)
    {
        cout << "Destination: ";
        getline(cin, destination);
        if (destination == "done")
            break;

        cout << "Distance to " << destination << ": ";
        cin >> distance;
        cin.ignore();

        routes[normalizeInput(destination)] = distance;
    }

    cityRoutes[normalizedLocation] = routes;
    crowdDensity[normalizedLocation] = "Low";          // Default crowd density
    weatherConditions[normalizedLocation] = "Clear";   // Default weather
    locationConditions[normalizedLocation] = "Normal"; // Default condition

    cout << "Location added successfully!" << endl;
}

// Delete Location Function
void deleteLocation()
{
    string locationName;
    cout << "Enter the name of the location to delete: ";
    cin.ignore();
    getline(cin, locationName);

    string normalizedLocation = normalizeInput(locationName);

    if (cityRoutes.find(normalizedLocation) == cityRoutes.end())
    {
        cout << "Location not found!" << endl;
        return;
    }

    cityRoutes.erase(normalizedLocation);
    crowdDensity.erase(normalizedLocation);
    weatherConditions.erase(normalizedLocation);
    locationConditions.erase(normalizedLocation);

    cout << "Location deleted successfully!" << endl;
}

// Find Shortest Distance
int findShortestDistance(const string &start, const string &end, vector<string> &path)
{
    unordered_map<string, int> distances;
    unordered_map<string, string> parents;
    unordered_map<string, bool> visited;

    for (const auto &loc : cityRoutes)
    {
        distances[loc.first] = numeric_limits<int>::max();
        visited[loc.first] = false;
    }

    distances[start] = 0;
    parents[start] = "";

    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> pq;
    pq.push({0, start});

    while (!pq.empty())
    {
        auto top = pq.top();
        int currDist = top.first;
        string currNode = top.second;
        pq.pop();

        if (visited[currNode])
            continue;
        visited[currNode] = true;

        for (const auto &neighbor : cityRoutes[currNode])
        {
            int newDist = currDist + neighbor.second;
            if (newDist < distances[neighbor.first])
            {
                distances[neighbor.first] = newDist;
                parents[neighbor.first] = currNode;
                pq.push({newDist, neighbor.first});
            }
        }
    }

    path.clear();
    if (distances[end] != numeric_limits<int>::max())
    {
        for (string at = end; !at.empty(); at = parents[at])
        {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());
    }

    return distances[end];
}

// Show City Routes
void viewCityRoutes()
{
    cout << "\n========== City Routes ==========" << endl;
    for (const auto &loc : cityRoutes)
    {
        cout << "\n"
             << loc.first << " connects to:" << endl;
        for (const auto &neighbor : loc.second)
        {
            cout << "  - " << neighbor.first << " (" << neighbor.second << " km)" << endl;
        }
        cout << "----------------------------------" << endl;
    }
}

// Show Route and Distance between Any Two Locations
void showRouteAndDistance(const string &start, const string &end)
{
    string normalizedStart = normalizeInput(start);
    string normalizedEnd = normalizeInput(end);

    if (!isValidLocation(normalizedStart))
    {
        cout << "Starting location \"" << start << "\" is not found in the database." << endl;
        return;
    }

    if (!isValidLocation(normalizedEnd))
    {
        cout << "Destination location \"" << end << "\" is not found in the database." << endl;
        return;
    }

    vector<string> path;
    int distance = findShortestDistance(normalizedStart, normalizedEnd, path);

    if (distance == numeric_limits<int>::max())
    {
        cout << "No direct route exists between " << start << " and " << end << ". Suggesting closest locations:" << endl;

        int minStartDist = numeric_limits<int>::max(), minEndDist = numeric_limits<int>::max();
        string closestStart = "", closestEnd = "";

        for (auto &neighbor : cityRoutes[normalizedStart])
        {
            if (neighbor.second < minStartDist)
            {
                closestStart = neighbor.first;
                minStartDist = neighbor.second;
            }
        }

        for (auto &neighbor : cityRoutes[normalizedEnd])
        {
            if (neighbor.second < minEndDist)
            {
                closestEnd = neighbor.first;
                minEndDist = neighbor.second;
            }
        }

        if (!closestStart.empty() && !closestEnd.empty())
        {
            cout << "Cut route: " << start << " -> " << closestStart << " -> " << closestEnd << " -> " << end << endl;
            cout << "Approximate distance: " << (minStartDist + minEndDist) << " km." << endl;
        }
        else
        {
            cout << "No feasible cut route found." << endl;
        }
    }
    else
    {
        cout << "The shortest distance between " << start << " and " << end << " is " << distance << " km." << endl;
        cout << "Path: ";
        for (const auto &loc : path)
        {
            cout << loc;
            if (loc != path.back())
                cout << " -> ";
        }
        cout << endl;
    }
}

// View Specific Details
void viewSpecificDetails()
{
    string location;
    cout << "Enter the location name: ";
    cin.ignore();
    getline(cin, location);

    string normalizedLocation = normalizeInput(location);
    if (!isValidLocation(normalizedLocation))
    {
        cout << "Location not found in the database." << endl;
        return;
    }

    cout << "Weather Condition: " << weatherConditions[normalizedLocation] << endl;
    cout << "Crowd Density: " << crowdDensity[normalizedLocation] << endl;
    cout << "Location Condition: " << locationConditions[normalizedLocation] << endl;
}

// Update Weather Conditions
void updateWeatherConditions()
{
    string location, condition;
    cout << "Enter the location name: ";
    cin.ignore();
    getline(cin, location);
    cout << "Enter the new weather condition (e.g., Clear, Rainy, etc.): ";
    getline(cin, condition);

    weatherConditions[normalizeInput(location)] = condition;
    cout << "Weather condition updated!" << endl;
}

// Update Crowd Density
void updateCrowdDensity()
{
    string location, density;
    cout << "Enter the location name: ";
    cin.ignore();
    getline(cin, location);
    cout << "Enter the new crowd density (Low, Moderate, High): ";
    getline(cin, density);

    crowdDensity[normalizeInput(location)] = density;
    cout << "Crowd density updated!" << endl;
}

// Update Location Conditions
void updateLocationConditions()
{
    string location, condition;
    cout << "Enter the location name: ";
    cin.ignore();
    getline(cin, location);
    cout << "Enter the new location condition (Normal, Road Blocked, etc.): ";
    getline(cin, condition);

    locationConditions[normalizeInput(location)] = condition;
    cout << "Location condition updated!" << endl;
}

// Main Menu
bool cityguide()
{
    int choice;
    do
    {
        cout << "\nWelcome to the City Guide System!" << endl;
        cout << "1. Administrator Menu" << endl;
        cout << "2. User Menu" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            administratorguide();
            break;
        case 2:
            userguide();
            break;
        case 3:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 3);
}

// Administrator Menu
bool administratorguide()
{
    int choice;
    do
    {
        cout << "\n===== Administrator Menu =====" << endl;
        cout << "1. Add Location" << endl;
        cout << "2. Delete Location" << endl;
        cout << "3. View City Routes" << endl;
        cout << "4. Update Weather Conditions" << endl;
        cout << "5. Update Crowd Density" << endl;
        cout << "6. Update Location Conditions" << endl;
        cout << "7. Return to Main Menu" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            addLocation();
            break;
        case 2:
            deleteLocation();
            break;
        case 3:
            viewCityRoutes();
            break;
        case 4:
            updateWeatherConditions();
            break;
        case 5:
            updateCrowdDensity();
            break;
        case 6:
            updateLocationConditions();
            break;
        case 7:
            return true;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 7);
    return false;
}
void findNearbyLocations()
{
    string currentLocation;
    int maxDistance;

    cout << "Enter your current location: ";
    cin.ignore();
    getline(cin, currentLocation);

    // Normalize the input location
    string normalizedLocation = normalizeInput(currentLocation);

    // Check if the location is valid
    if (!isValidLocation(normalizedLocation))
    {
        cout << "Location not found in the database." << endl;
        return;
    }

    cout << "Enter the maximum distance (in km) within which you want to find nearby locations: ";
    cin >> maxDistance;

    cout << "\nNearby locations within " << maxDistance << " km from " << currentLocation << " are:" << endl;

    // Check all routes from the current location and print those within the specified distance
    bool found = false;
    for (const auto &neighbor : cityRoutes[normalizedLocation])
    {
        if (neighbor.second <= maxDistance)
        {
            cout << neighbor.first << " (" << neighbor.second << " km)" << endl;
            found = true;
        }
    }

    if (!found)
    {
        cout << "No locations found within " << maxDistance << " km." << endl;
    }
}

bool userguide()
{
    int choice;
    do
    {
        cout << "\n===== User Menu =====\n" << endl;
        cout << "1. View City Routes" << endl;
        cout << "2. Find Shortest Path between Locations" << endl;
        cout << "3. View Specific Details for a Location" << endl;
        cout << "4. Find Nearby Locations" << endl; // Added new option
        cout << "5. Return to Main Menu" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            viewCityRoutes();
            break;
        case 2:
        {
            string start, end;
            cout << "Enter start location: ";
            cin.ignore();
            getline(cin, start);
            cout << "Enter end location: ";
            getline(cin, end);
            showRouteAndDistance(start, end);
            break;
        }
        case 3:
            viewSpecificDetails();
            break;
        case 4:
            findNearbyLocations(); // Calling the new function
            break;
        case 5:
            return true;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 5);
    return false;
}

void userMenu(TravelBooking &travelBooking, HotelBooking &hotelBooking, LocalTravelBooking &localTravelBooking, RecommendationSystem &recommendationSystem, GraphCreator &graphCreator, int cid)
{
    int bookingType;
    cout << "1. Manage Travel Bookings\n";
    cout << "2. Manage Hotel Bookings\n";
    cout << "3. Manage Local Travel Bookings\n";
    cout << "4. Guide\n";
    cout << "5. Travel Planner\n";
    cout << "Enter your choice: ";
    cin >> bookingType;

    int choice;
    do
    {
        switch (bookingType)
        {
        case 1: // Travel Booking
            cout << "\n Your Cid : " << cid << endl;
            cout << "\n--- User Menu ---\n";
            cout << "1. Add Booking\n";
            cout << "2. Update Booking\n";
            cout << "3. Cancel Booking\n";
            cout << "4. View All Bookings\n";
            cout << "5. View Packages by Rate\n";
            cout << "6. View Packages by Number of People\n";
            cout << "7. View Packages by Rating\n";
            cout << "8. View Packages by Season\n";
            cout << "9. View Own Bookings\n";
            cout << "0. Logout\n";
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice)
            {
            case 1:
                travelBooking.addBooking();
                break;
            case 2:
                travelBooking.updateBooking();
                break;
            case 3:
                travelBooking.cancelBooking();
                break;
            case 4:
                travelBooking.viewBookings();
                break;
            case 5:
                travelBooking.viewPackagesByRate();
                break;
            case 6:
                travelBooking.viewPackagesByNumberOfPeople();
                break;
            case 7:
                travelBooking.viewPackagesByRating();
                break;
            case 8:
                travelBooking.viewPackagesBySeason();
                break;
            case 9:
                travelBooking.viewOwnBookings(to_string(cid));
                break;
            case 0:
                return;
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
            break;
        case 2: // Hotel Booking
            cout << "\n Your Cid : " << cid << endl;
            cout << "\n--- User Menu ---\n";
            cout << "1. Add Booking\n";
            cout << "2. Update Booking\n";
            cout << "3. Cancel Booking\n";
            cout << "4. View All Bookings\n";
            cout << "5. View Packages by Rate\n";
            cout << "6. View Packages by Number of People\n";
            cout << "7. View Packages by Rating\n";
            cout << "8. View Packages by Season\n";
            cout << "9. View Own Bookings\n";
            cout << "0. Logout\n";
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice)
            {
            case 1:
                hotelBooking.addBooking();
                break;
            case 2:
                hotelBooking.updateBooking();
                break;
            case 3:
                hotelBooking.cancelBooking();
                break;
            case 4:
                hotelBooking.viewBookings();
                break;
            case 5:
                hotelBooking.viewPackagesByRate();
                break;
            case 6:
                hotelBooking.viewPackagesByNumberOfPeople();
                break;
            case 7:
                hotelBooking.viewPackagesByRating();
                break;
            case 8:
                hotelBooking.viewPackagesBySeason();
                break;
            case 9:
                hotelBooking.viewOwnBookings(to_string(cid));
                break;
            case 0:
                return;
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
            break;
        case 3: // Local Travel Booking
            cout << "\n Your Cid : " << cid << endl;
            cout << "\n--- User Menu ---\n";
            cout << "1. Add Booking\n";
            cout << "2. Update Booking\n";
            cout << "3. Cancel Booking\n";
            cout << "4. View All Bookings\n";
            cout << "5. View Own Bookings\n";
            cout << "6. View Packages by Rate\n";
            cout << "0. Logout\n";
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice)
            {
            case 1:
                localTravelBooking.addBooking();
                break;
            case 2:
                localTravelBooking.updateBooking();
                break;
            case 3:
                localTravelBooking.cancelBooking();
                break;
            case 4:
                localTravelBooking.viewBookings();
                break;
            case 5:
                localTravelBooking.viewOwnBookings(to_string(cid));
                break;
            case 6: // Add this case for viewing packages by rate
                localTravelBooking.viewPackagesByRate();
                break;
            case 0:
                return;
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
            break;
        case 4: // Guide
            // Add guide functionality here
            userguide();
            break;
        case 5: // Travel Planner
            // Add travel planner functionality here
            planner();
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 0);
}

void agentMenu(TravelBooking &travelBooking, HotelBooking &hotelBooking, LocalTravelBooking &localTravelBooking, int aid)
{
    int bookingType;
agentmain:
    cout << "\n--- Agent Menu ---\n";
    cout << "1. Manage Travel Agent Files\n";
    cout << "2. Manage Hotel Agent Files\n";
    cout << "3. Manage Local Travel Agent Files\n";
    cout << "4.Manage guide \n";
    cout << "Enter your choice: ";
    cin >> bookingType;

    int choice;
    do
    {

        switch (bookingType)
        {
        case 1: // Travel Agent Files
            cout << "\n--- Agent File Management ---\n";
            cout << "1. View Agent File\n";
            cout << "2. Add Agent Data\n";
            cout << "3. Update Agent Data\n";
            cout << "4. Remove Agent Data\n";
            cout << "5. View Bookings\n";
            cout << "0. Back to Main Menu\n";
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice)
            {
            case 1:
                travelBooking.viewAgentFile("travelagency.txt");
                break;
            case 2:
                travelBooking.addAgentData();
                break;
            case 3:
                travelBooking.updateAgentData();
                break;
            case 4:
                travelBooking.removeAgentData();
                break;
            case 5:
                travelBooking.viewBookings();
                break;
            case 0:
                cout << "Returning to Main Menu...\n";
                goto agentmain;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
            }
            break;
        case 2: // Hotel Agent Files
            cout << "\n--- Agent File Management ---\n";
            cout << "1. View Agent File\n";
            cout << "2. Add Agent Data\n";
            cout << "3. Update Agent Data\n";
            cout << "4. Remove Agent Data\n";
            cout << "5. View Bookings\n";
            cout << "0. Back to Main Menu\n";
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice)
            {
            case 1:
                hotelBooking.viewAgentFile("hotelagency.txt");
                break;
            case 2:
                hotelBooking.addAgentData();
                break;
            case 3:
                hotelBooking.updateAgentData();
                break;
            case 4:
                hotelBooking.removeAgentData();
                break;
            case 5:
                hotelBooking.viewBookings();
                break;
            case 0:
                cout << "Returning to Main Menu...\n";
                goto agentmain;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
            }
            break;
        case 3: // Local Travel Agent Files
            cout << "\n--- Agent File Management ---\n";
            cout << "1. View Agent File\n";
            cout << "2. Add Agent Data\n";
            cout << "3. Update Agent Data\n";
            cout << "4. Remove Agent Data\n";
            cout << "5. View Bookings\n";
            cout << "0. Back to Main Menu\n";
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice)
            {
            case 1:
                localTravelBooking.viewAgentFile("localtravelagency.txt");
                break;
            case 2:
                localTravelBooking.addAgentData();
                break;
            case 3:
                localTravelBooking.updateAgentData();
                break;
            case 4:
                localTravelBooking.removeAgentData();
                break;
            case 5:
                localTravelBooking.viewBookings();
                break;
            case 0:
                cout << "Returning to Main Menu...\n";
                goto agentmain;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
            }
            break;
        case 4: // Manage guide
        {
            while (administratorguide())
            {
            };
        }
        default:
            cout << "Invalid booking type. Returning to main menu...\n";
            return;
        }
    } while (choice != 0);
}

// Agent Class
class Agent
{
public:
    string file;
    unordered_map<int, vector<string>> agents;

    Agent(const string &fileName);
    void loadAgents();
    vector<string> getAgentDetails(int agentID);
    int login(const string &name, const string &password);
    int signUp(const string &name, const string &password);
    int getNextAgentID();
    void updateAgentFile();
    bool forgotPassword(const string &name, const string &newPassword);
};

Agent::Agent(const string &fileName) : file(fileName)
{
    loadAgents();
}

void Agent::loadAgents()
{
    ifstream inFile(file);
    if (!inFile)
    {
        cout << "Error opening agent file: " << file << "\n";
        return;
    }

    string line;
    getline(inFile, line); // Skip the header line
    while (getline(inFile, line))
    {
        vector<string> agentDetails;
        stringstream ss(line);
        string item;
        int agentID;

        try
        {
            getline(ss, item, ',');
            agentID = stoi(item);
            while (getline(ss, item, ','))
            {
                agentDetails.push_back(item);
            }

            agents[agentID] = agentDetails;
        }
        catch (const std::invalid_argument &e)
        {
            cout << "Error parsing agent data: " << line << "\n";
        }
    }
    inFile.close();
}

vector<string> Agent::getAgentDetails(int agentID)
{
    if (agents.find(agentID) != agents.end())
    {
        return agents[agentID];
    }
    else
    {
        return {};
    }
}

int Agent::login(const string &name, const string &password)
{
    for (const auto &agent : agents)
    {
        if (agent.second[0] == name)
        {
            if (agent.second[1] == password)
            {
                cout << "Do you want to change password? Enter [Y/N]: ";
                char c;
                cin >> c;
                if (c == 'Y' || c == 'y')
                {
                    string newPassword;
                    cout << "Enter new password: ";
                    char ch;
                    while (true)
                    {
                        ch = _getch(); // Read a single character without echoing it to the console

                        if (ch == '\r')
                        {
                            break;
                        }

                        newPassword += ch; // Append character to input string
                        std::cout << '*';  // Display an asterisk
                    }
                    cout << '\n';
                    agents[agent.first][1] = newPassword;
                    updateAgentFile();
                    cout << "Password changed successfully...\n";
                }
                return agent.first; // Return agent ID
            }
            else
            {
                cout << "Invalid password...\n";
                cout << "Do you want to reset your password? Enter [Y/N]: ";
                char c;
                cin >> c;
                if (c == 'Y' || c == 'y')
                {
                    string newPassword;
                    cout << "Enter new password: ";
                    char ch;
                    while (true)
                    {
                        ch = _getch(); // Read a single character without echoing it to the console

                        if (ch == '\r')
                        {
                            break;
                        }

                        newPassword += ch; // Append character to input string
                        std::cout << '*';  // Display an asterisk
                    }
                    cout << '\n';
                    agents[agent.first][1] = newPassword;
                    updateAgentFile();
                    cout << "Password reset successfully. You can now log in with your new password.\n";
                    return agent.first; // Return agent ID
                }
                return -1; // Indicate invalid password
            }
        }
    }
    cout << "Invalid name...\n";
    return -1; // Indicate invalid name
}

int Agent::signUp(const string &name, const string &password)
{
    int newAgentID = getNextAgentID();
    agents[newAgentID] = {name, password};

    ofstream outFile(file, ios::app);
    if (outFile)
    {
        outFile << newAgentID << "," << name << "," << password << "\n";
        outFile.close();
        return newAgentID; // Return new agent ID
    }
    else
    {
        cout << "Error opening agent file for writing: " << file << "\n";
        return -1; // Indicate error in file writing
    }
}

void Agent::updateAgentFile()
{
    ofstream outFile(file, ios::trunc);
    if (outFile)
    {
        outFile << "AgentID,AgentName,Password\n"; // Write header
        for (const auto &agent : agents)
        {
            outFile << agent.first << "," << agent.second[0] << "," << agent.second[1] << "\n";
        }
        outFile.close();
    }
    else
    {
        cout << "Error opening agent file for writing: " << file << "\n";
    }
}

int Agent::getNextAgentID()
{
    int maxID = 0;
    for (const auto &agent : agents)
    {
        if (agent.first > maxID)
        {
            maxID = agent.first;
        }
    }
    return maxID + 1;
}

bool Agent::forgotPassword(const string &name, const string &newPassword)
{
    for (auto &agent : agents)
    {
        if (agent.second[0] == name)
        {
            agent.second[1] = newPassword;
            updateAgentFile();
            return true;
        }
    }
    return false;
}

int main()
{
    string travelBookingUserFile = "travelBookings.txt";
    string travelBookingAgencyFile = "travelagency.txt";
    string hotelBookingUserFile = "hotelBookings.txt";
    string hotelBookingAgencyFile = "hotelagency.txt";
    string localTravelBookingUserFile = "localTravelBookings.txt";
    string localTravelBookingAgencyFile = "localtravelagency.txt";
    string recommendationFile = "reccm.txt";
    string customerFile = "customer.txt";
    string agentFile = "agentfile.txt";

    Customer customerData(customerFile);
    Agent agentData(agentFile);

    TravelBooking travelBooking(travelBookingUserFile, travelBookingAgencyFile, customerData);
    HotelBooking hotelBooking(hotelBookingUserFile, hotelBookingAgencyFile, customerData);
    LocalTravelBooking localTravelBooking(localTravelBookingUserFile, localTravelBookingAgencyFile, customerData);
    RecommendationSystem recommendationSystem;
    GraphCreator graphCreator;

    int role;
    cout << "Login as:\n";
    cout << "1. User\n";
    cout << "2. Agent\n";
    cout << "Enter your choice: ";
    cin >> role;

    if (role == 1)
    {
        while (true)
        {
            Customer customer("customer.txt");
            int choice;
            cout << "1. Login\n2. Sign Up\nEnter your choice: ";
            cin >> choice;

            if (choice == 1)
            {
            loginchoice1:
                string name, password;
                cout << "Enter your name: ";
                cin >> name;
                cout << "Enter your password: ";
                char ch;
                while (true)
                {
                    ch = _getch(); // Read a single character without echoing it to the console
                    if (ch == '\r')
                    {
                        break;
                    }

                    password += ch;   // Append character to input string
                    std::cout << '*'; // Display an asterisk
                }
                cout << '\n';
                int cid = customer.login(name, password);
                if (cid > 0)
                {
                    cout << "Login successful.\n";
                    userMenu(travelBooking, hotelBooking, localTravelBooking, recommendationSystem, graphCreator, cid);
                }
                else
                {
                    goto loginchoice1;
                }
            }
            else if (choice == 2)
            {
                string name, password;
                cout << "Enter your name: ";
                cin >> name;
                cout << "Enter your password: ";
                char ch;
                while (true)
                {
                    ch = _getch(); // Read a single character without echoing it to the console

                    if (ch == '\r')
                    {
                        break;
                    }

                    password += ch;   // Append character to input string
                    std::cout << '*'; // Display an asterisk
                }
                cout << '\n';

                int cid = customer.signUp(name, password);
                if (cid != -1)
                {
                    cout << "Sign up successful. Your Customer ID is: " << cid << ". You can now log in.\n";
                }
            }
            else
            {
                cout << "Invalid choice.\n";
            }
        }
    }
    else if (role == 2)
    {
        while (true)
        {
            Agent agent("agentfile.txt");
            int choice;
            cout << "1. Login\n2. Sign Up\nEnter your choice: ";
            cin >> choice;

            if (choice == 1)
            {
            loginchoice2:
                string name, password;
                cout << "Enter your name: ";
                cin >> name;
                cout << "Enter your password: ";
                char ch;
                while (true)
                {
                    ch = _getch(); // Read a single character without echoing it to the console

                    if (ch == '\r')
                    {
                        break;
                    }

                    password += ch;   // Append character to input string
                    std::cout << '*'; // Display an asterisk
                }
                cout << '\n';
                int aid = agent.login(name, password);
                if (aid > 0)
                {
                    cout << "Login successful.\n";
                    agentMenu(travelBooking, hotelBooking, localTravelBooking, aid);
                }
                else
                {
                    goto loginchoice2;
                }
            }
            else if (choice == 2)
            {
                string name, password;
                cout << "Enter your name: ";
                cin >> name;
                cout << "Enter your password: ";
                char ch;
                while (true)
                {
                    ch = _getch(); // Read a single character without echoing it to the console

                    if (ch == '\r')
                    {
                        break;
                    }

                    password += ch;   // Append character to input string
                    std::cout << '*'; // Display an asterisk
                }
                cout << '\n';

                int aid = agent.signUp(name, password);
                if (aid != -1)
                {
                    cout << "Sign up successful. Your Agent ID is: " << aid << ". You can now log in.\n";
                }
            }
            else
            {
                cout << "Invalid choice.\n";
            }
        }
    }
    else
    {
        cout << "Invalid role.\n";
    }

    return 0;
}