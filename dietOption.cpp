#include "dietoption.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cstdlib> // for system()
using namespace std;


void setDietGoal(const string& username) {
    system("cls"); //  Clear the console first

    int choice;
    cout << "=================== \033[36mSET DIET GOAL\033[0m ===================\n\n";

    cout << "1. Gain Weight (Calorie Surplus)\n";
    cout << "2. Maintain Weight\n";
    cout << "3. Lose Weight (Calorie Deficit)\n\n";
    cout << "0. Go Back\n";
    cout << "Enter your choice: ";
    cin >> choice;
    
    if (cin.fail()){
		cin.clear();
        cin.ignore(10000, '\n');
        system("cls");
        cout << "\033[31mInvalid input! Please enter a number between 1-3 or 0 to go back.\033[0m\n";
        cout << "\nPress Enter to continue...";
        cin.ignore();
        system("cls");
        return setDietGoal(username);
	}

    
    if (choice==0){
		system("cls");
		return;    	
	}

    string goal;
    switch (choice) {
        case 1: goal = "gain"; break;
        case 2: goal = "maintain"; break;
        case 3: goal = "lose"; break;
        default:
            cout << "\033[1;31m" << "Invalid choice.\n" << "\033[0m";
            cout << "\nPress Enter to continue...";
        	cin.ignore();
        	cin.get();
        	system("cls");
        	return setDietGoal(username);
    }

    system("cls"); // ? Clear again for clean output

    ofstream file(username + "_diet.txt");
    if (file.is_open()) {
        file << goal;
        file.close();
        cout << "\033[1;32m"; // Green text
        cout << "Diet goal set to '" << goal << "' successfully!\n";
        cout << "\033[0m";
    } else {
        cout << "\033[1;31m" << "Error saving diet goal.\n" << "\033[0m";
        return;
    }

    // Retrieve original BMR from users.txt
    ifstream users("users.txt");
    if (!users.is_open()) {
        cout << "\033[1;31m" << "Error: users.txt not found.\n" << "\033[0m";
        return;
    }

    string u, p, gender;
    int age;
    double weight, height, bmi, baseBMR = 0;
    while (users >> u >> p >> age >> weight >> height >> gender >> bmi >> baseBMR) {
        if (u == username) break;
    }
    users.close();

    double adjusted = getAdjustedBMR(username, baseBMR);

    string profileFile = username + "_profile.txt";
    ifstream in(profileFile);
    if (!in.is_open()) {
        cout << "\033[1;31m" << "Error: Cannot find profile file to update BMR.\n" << "\033[0m";
        return;
    }

    stringstream buffer;
    string line;
    bool updated = false;
    while (getline(in, line)) {
        if (line.find("BMR (Calories Needed):") != string::npos) {
            ostringstream newLine;
            newLine << "BMR (Calories Needed): " << fixed << setprecision(2)
                    << adjusted << " kcal/day";
            line = newLine.str();
            updated = true;
        }
        buffer << line << "\n";
    }
    in.close();

    ofstream out(profileFile);
    out << buffer.str();
    out.close();

    cout << "\n";
    if (updated)
        cout << "\033[1;33m" << "Profile updated with adjusted BMR (" << adjusted << " kcal/day)!\n" << "\033[0m";
    else
        cout << "\033[1;31m" << "Warning: BMR line not found in profile file.\n" << "\033[0m";
        
    cout << "\n\n";
	cout << "\033[1;37m" << "Press Enter to return to the main menu...";
    cout << "\033[0m";
    cin.ignore();
    cin.get(); // Wait for Enter
    
    system("cls");
}



double getAdjustedBMR(const string& username, double bmr) {
    ifstream file(username + "_diet.txt");
    if (!file.is_open()) return bmr;

    string goal;
    getline(file, goal);
    file.close();

    if (goal == "gain") return bmr + 300;
    if (goal == "lose") return bmr - 300;
    return bmr;
}

