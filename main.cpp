#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "login.h"
#include "addFood.h"
#include "logFood.h"
#include "viewReport.h"
using namespace std;

int main() {
    int choice;

    do {
        system("cls"); // THIS CLEARS THE SCREEN EACH TIME IT RUNS
        
        cout << "\033[36m";
        cout << R"(
 _   _       _        _     _____               _    
| \ | |_   _| |_ _ __(_)   |_   _| __ __ _  ___| | __
|  \| | | | | __| '__| |_____| || '__/ _` |/ __| |/ /
| |\  | |_| | |_| |  | |_____| || | | (_| | (__|   < 
|_| \_|\__,_|\__|_|  |_|     |_||_|  \__,_|\___|_|\_\
		)";
		cout << "\033[0m\n";
        
        
        
        cout << "=================== \033[36mLOGIN & REGISTRATION MENU\033[0m ===================\n\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Forgot Password\n";
        cout << "4. Exit\n\n";
        cout << "Enter choice: ";
        cin >> choice;
        
        // THIS HANDLES ERRORS ON INPUT SUCH AS IF YOU ARAE GOING TO INPUT Q on 1-3 IT WILL JUST RETURN 
        if (cin.fail()){
			cin.clear();												// remove the error state
        	cin.ignore(10000, '\n');									// ignore all the leftover input in buffer
        	system("cls");
        	cout << "\033[31mInvalid input! Please enter a number between 1 and 4.\033[0m\n";
        	cout << "\nPress Enter to continue...";
        	cin.ignore();												// wait for enter key
        	system("cls");
        	continue;													// go back to the start of the loop
	}

        system("cls"); // Clear again before showing new section

        switch (choice) {
            case 1:			// registration option
                cout << "=================== \033[36mREGISTER USER\033[0m ===================\n\n";
                registerUser();
                break;

            case 2:			// login option
                cout << "=================== \033[36mLOGIN\033[0m ===================\n\n";
                loginUser();
                break;
			
			case 3:
				system("cls");
				forgotPassword();
				break;
			
            case 4:
                cout << "Exiting program...\n";
                break;

            default:
                cout << "\033[31mInvalid choice. Try again.\033[0m\n";
        }

        if (choice != 4) {
            cout << "\n" << "Press Enter to return to the main menu...";
            cin.ignore();
            cin.get();
        }

    } while (choice != 4);		// repeat until user doesn't choose exit

    return 0;
}


