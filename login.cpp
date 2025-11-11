#include "login.h"
#include "userprofile.h"
#include "dietoption.h"
#include "logFood.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
using namespace std;
// function for registration 
void registerUser() {
    string username, password, gender;
    int age;
    double weight, height;
    double bmi, bmr;

    cout << "=== \033[36mUSER REGISTRATION\033[0m ===\n";
    
    cin.ignore();						// clear leftover
    cout << "Enter username: ";
    getline(cin, username);				// get username with whitespaces
    
    cout << "Enter password: ";
    cin >> password;

    cout << "\n--- PROFILE INFORMATION ---\n";
    
    while(true){
	    cout << "Enter age: ";
		cin >> age;
	if (cin.fail() || age <= 0) {			// check if the input is valid or not less than 0
            cin.clear();					// reset error state
            cin.ignore(10000, '\n');		// clear invalid input
            cout << "\033[31mInvalid input. Please enter a valid number for age.\033[0m\n";
        } else break;
    }
    
    while(true){
	    cout << "Enter weight (kg): ";
	    cin >> weight;
    if (cin.fail() || weight <= 0) {		// check if the input is valid or not less than 0
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\033[31mInvalid input. Please enter a valid number for weight.\033[0m\n";
        } else break;
    }
    
    while (true) {
        cout << "Enter height (cm): ";
        cin >> height;
        if (cin.fail() || height <= 0) {		// check if the input is valid or not less than 0
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\033[31mInvalid input. Please enter a valid number for height.\033[0m\n";
        } else break;
    }

    do {
        cout << "Enter gender (male/female): ";
        cin >> gender;
        for (auto &c : gender) c = tolower(c);				// convert uppercase into lowercase
        if (gender != "male" && gender != "female") {
            cout << "\033[31mInvalid input. Please type 'male' or 'female'.\033[0m\n";
        }
    } while (gender != "male" && gender != "female");

    double height_m = height / 100.0;
    bmi = weight / (height_m * height_m);

    string bmiStatus;
    if (bmi < 18.5)
        bmiStatus = "Underweight";
    else if (bmi < 24.9)
        bmiStatus = "Normal weight";
    else if (bmi < 29.9)
        bmiStatus = "Overweight";
    else
        bmiStatus = "Obese";

    if (gender == "male")
        bmr = 88.36 + (13.4 * weight) + (4.8 * height) - (5.7 * age);
    else
        bmr = 447.6 + (9.2 * weight) + (3.1 * height) - (4.3 * age);
	// save user data file
    ofstream file("users.txt", ios::app);										// write in the user.txt file
    if (file.is_open()) {														// check if the file is open
        file << username << " " << password << " " << age << " "
             << weight << " " << height << " " << gender << " "
             << fixed << setprecision(2) << bmi << " " << bmr << endl;
        file.close();															// close userfile
    }
	//create a personal txt file containing this informations
    createUserProfileFile(username, password, age, weight, height, gender, bmi, bmiStatus, bmr);

	system("cls");
    cout << "\n=== \033[36mREGISTRATION COMPLETE\033[0m ===\n";
    cout << "Username: " << username << endl;
    cout << "BMI: " << fixed << setprecision(2) << bmi << " (" << bmiStatus << ")\n";
    cout << "BMR: " << fixed << setprecision(2) << bmr << " kcal/day\n";
    cout << "\033[32mRegistration successful!\033[0m\n\n";
}
// function for getting password
void forgotPassword(){
	string username, password;
    string fileUser, filePass, gender;
    int age;
    double weight, height, bmi, bmr;
    bool found = false;
    
    cout << "=== \033[36mFORGOT PASSWORD\033[0m ===\n";
    cout << "Enter your username: ";
    cin >> username;
    
	ifstream file("users.txt");											// read the user txt file
    if (!file) {														// if txt file does not exist
        cout << "\033[31mNo registered users found.\033[0m\n";
        return;															// return to main menu
    }
	// search for the password in the txt file
    while (file >> fileUser >> filePass >> age >> weight >> height >> gender >> bmi >> bmr) {
        if (fileUser == username) {
            cout << "\033[32mYour password is: " << filePass << "\033[0m\n";		// display the user password
            found = true;
            break;	
        }
    }

    file.close();														// close file

    if (!found) {														// check if the username is in the database
        cout << "\033[31mUsername not found.\033[0m\n";
    }
}


// function for logging in
void loginUser() {
    string username, password;
    string fileUser, filePass, gender;
    int age;
    double weight, height, bmi, bmr;
    bool found = false;

    cout << "=== \033[36mUSER LOGIN\033[0m ===\n";
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    ifstream file("users.txt");												// open/read the user txt file
    if (!file) {															// if file does not exist
        cout << "\033[31mNo registered users found. Please register first.\033[0m\n\n";
        return;																// return to main menu
    }
	// check if the username and the password match in the txt file
    while (file >> fileUser >> filePass >> age >> weight >> height >> gender >> bmi >> bmr) {
        if (fileUser == username && filePass == password) {
            found = true;
            break;	
        }
    }
    file.close();															// close file
	
    if (found) {															// if the found condition is true display the main menu
    	
    	system("cls");
        cout << "\033[32m\nLogin successful! Welcome, " << username << "!\033[0m\n";
        int option;
        do {
            cout << "\n=== \033[36mMAIN MENU\033[0m ===\n";
            cout << "1. User Profile\n";
            cout << "2. Set Diet Goal\n";
            cout << "3. Food List\n";
            cout << "4. Food Log\n";
            cout << "5. Report\n";
            cout << "6. Logout\n";
            cout << "Enter choice: ";
            cin >> option;
			// handles the user inputs
            switch (option) {
                case 1:
                    displayUserProfile(username);			
                    break;
                case 2:
                    setDietGoal(username);					
                    break;
                case 3:
                    addFoodToList(username);
                    break;
                case 4:
                    logFood(username);
                    break;
                case 5:
                    viewReport(username, bmr);
                    break;
                case 6:
                    cout << "Logging out...\n";
                    break;
                default:
                    cout << "\033[31mInvalid choice.\033[0m\n";
            }
        } while (option != 6);

    } else {
        cout << "\033[31mInvalid username or password.\033[0m\n\n";
    }
}

