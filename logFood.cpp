#ifndef LOGFOOD_H
#define LOGFOOD_H
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
using namespace std;
// function to log user food intake
void logFood(const string& username) {
    system("cls");

    ifstream foodList(username + "_foodlist.txt");                    // open/read user txt file 
    if (!foodList.is_open()) {                                        // if file doesn't exist
        cout << "\033[31mNo food list found. Please add some foods first.\033[0m\n";
        cout << "\nPress Enter to go back...";
        cin.ignore();
        cin.get();
        system("cls");
        return;                                                        // return to main menu
    }

    cout << "=== \033[36mYOUR FOOD LOG\033[0m ===\n\n";
    cout << left << setw(5) << "No."
         << setw(25) << "Food Name"
         << right << setw(12) << "Calories"
         << setw(12) << "Protein(g)"
         << setw(12) << "Carbs(g)" << "\n";
    cout << string(65, '-') << "\n";
    // array to store food data temporarily
    string line;
    string names[100];
    double cals[100], prots[100], carbsArr[100];
    int index = 0;
    
    while (getline(foodList, line)) {                            // read each line from the food list file
        if (line.empty()) continue;                              // skip empty lines
        
        size_t p1 = line.find(",");                             // read up to the 1st comma
        size_t p2 = line.find(",", p1 + 1);                     // read up to the 2nd comma
        size_t p3 = line.find(",", p2 + 1);                     // read up to the 3rd comma
        if (p3 == string::npos) continue;                        

        string name = line.substr(0, p1);                           // extract name of the food
        double cal = stod(line.substr(p1 + 1, p2 - p1 - 1));        // extract calorie of the food
        double prot = stod(line.substr(p2 + 1, p3 - p2 - 1));        // extract protein of the food
        double carb = stod(line.substr(p3 + 1));                    // extract carbs of the food
        // store the extracted value in the array
        names[index + 1] = name;
        cals[index + 1] = cal;
        prots[index + 1] = prot;
        carbsArr[index + 1] = carb;

        cout << left << setw(5) << (++index)
             << setw(25) << name
             << right << setw(12) << fixed << setprecision(1) << cal
             << setw(12) << fixed << setprecision(1) << prot
             << setw(12) << fixed << setprecision(1) << carb
             << "\n";
    }
    foodList.close();                                            // close file

    if (index == 0) {                                           // check if the food list is empty
        cout << "\033[31mYour food list is empty.\033[0m\n";
        cout << "\nPress Enter to go back...";
        cin.ignore();
        cin.get();
        system("cls");
        return;                                                 // return to main menu
    }

    int choice;
    if (index == 1) {
        cout << "\nEnter which food you ate (1) or (0) to go back: ";
        while (!(cin >> choice)) {                               // handles error input
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\033[31mInvalid input! Please enter a number: \033[0m";
        }
    } else {
        cout << "\nEnter which food you ate (1-" << index << "), or (0) to go back: ";
        while (!(cin >> choice)) {                                // handles error input
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\033[31mInvalid input! Please enter a number (1-" << index << "), or (0) to go back): \033[0m";
        }
    }

    if (choice == 0) {
        system("cls");
        return;                                                    // return to main menu
    }

    if (choice < 1 || choice > index) {                            // validates if the chosen number is in the index
        cout << "\033[31mInvalid choice.\033[0m\n";
        cout << "\nPress Enter to go back...";
        cin.ignore();
        cin.get();
        system("cls");
        return;
    }

    int quantity;
    cout << "How many servings did you eat? ";
    while (!(cin >> quantity)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "\033[31mInvalid input! Please enter a valid number of servings: \033[0m";
    }

    if (quantity <= 0) {
        cout << "\033[31mInvalid quantity.\033[0m\n";
        cout << "\nPress Enter to go back...";
        cin.ignore();
        cin.get();
        system("cls");
        return;
    }

    // Multiply the nutrients based on the number of quantity eaten
    double totalCals = cals[choice] * quantity;
    double totalProt = prots[choice] * quantity;
    double totalCarb = carbsArr[choice] * quantity;

    // Get current date
    time_t now = time(0);
    tm *ltm = localtime(&now);
    string date = to_string(1900 + ltm->tm_year) + "-" +
                  to_string(1 + ltm->tm_mon) + "-" +
                  to_string(ltm->tm_mday);

    // Open the file and save the food entry in the user food log file
    ofstream logFile(username + "_foodlog.txt", ios::app);
    logFile << date << "," << names[choice] << "," << totalCals << ","
            << totalProt << "," << totalCarb << "," << quantity << endl;
    logFile.close();    // close the file

    cout << "\n\033[32mLogged successfully!\033[0m\n";
    cout << left << setw(15) << "Food:" << names[choice] << "\n";
    cout << left << setw(15) << "Quantity:" << quantity << "x\n";
    cout << left << setw(15) << "Calories:" << fixed << setprecision(1) << totalCals << " kcal\n";
    cout << left << setw(15) << "Protein:" << fixed << setprecision(1) << totalProt << " g\n";
    cout << left << setw(15) << "Carbs:" << fixed << setprecision(1) << totalCarb << " g\n";

    cout << "\nPress Enter to return to the menu...";
    cin.ignore();
    cin.get();
    system("cls");
}

#endif

