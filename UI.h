#ifndef UI_H
#define UI_H

#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>

using namespace std;

namespace color{            // From https://github.com/Basit-Ahmad-GCUF/C-Utilities
	// Reset Color
	inline const std::string reset   = "\033[0m";

    // Standard Colors
    inline const std::string black   = "\033[30m";
    inline const std::string blue    = "\033[34m";
    inline const std::string green   = "\033[32m";
    inline const std::string cyan    = "\033[36m";
    inline const std::string red     = "\033[31m";
    inline const std::string magenta = "\033[35m";
    inline const std::string yellow  = "\033[33m";
    inline const std::string white   = "\033[37m";

    // Bright Colors
    inline const std::string b_black   = "\033[90m";
    inline const std::string b_blue    = "\033[94m";
    inline const std::string b_green   = "\033[92m";
    inline const std::string b_cyan    = "\033[96m";
    inline const std::string b_red     = "\033[91m";
    inline const std::string b_magenta = "\033[95m";
    inline const std::string b_yellow  = "\033[93m";
    inline const std::string b_white   = "\033[97m";

}

class UI {
public:
	static void showWelcomeScreen() {
        clearScreen();
        cout << color::b_white << "\n\n\n\n";
        cout << "\t=========================================================\n";
        cout << color::b_green << "\t  WELCOME TO MY ADVANCE SMART PARKING MANAGMENT SYSTEM   \n"<< color::reset ;
        cout << "\t=========================================================\n\n";
        cout << color::b_yellow;
        cout << "\t                   Developed By: \n";
        cout << "\t                   - 1st Person \n";
        cout << "\t                   - 2nd Person \n";
        cout << "\t                   - 3rd Person \n\n" << color::b_white;
        cout << "\t=========================================================\n";

        cout << "\n\t\t\tPress any key to start it..." << color::reset;
       _getch();
    }

    static void showEndingScreen() {
        clearScreen();
        cout << color::b_white << "\n\n\n\n";
        cout << "\t=========================================================\n";
        cout << color::b_green << "\t      THANK YOU FOR USING THE ADVANCE SMART PARKING SYSTEM       \n";
        cout << color::b_white << "\t=========================================================\n\n";
        cout << color::b_yellow;
        showLoading("               Saving all files and shutting down", 1500);

        cout << "\n\t                     System Offline.\n";
        cout << color::b_green << "\t                        Goodbye!\n\n" << color::reset;
        cout << color::b_white << "\t=========================================================\n\n" ;
        _getch();
    }
    static void clearScreen() {
        std::cout<<"\033[2J\033[H";
    }

    static void showLoading(string message, int duration_ms = 1500)
	{
        cout << "\n " << message;
        int steps = 5;
        int step_delay = duration_ms / steps;

        for (int i = 0; i < steps; ++i) {
            cout << ".";
            cout.flush();
            std::this_thread::sleep_for(std::chrono::milliseconds(step_delay));
        }
        cout << " Done\n";
    }

    static void printHeader() {
        clearScreen();
        cout << color::b_white << "\t=========================================================\n";
        cout << color::b_green << "\t         ADVANCE SMART PARKING MANAGMENT SYSTEM          \n";
        cout << color::b_white << "\t=========================================================\n";

        time_t now = time(0);
        string dt = ctime(&now);
        cout << "\t   Live Time: " << color::b_blue << dt;
        cout << color::b_white << "\t=========================================================\n";
    }

    static void openSubWindow(string title) {
        clearScreen();
        cout << color::b_white << "=========================================================\n";
        int spaces = (55 - title.length()) / 2;
        for(int i = 0; i < spaces; i++) cout << " ";
        cout << color::b_cyan << title << "\n";
        cout << color::b_white << "=========================================================\n\n";
    }

    static int getIntInput(string statement) {
        int value;
        while (true) {
            cout << statement;
            if (cin >> value)
			{
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            } else {
                cout << color::b_red << " Invalid input! Please type numbers only.\n" << color::reset;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        return value;
    }


    static string getStringInput(string statement) {
        string value;
        cout << statement;
        getline(cin, value);
        return value;
    }
    static string getPasswordInput(string statement)
    {
    	cout<<statement;
    	string password="";
    	char ch;
    	while ((ch=_getch())!='\r')
    	{
    		if(ch=='\b'|| ch==8)
    		{
    			if(password.length()>0)
				{
					password.pop_back();
					cout<<"\b \b";
				}
			}
			else
			{
				password+=ch;
				cout<<'*';
			}
		}
		cout<<"\n";
		return password;
	}

    static char getCharInput(string statement) {
        char value;
        cout << statement;
        cin >> value;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return value;
    }


    static bool isValidPhone(string phone)
	 {
        if (phone.length() != 11) return false;
        for (int i = 0; i < phone.length(); i++)
	 {
        if (!isdigit(phone[i])) return false;
    }
        if (phone[0] != '0' || phone[1] != '3') return false;

        return true;
    }
};

#endif // UI_H
