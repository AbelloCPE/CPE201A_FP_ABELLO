#include "addFood.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdlib>
using namespace std;

void displayFoodList(const string& filename) {
    ifstream file(filename);
    cout << left << setw(5) << "No."
         << setw(25) << "Food Name"
         << right << setw(12) << "Calories"
         << setw(12) << "Protein(g)"
         << setw(12) << "Carbs(g)" << "\n";
    cout << string(65, '-') << "\n";

    if (!file.is_open()) {
        cout << "(No foods added yet)\n";
        return;
    }

    string line;
    int index = 0;
    while (getline(file, line)) {
        size_t p1 = line.find(","), p2 = line.find(",", p1 + 1), p3 = line.find(",", p2 + 1);
        if (p3 == string::npos) continue;
        string name = line.substr(0, p1);
        double cal = stod(line.substr(p1 + 1, p2 - p1 - 1));
        double prot = stod(line.substr(p2 + 1, p3 - p2 - 1));
        double carb = stod(line.substr(p3 + 1));
        cout << left << setw(5) << (++index)
             << setw(25) << name
             << right << setw(12) << fixed << setprecision(1) << cal
             << setw(12) << fixed << setprecision(1) << prot
             << setw(12) << fixed << setprecision(1) << carb << "\n";
    }
    if (index == 0) cout << "(Your list is empty)\n";
}

vector<string> loadFileLines(const string& filename) {
    ifstream file(filename);
    vector<string> lines;
    string line;
    while (getline(file, line)) if (!line.empty()) lines.push_back(line);
    return lines;
}

void saveLines(const string& filename, const vector<string>& lines) {
    ofstream out(filename, ios::trunc);
    for (const auto& line : lines) out << line << "\n";
}

void waitForEnter() {
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
    system("cls");
}

void addFoodToList(const string& username) {
    while (true) {
        system("cls");
        cout << "=== \033[36mYOUR FOOD LIST\033[0m ===\n";
        displayFoodList(username + "_foodlist.txt");

        cout << "\n=== \033[36mFOOD LIST MENU\033[0m ===\n";
        cout << "1. Choose from predefined foods\n";
        cout << "2. Add custom food manually\n";
        cout << "3. Mix existing foods to create a new one\n";
        cout << "4. Remove food from list\n";
        cout << "0. Go Back\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            system("cls");
            cout << "Invalid input! Please enter a number between (0-4).\n";
            cin.get();
            system("cls");
            continue;
        }

        if (choice < 0 || choice > 4) {
            system("cls");
            cout << "Invalid choice! Please enter a number between (0-4).\n";
            waitForEnter();
            continue;
        }

        if (choice == 0) {
            system("cls");
            return;
        }

        string foodName;
        double calories = 0, protein = 0, carbs = 0;

        // Remove Food
        if (choice == 4) {
            system("cls");
            auto foods = loadFileLines(username + "_foodlist.txt");
            if (foods.empty()) {
                cout << "Your food list is empty.\n";
                waitForEnter();
                continue;
            }

            cout << "=== \033[36mREMOVE FOOD FROM LIST\033[0m ===\n";
            displayFoodList(username + "_foodlist.txt");
            cout << "\nEnter number of food to remove (0 to cancel): ";
            int removeIndex;
            cin >> removeIndex;

            if (cin.fail() || removeIndex < 0 || removeIndex > (int)foods.size()) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "\033[31mInvalid input.\033[0m\n";
                waitForEnter();
                continue;
            }

            if (removeIndex == 0) {
                system("cls");
                continue;
            }

            foods.erase(foods.begin() + removeIndex - 1);
            saveLines(username + "_foodlist.txt", foods);
            cout << "\n\033[32mFood successfully removed!\033[0m\n";
            waitForEnter();
            continue;
        }

        // Predefined Foods
        if (choice == 1) {
            ifstream predef("predefined_foods.txt");
            if (!predef) {
                cout << "No predefined food file found.\n";
                waitForEnter();
                continue;
            }

            vector<string> foods;
            vector<double> cals, prots, carbsArr;
            string line;
            while (getline(predef, line)) {
                size_t p1 = line.find(","), p2 = line.find(",", p1 + 1), p3 = line.find(",", p2 + 1);
                if (p3 == string::npos) continue;
                foods.push_back(line.substr(0, p1));
                cals.push_back(stod(line.substr(p1 + 1, p2 - p1 - 1)));
                prots.push_back(stod(line.substr(p2 + 1, p3 - p2 - 1)));
                carbsArr.push_back(stod(line.substr(p3 + 1)));
            }
            predef.close();

            if (foods.empty()) {
                cout << "Predefined list is empty.\n";
                waitForEnter();
                continue;
            }

            int page = 0, perPage = 10, totalPages = (foods.size() + perPage - 1) / perPage;
            string action;

            while (true) {
                system("cls");
                cout << "\n=== \033[36mPREDEFINED FOOD LIST (Page " << page + 1 << " of " << totalPages << ")\033[0m ===\n\n";
                cout << left << setw(5) << "No." << setw(25) << "Food Name"
                     << right << setw(12) << "Calories"
                     << setw(12) << "Protein(g)"
                     << setw(12) << "Carbs(g)" << "\n";
                cout << string(65, '-') << "\n";

                int start = page * perPage, end = min((int)foods.size(), start + perPage);
                for (int i = start; i < end; ++i)
                    cout << left << setw(5) << (i + 1) << setw(25) << foods[i]
                         << right << setw(12) << fixed << setprecision(1) << cals[i]
                         << setw(12) << prots[i] << setw(12) << carbsArr[i] << "\n";

                cout << "\nEnter number to select food, N for next, P for previous, or Q to quit: ";
                cin >> action;

                if (action == "N" || action == "n") {
                    if (page < totalPages - 1) page++;
                } else if (action == "P" || action == "p") {
                    if (page > 0) page--;
                } else if (action == "Q" || action == "q") {
                    system("cls");
                    break;
                } else {
                    try {
                        int select = stoi(action);
                        if (select >= 1 && select <= (int)foods.size()) {
                            foodName = foods[select - 1];
                            calories = cals[select - 1];
                            protein = prots[select - 1];
                            carbs = carbsArr[select - 1];
                            break;
                        } else cout << "\033[31mInvalid number.\033[0m\n";
                    } catch (...) {
                        cout << "\033[31mInvalid input.\033[0m\n";
                    }
                }
            }
        }

        // Add Custom Food
        else if (choice == 2) {
            char confirm;
            system("cls");
            cin.ignore();
            cout << "=== \033[36mCUSTOM FOOD ADD\033[0m ===\n";
            cout << "Do you want to continue adding a custom food? (Y/N): ";
            cin >> confirm;

            if (confirm == 'N' || confirm == 'n') {
                system("cls");
                continue;
            }

            cin.ignore();
            cout << "Enter food name: ";
            getline(cin, foodName);

            cout << "Calories: ";
            while (!(cin >> calories)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "\033[31mInvalid, Enter a number for calories: \033[0m";
            }

            cout << "Protein (g): ";
            while (!(cin >> protein)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "\033[31mInvalid, Enter a number for protein: \033[0m";
            }

            cout << "Carbs (g): ";
            while (!(cin >> carbs)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "\033[31mInvalid, Enter a number for carbs: \033[0m";
            }

            cin.ignore();
            system("cls");
            cout << "\nCustom food added successfully!\n";
            cout << left << setw(15) << "Food:" << foodName << "\n"
                 << setw(15) << "Calories:" << calories << " kcal\n"
                 << setw(15) << "Protein:" << protein << " g\n"
                 << setw(15) << "Carbs:" << carbs << " g\n";
        }

        // Mix Foods
        else if (choice == 3) {
            system("cls");
            auto foods = loadFileLines(username + "_foodlist.txt");
            if (foods.empty()) {
                cout << "No existing food list found. Add some foods first.\n";
                waitForEnter();
                continue;
            }

            vector<string> names;
            vector<double> cals, prots, carbsArr;
            for (const auto& line : foods) {
                size_t p1 = line.find(","), p2 = line.find(",", p1 + 1), p3 = line.find(",", p2 + 1);
                if (p3 == string::npos) continue;
                names.push_back(line.substr(0, p1));
                cals.push_back(stod(line.substr(p1 + 1, p2 - p1 - 1)));
                prots.push_back(stod(line.substr(p2 + 1, p3 - p2 - 1)));
                carbsArr.push_back(stod(line.substr(p3 + 1)));
            }

            displayFoodList(username + "_foodlist.txt");
            vector<int> selected;
            cout << "\nEnter food numbers to mix (separated by space), (0) to stop: ";

            int choiceNum;
            while (true) {
                cin >> choiceNum;
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "\033[31mInvalid input!\033[0m\n";
                    continue;
                }

                if (choiceNum == 0) 
					break;
                if (choiceNum >= 1 && choiceNum <= (int)names.size()) selected.push_back(choiceNum - 1);
                else 
					cout << "\033[31mInvalid number. Try again.\033[0m\n";
            }

            if (selected.empty()) {
                cout << "\033[31mNo foods selected.\033[0m\n";
                waitForEnter();
                continue;
            }

            for (int i : selected) { 
			calories += cals[i]; 
			protein += prots[i]; 
			carbs += carbsArr[i]; 
			}

            cin.ignore();
            cout << "\nEnter a name for your new mixed food: ";
            getline(cin, foodName);

            system("cls");
            cout << "\nNew food '" << foodName << "' created! Total:\n";
            cout << left << setw(15) << "Calories:" << calories << " kcal\n"
                 << setw(15) << "Protein:" << protein << " g\n"
                 << setw(15) << "Carbs:" << carbs << " g\n";
        }

        // Save food
        ofstream file(username + "_foodlist.txt", ios::app);
        if (file.is_open()) {
            file << foodName << "," << calories << "," << protein << "," << carbs << endl;
            file.close();
            cout << "\nFood saved successfully!\n";
        } 
		else {
            cout << "\033[31mError saving food list.\033[0m\n";
        }

        cout << "\nPress Enter to return to menu...";
        cin.get();
        system("cls");
    }
}

