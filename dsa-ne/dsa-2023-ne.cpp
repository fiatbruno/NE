#include <iostream>
#include <sstream>
#include <fstream>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <regex>

using namespace std;

// Function to check if a new line already exists in a file
bool isDuplicate(const string& filename, const string& newLine) {
    ifstream infile(filename);
    string line;

    while (getline(infile, line)) {
        stringstream ss(line);
        string id;
        getline(ss, id, ',');

        if (id == newLine) {
            infile.close();
            return true;
        }
    }

    infile.close();
    return false;
}


// Function to add an item to the file
void addItem(string filename, string item_id, string item_name, string item_quantity, string item_registration_date){
    string delimiter = ",";
    ofstream outfile;
    regex dateRegex("\\d{4}-\\d{2}-\\d{2}");
    if (!regex_match(item_registration_date, dateRegex))
    {
        cout << "Error: Invalid registration date format. Correct format: YYYY-MM-DD." << endl;
        return;
    }
    outfile.open(filename, ios::app);
    outfile << item_id << delimiter << item_name << delimiter << item_quantity << delimiter << item_registration_date << endl;
    cout << "Item " << item_name << " with ID: " << item_id << " is successfully added !"<<endl;

    outfile.close();
}

// Function to delete an item from the file
void deleteItem(string filename, const string& item_id){
    ifstream infile(filename);
    string line;
    stringstream ss;

    while (getline(infile, line)) {
        string id;
        stringstream ss_line(line);
        getline(ss_line, id, ',');

        if (id != item_id) {
            ss << line << endl;
        }
    }

    infile.close();

    ofstream outfile(filename);
    outfile << ss.str();
    cout << "Item with ID: " << item_id << " is successfully deleted!" << endl;
    outfile.close();
}

// Function to compare items by name
bool compareByName(const vector<string>& item1, const vector<string>& item2) {
    return item1[1] < item2[1]; // Compare based on the second value (item name)
}

// Function to compare items by ID
bool compareById(const vector<string>& item1, const vector<string>& item2) {
    return stoi(item1[0]) < stoi(item2[0]); // Compare based on the first value (ID)
}

// Function to compare items by quantity
bool compareByQuantity(const vector<string>& item1, const vector<string>& item2) {
    return stoi(item1[3]) < stoi(item2[3]); // Compare based on the third value (ID)
}

// Function to list items based on the provided comparison function
void listItems(string compare_function){
    ifstream infile("items.csv");
    vector<vector<string>> items;
    string line;

    while (getline(infile, line)) {
        stringstream ss(line);
        string itemData;
        vector<string> item;

        while (getline(ss, itemData, ',')) {
            item.push_back(itemData);
        }

        items.push_back(item);
    }

    infile.close();

    // Sorting the items vector based on item names, ID, or quantity 
    if(compare_function.compare("id") == 0){
        sort(items.begin(), items.end(), compareById);
    } else if(compare_function.compare("name") == 0)  {
        sort(items.begin(), items.end(), compareByName);
    }else if(compare_function.compare("quantity") == 0)  {
        sort(items.begin(), items.end(), compareByQuantity);
    }

    // Displaying the sorted items
    for (const auto& item : items) {
        cout << "Item ID: " << item[0] << "\tItem Name: " << item[1]
                  << "\tQuantity: " << item[2] << "\tReg Date: " << item[3] << endl;
    }
}

// Function to display the help menu
int help(){
    cout << "Need any help? Type 'help' then press Enter key."<<endl;

    string command;

    do {
        cout << "Console> ";
        getline(cin, command);

        for_each(command.begin(), command.end(), [](char& c) {
            c = ::tolower(c);
        });

        system("clear");

        string commandCopy = command;
        string space_delimiter = " ";

        vector<string> splitedCommands{};

        size_t pos = 0;

        while ((pos = commandCopy.find(space_delimiter)) != string::npos){
            splitedCommands.push_back(commandCopy.substr(0, pos));
            commandCopy.erase(0, pos + space_delimiter.length());
        }
        splitedCommands.push_back(commandCopy);


        if (splitedCommands.at(0).compare("itemadd") == 0)
        {
            if(isDuplicate("items.csv", splitedCommands.at(1))) {
                cout << "Item with ID: " << splitedCommands.at(1) << " already exits" << endl;
                continue;
            }
            addItem("items.csv", splitedCommands.at(1), splitedCommands.at(2), splitedCommands.at(3), splitedCommands.at(4));
        }
        else if (splitedCommands.at(0).compare("itemdelete") == 0) {
            if(!isDuplicate("items.csv", splitedCommands.at(1))) {
                cout << "Item with ID: " << splitedCommands.at(1) << " does not exist" << endl;
                continue;
            }
            deleteItem("items.csv", splitedCommands.at(1));
        }
         else if (splitedCommands.at(0).compare("itemupdate") == 0) {
            if(!isDuplicate("items.csv", splitedCommands.at(1))) {
                cout << "Item with ID: " << splitedCommands.at(1) << " does not exist" << endl;
                continue;
            }
            string item_id = splitedCommands.at(1);
            string item_name = splitedCommands.at(2);
            string item_quantity = splitedCommands.at(3);
            string item_registration_date = splitedCommands.at(4);

            deleteItem("items.csv", item_id);
            addItem("items.csv", item_id, item_name, item_quantity, item_registration_date);

            cout << "Item with ID: " << item_id << " is successfully updated!" << endl;
        } 
        else if (splitedCommands.at(0).compare("itemslist") == 0){
            listItems("name");
        }
        else if (splitedCommands.at(0).compare("help") == 0){
            cout << "--------------------------------------------------------------------------------------------------"<<endl;
            cout << "*                            Commands     Syntaxes                                               *"<<endl;
            cout << "--------------------------------------------------------------------------------------------------"<<endl;
            cout << "itemadd <item_id> <item_name> <quantity> <registration_date>           :Add a new item"<<endl;
            cout << "itemdelete <item_id>                                                   :Delete an item"<<endl;
            cout << "itemupdate <item_id> <item_name> <quantity> <registration_date>        :Update an item"<<endl;
            cout << "itemslist <compare_function>                                           :Lists all items"<<endl;
            cout << "help                                                                   :Prints user manual"<<endl;
            cout << "exit                                                                   :Exit the program"<<endl;
            cout << "--------------------------------------------------------------------------------------------------"<<endl;
        }
        else if (splitedCommands.at(0).compare("exit") == 0){
            return 0;
        }
        else{
            cout << "Invalid command !"<<endl;
        }
    } while(command.compare("exit") != 0);
    
    return 0;
}


int main (){
    cout << "--------------------------------------------------------------------------------------------------"<<endl;
    cout << "*                                                                                                *"<<endl;
    cout << "*                     Welcome to RCA Invetory Management System!                                 *"<<endl;
    cout << "*                                                                                                *"<<endl;
    cout << "--------------------------------------------------------------------------------------------------"<<endl;
    cout << "*                                                                                                *"<<endl;
    cout << "*     It is developed by Fiat Bruno as practical evaluation for the end of Year 3.               *"<<endl;
    cout << "*                                                                                                *"<<endl;
    cout << "*                                                                                                *"<<endl;
    cout << "--------------------------------------------------------------------------------------------------"<<endl;
    cout << "*                                                                                                *"<<endl;
    cout << "=================================================================================================="<<endl;
    cout << "*                                                                                                *"<<endl;
    cout << "Starting Time  : Thu Jun 21 09:30:00 CAT 2023                                                     "<<endl;
    cout << "Ending Time    : Thu Jun 21 13:30:00 CAT 2023                                                     "<<endl;
    cout << "*                                                                                                *"<<endl;
    cout << "=================================================================================================="<<endl;


    

    help();
 
    cout << "=================================================================================================="<<endl;
    cout << "*                                          BYE  "<<endl;
    cout << "=================================================================================================="<<endl;
    return 0;
}