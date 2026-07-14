#include <iostream>
#include <conio.h>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <limits>
#include <cctype>

#include "UI.h"

using namespace std;

// ==========================================
// 1. MEMBERSHIP CLASS
// ==========================================
class Membership {
private:
    int memberId;
    string ownerName;
    string ownerNumber;
    bool isVip;
    int validDaysLeft;

public:
    Membership(int id = 0, string name = "", string num = "", bool vip = false, int days = 0)
        : memberId(id), ownerName(name), ownerNumber(num), isVip(vip), validDaysLeft(days) {}

    int getId() { return memberId; }
    string getName() { return ownerName; }
    string getNumber() { return ownerNumber; }
    bool checkVip() { return isVip; }
    int getDays() { return validDaysLeft; }
    bool isValid() { return validDaysLeft > 0; }

    double getDiscountPercentage() {
        if (validDaysLeft > 60) return 0.20;
        else if (validDaysLeft > 30) return 0.15;
        else if (validDaysLeft > 0) return 0.10;
        return 0.0;
    }
};

// ==========================================
// 2. VEHICLE CLASSES (Abstraction & Inheritance)
// ==========================================
class Vehicle {
protected:
    string plateNumber;
    int expectedDuration;
    int memberId;

public:
    Vehicle(string plate, int duration, int mId)
        : plateNumber(plate), expectedDuration(duration), memberId(mId) {}

    virtual ~Vehicle() {}

    string getPlate() { return plateNumber; }
    int getExpectedDuration() { return expectedDuration; }
    int getMemberId() { return memberId; }

    virtual string getType() = 0;
    virtual double calculateFee(int actualHrs, double penaltyRate) = 0;
};

class Car : public Vehicle {
private:
    double baseRate = 10.0;
public:
    Car(string plate, int duration, int mId) : Vehicle(plate, duration, mId) {}
    string getType() override { return "Car"; }
    double calculateFee(int actualHrs, double penaltyRate) override {
        double fee = baseRate * actualHrs;
        if (actualHrs > expectedDuration) fee += (actualHrs - expectedDuration) * penaltyRate;
        return fee;
    }
};

class Bike : public Vehicle {
private:
    double baseRate = 5.0;
public:
    Bike(string plate, int duration, int mId) : Vehicle(plate, duration, mId) {}
    string getType() override { return "Bike"; }
    double calculateFee(int actualHrs, double penaltyRate) override {
        double fee = baseRate * actualHrs;
        if (actualHrs > expectedDuration) fee += (actualHrs - expectedDuration) * penaltyRate;
        return fee;
    }
};

class Truck : public Vehicle {
private:
    double baseRate = 15.0;
public:
    Truck(string plate, int duration, int mId) : Vehicle(plate, duration, mId) {}
    string getType() override { return "Truck"; }
    double calculateFee(int actualHrs, double penaltyRate) override {
        double fee = baseRate * actualHrs;
        if (actualHrs > expectedDuration) fee += (actualHrs - expectedDuration) * penaltyRate;
        return fee;
    }
};

// ==========================================
// 3. PARKING SLOT CLASS
// ==========================================
class ParkingSlot {
private:
    int slotId;
    bool isVipSlot;
    Vehicle* parkedVehicle;

public:
    ParkingSlot(int id, bool vip) : slotId(id), isVipSlot(vip), parkedVehicle(nullptr) {}

    int getId() { return slotId; }
    bool isVip() { return isVipSlot; }
    bool getStatus() { return parkedVehicle != nullptr; }
    Vehicle* getVehicle() { return parkedVehicle; }

    void assignVehicle(Vehicle* v) { parkedVehicle = v; }
    void freeSlot() {
        delete parkedVehicle;
        parkedVehicle = nullptr;
    }
};

// ==========================================
// 4. PARKING SYSTEM CLASS
// ==========================================
class ParkingSystem {
private:
    vector<ParkingSlot> slots;
    vector<Membership> members;

    const double VIP_SURCHARGE = 20.0;
    const double LATE_NIGHT_FEE = 15.0;
    const double EXTRA_TIME_PENALTY = 10.0;

    double dailyRevenue, weeklyRevenue;
    int weeklyCars, weeklyBikes, weeklyTrucks, weeklyVips;
    const string ADMIN_PASS = "admin123";

    void loadStats() {
        ifstream file("stats.txt");
        if (file.is_open()) {
            file >> dailyRevenue >> weeklyRevenue >> weeklyCars >> weeklyBikes >> weeklyTrucks >> weeklyVips;
            file.close();
        } else {
            weeklyRevenue = 0.0; dailyRevenue = 0.0; weeklyCars = 0; weeklyBikes = 0; weeklyTrucks = 0; weeklyVips = 0;
        }
    }

    void saveStats() {
        ofstream file("stats.txt");

        file << dailyRevenue << " " << weeklyRevenue << " " << weeklyCars << " " << weeklyBikes << " " << weeklyTrucks << " " << weeklyVips;
        file.close();
    }

    void loadMembers() {
        ifstream file("members.txt");
        if (file.is_open()) {
            int id, days; string name, num; bool vip;
            while (file >> id >> name >> num >> vip >> days) {
                members.push_back(Membership(id, name, num, vip, days));
            }
            file.close();
        }
    }

    void saveMembers() {
        ofstream file("members.txt");
        for (size_t i = 0; i < members.size(); i++) {
            file << members[i].getId()
			     << " " << members[i].getName()
				 << " " << members[i].getNumber()
				 << " " << members[i].checkVip()
				 << " " << members[i].getDays() << "\n";
        }
        file.close();
    }

    void saveParkedVehicles() {
        ofstream file("parked.txt");
        for (size_t i = 0; i < slots.size(); i++) {
            if (slots[i].getStatus()) {
                Vehicle* v = slots[i].getVehicle();
                file << slots[i].getId() << " " << v->getPlate() << " "
                     << v->getType() << " " << v->getExpectedDuration() << " " << v->getMemberId() << "\n";
            }
        }
        file.close();
    }

    void loadParkedVehicles() {
        ifstream file("parked.txt");
        if (file.is_open()) {
            int slotId, expectedHrs, memId; string plate, type;
            while (file >> slotId >> plate >> type >> expectedHrs >> memId) {
                Vehicle* newVehicle = nullptr;
                if (type == "Car" || type == "car") newVehicle = new Car(plate, expectedHrs, memId);
                else if (type == "Bike" || type == "bike") newVehicle = new Bike(plate, expectedHrs, memId);
                else if (type == "Truck" || type == "truck") newVehicle = new Truck(plate, expectedHrs, memId);

                for (size_t i = 0; i < slots.size(); i++) {
                    if (slots[i].getId() == slotId && newVehicle != nullptr) {
                        slots[i].assignVehicle(newVehicle);
                        break;
                    }
                }
            }
            file.close();
        }
    }

    bool isLateNight() {
        time_t now = time(0); tm *ltm = localtime(&now);
        return (ltm->tm_hour >= 22 || ltm->tm_hour < 6);
    }

    Membership* checkMembership(int id) {
        for (size_t i = 0; i < members.size(); i++) {
            if (members[i].getId() == id) return &members[i];
        }
        return nullptr;
    }

public:
    ParkingSystem() {
        dailyRevenue = 0.0;
        for (int i = 1; i <= 8; i++) slots.push_back(ParkingSlot(i, false));
        for (int i = 9; i <= 10; i++) slots.push_back(ParkingSlot(i, true));
        loadStats(); loadMembers(); loadParkedVehicles();

	}
	bool systemLogin() {
	    UI::clearScreen();
        while (true) {
            UI::clearScreen();
            cout << "\n\n\n\n" << color::b_white;
            cout << "\t=========================================================\n";
            cout << color::b_green << "\t                  MAIN MENU                   \n" << color::reset;
            cout << "\t=========================================================\n\n"<< color::b_yellow;
            cout << "\t  [1] Continue as Standard User\n";
            cout << "\t  [2] Login as ADMIN\n";
            cout << "\t  [3] Exit System\n";
            cout << color::b_white << "\t=========================================================\n\n";

            int choice = UI::getIntInput("\t  Enter choice (1-3): ");

            if (choice == 1) {
                UI::showLoading(" Loading User Interface", 1000);
                return false;
            }
            else if (choice == 2) {
                string pass = UI::getPasswordInput("\n\t  Enter Admin Password: ");
                if (pass == ADMIN_PASS) {
                    UI::showLoading(" Authenticating and loading Admin Dashboard", 1000);
                    return true;
                } else {
                    cout << color::b_red << "\n\t  Incorrect Password! Access Denied.\n" << color::reset;
                    cout << color::b_white << "\t  Press Enter to try again...";
                    cin.get();
                }
            }
            else if (choice == 3) {
                UI::showEndingScreen();
                exit(0);
            }
            else {
                cout << color::b_red << "\t  Invalid Option. Press Enter to try again..." << color::reset;
                cin.get();
            }
        }
    }

    void showFeeStructure() {
        UI::clearScreen();
        UI::openSubWindow("FEE STRUCTURE & DISCOUNTS");
        cout << color::b_yellow << " [1] Bikes           : $5.00 / hour\n";
        cout << " [2] Cars            : $10.00 / hour\n";
        cout << " [3] Trucks          : $15.00 / hour\n";
        cout << color::b_black << "-----------------------------------------------\n";
        cout << color::b_yellow << "  VIP Surcharge   : +$" << VIP_SURCHARGE << " (Priority Slot)\n";
        cout << "  Late Night Fee  : +$" << LATE_NIGHT_FEE << " (10 PM to 6 AM)\n";
        cout << "  Late Penalty    : +$" << EXTRA_TIME_PENALTY << " / extra hour\n";
        cout << color::b_black << "-----------------------------------------------\n";
        cout << color::b_blue<< "            MEMBERSHIP DISCOUNTS               \n";
        cout << color::b_yellow << " >  60 Days Validity : 20% OFF Final Bill\n";
        cout << " >  30 Days Validity : 15% OFF Final Bill\n";
        cout << " 1- 30 Days Validity : 10% OFF Final Bill\n";
    }


    void registerMember() {
        UI::clearScreen();
        UI::openSubWindow("REGISTER NEW MEMBERSHIP");

        int id = UI::getIntInput(" Enter new Member ID (e.g., 000): ");
        if (checkMembership(id) != nullptr) {
            cout << color::b_red << "\n This ID already exists. Try another.\n" << color::reset; return;
        }

        string name = UI::getStringInput(" Enter Owner's Name (First & Last): ");
        string num;
        while (true)
		 {
            num = UI::getStringInput("\n Enter Phone Number (11 digits, e.g., 03XXXXXXXXX): ");
            if (UI::isValidPhone(num))
            {
                break;
            }
            else
            {
                cout << color::b_red << "\n Invalid format!  \n" << color::reset;
            }
        }

        int vipChoice = UI::getIntInput(" Select Type (1 for VIP, 0 for Normal): ");
        int days = UI::getIntInput(" Enter Validity (in days, e.g., 30): ");

        UI::showLoading(" Encrypting details and saving to database", 1500);
        members.push_back(Membership(id, name, num, (vipChoice == 1), days));
        saveMembers();
        cout << color::b_green << "\n Membership Registered Successfully for " << color::green << name << color::b_white << "!\n";
    }

    void parkVehicle() {
        UI::clearScreen();
        UI::openSubWindow("PARK A VEHICLE");

        string plate = UI::getStringInput(" Enter Plate Number: ");
        string type;

        while (true) {
            type = UI::getStringInput(" Enter Type (Car/Bike/Truck): ");
            if (type == "Car" || type == "car" || type == "Bike" || type == "bike" || type == "Truck" || type == "truck") {
                break;
            } else {
                //cout << "\n  Haha, nice try! But we do not park '" << type << "'s here.\n";
                cout << color::b_red << "  Please enter a valid type (Car/Bike/Truck).\n\n" << color::reset;
            }
        }

        int hrs = UI::getIntInput(" Enter Expected Duration (Hours): ");
        int memId = UI::getIntInput(" Enter Membership ID (0 if none): ");

        UI::showLoading(" Validating Membership and searching for slots", 1500);
        Membership* mem = nullptr;

        if (memId != 0) {
            mem = checkMembership(memId);
            if (mem == nullptr) {
                cout << color::b_red << "\n  Membership ID not found in database\n" << color::reset;
                return;
            }
            if (!mem->isValid()) {
                cout << color::b_red << "\n  Membership is expired! Parking as a standard guest.\n" << color::reset;
                mem = nullptr;
            }
        }

        Vehicle* newVehicle = nullptr;
        if (type == "Car" || type == "car")
		 newVehicle = new Car(plate, hrs, memId);
        else if (type == "Bike" || type == "bike")
		 newVehicle = new Bike(plate, hrs, memId);
        else if (type == "Truck" || type == "truck")
		newVehicle = new Truck(plate, hrs, memId);

        bool isUserVip = (mem != nullptr && mem->checkVip());

        if (isUserVip)
		{
            for (size_t i = 0; i < slots.size(); i++) {
                if (slots[i].isVip() && !slots[i].getStatus())
				{
                    slots[i].assignVehicle(newVehicle);
                    saveParkedVehicles();
                    cout << "\n Welcome back, " << mem->getName() << " Park in SPECIAL SLOT: " << slots[i].getId() << "\n";
                    return;
                }
            }
            cout << color::b_red << "\n VIP Slots are full. Searching for a normal slot...\n" << color::reset;
        }

        for (size_t i = 0; i < slots.size(); i++)
		 {
            if (!slots[i].isVip() && !slots[i].getStatus()) {
                slots[i].assignVehicle(newVehicle);
                saveParkedVehicles();
                if (mem != nullptr) cout << "\n Welcome, " << mem->getName() << ". Park in NORMAL SLOT: " << slots[i].getId() << "\n";
                else cout << "\n Guest Vehicle parked in NORMAL SLOT: " << slots[i].getId() << "\n";
                return;
            }
        }
        delete newVehicle;
        cout << color::b_red << "\n Parking Lot is FULL!\n" << color::reset;
    }

    void viewParkedVehicles() {
        UI::clearScreen();
        UI::openSubWindow("CURRENT PARKING LOT STATUS");
        bool foundAny = false;

        cout << color::b_blue << left << setw(10) << "Slot" << setw(15) << "Plate No." << setw(10) << "Type"
             << setw(12) << "Duration" << "Status\n";
        cout << color::b_black << "------------------------------------------------------------\n";

        for (size_t i = 0; i < slots.size(); i++)
		 {
            if (slots[i].getStatus()) {
                foundAny = true;
                Vehicle* v = slots[i].getVehicle();

                string slotName = to_string(slots[i].getId());
                if (slots[i].isVip()) slotName += " (VIP)";

                string memStatus = "Guest";
                if (v->getMemberId() != 0) memStatus = "Member (ID: " + to_string(v->getMemberId()) + ")";

                string durationStr = to_string(v->getExpectedDuration()) + " hrs";

                cout << color::b_yellow << left << setw(10) << slotName << setw(15) << v->getPlate()
                     << setw(10) << v->getType() << setw(12) << durationStr << memStatus << "\n";
            }
        }

        if (!foundAny) cout << color::b_red << "\n The parking lot is completely empty right now.\n" << color::reset;
        else cout << color::b_black << "--------------------------------------------------------------\n";
    }

    void exitVehicle() {
        UI::openSubWindow("VEHICLE CHECKOUT");

        int slotId = UI::getIntInput(" Enter Slot ID to free: ");
        int actualHrsSpent = UI::getIntInput(" Actual hours spent? ");

        UI::showLoading(" Generating bill and contacting database", 2000);

        for (size_t i = 0; i < slots.size(); i++) {
            if (slots[i].getId() == slotId && slots[i].getStatus()) {
                Vehicle* v = slots[i].getVehicle();

                double subTotal = v->calculateFee(actualHrsSpent, EXTRA_TIME_PENALTY);
                double discount = 0.0, finalTotal = 0.0;

                if (v->getType() == "Car") weeklyCars++;
                else if (v->getType() == "Truck") weeklyTrucks++;
                else weeklyBikes++;

                if (slots[i].isVip()) { subTotal += VIP_SURCHARGE; weeklyVips++; }
                if (isLateNight()) subTotal += LATE_NIGHT_FEE;

                Membership* mem = checkMembership(v->getMemberId());
                if (mem != nullptr && mem->isValid()) {
                    discount = subTotal * mem->getDiscountPercentage();
                }

                finalTotal = subTotal - discount;
                dailyRevenue += finalTotal; weeklyRevenue += finalTotal;

                string savedPlate = v->getPlate();
                string savedType = v->getType();
                string savedName = (mem != nullptr) ? mem->getName() : "Guest";

                slots[i].freeSlot();
                saveStats(); saveParkedVehicles();

                cout << color::b_white << "\n========== CHECKOUT RECEIPT ==========\n";
                cout << color::b_cyan << " Owner Name    : " << color::b_yellow << savedName << "\n";
                cout << color::b_cyan << " Plate Number  : " << color::b_yellow << savedPlate << "\n";
                cout << color::b_cyan << " Vehicle Type  : " << color::b_yellow << savedType << "\n";
                cout << color::b_cyan << " Slot Freed    : " << color::b_yellow << slotId << "\n";
                cout << color::b_black << "--------------------------------------\n";
                cout << color::b_cyan << " Subtotal      : $" << color::b_yellow << fixed << setprecision(2) << subTotal << "\n";
                if (discount > 0) cout << " Mem. Discount : -$" << color::b_yellow << fixed << setprecision(2) << discount << "\n";
                cout << color::b_black << "--------------------------------------\n";
                cout << color::b_cyan << " FINAL TOTAL   : $" << color::b_yellow << fixed << setprecision(2) << finalTotal << "\n";
                cout << color::b_white << "======================================\n";

                return;
            }
        }
        cout << color::b_red << "\n Invalid Slot ID or Slot is already empty!\n" << color::reset;
    }

    void viewAllMembers() {
        UI::clearScreen();
        UI::openSubWindow("SECURE MEMBER DATABASE");
            if (members.empty()) { cout << color::b_red << "\n No members registered.\n" << color::reset; return; }
            cout << color::b_cyan << "\n" << left << setw(10) << "ID" << setw(20) << "Name" << setw(15) << "Phone" << setw(10) << "Type" << "Validity\n";
            cout << color::b_black << "----------------------------------------------------------------------\n";
            for (size_t i = 0; i < members.size(); i++) {
                string type = members[i].checkVip() ? "VIP" : "Normal";
                cout << color::b_yellow << left << setw(10) << members[i].getId() << setw(20) << members[i].getName()
                     << setw(15) << members[i].getNumber() << setw(10) << type << members[i].getDays() << " days\n";
            }
        }



    void adminPanel() {
        UI::clearScreen();
        UI::openSubWindow("WEEKLY ADMIN ANALYTICS");
            cout << color::b_cyan << "\nTotal Revenue Collected : $" << color::b_blue << fixed << setprecision(2) << weeklyRevenue << "\n";
            cout << color::b_cyan << " Total Cars Parked       : " << color::b_yellow << weeklyCars << "\n";
            cout << color::b_cyan << " Total Bikes Parked      : " << color::b_yellow << weeklyBikes << "\n";
            cout << color::b_cyan << " Total Trucks Parked     : " << color::b_yellow << weeklyTrucks << "\n";
            cout << color::b_cyan << " Total VIP Cars Parked   : " << color::b_yellow << weeklyVips << "\n";
        }


    void clearAllData() {
        UI::clearScreen();
        UI::openSubWindow("SYSTEM FACTORY RESET");
            cout << color::red << "\n\t  < WARNING! > \n";
            cout << "\n\t  Are you absolutely sure you want to permanently delete ALL data? (Y/N): "<< color::reset;
            char confirm = UI::getCharInput("");

            if ( toupper(confirm)=='Y') {
                UI::showLoading("\tErasing database", 1500);
                dailyRevenue = 0.0; weeklyRevenue = 0.0;
                weeklyCars = 0; weeklyBikes = 0; weeklyTrucks = 0; weeklyVips = 0;
                members.clear();

                for(size_t i = 0; i < slots.size(); i++) { slots[i].freeSlot(); }
                slots.clear();

                for (int i = 1; i <= 8; i++) slots.push_back(ParkingSlot(i, false));
                for (int i = 9; i <= 10; i++) slots.push_back(ParkingSlot(i, true));

                saveStats(); saveMembers(); saveParkedVehicles();
                cout << color::b_green << "\n\t  FACTORY RESET COMPLETE.\n" << color::reset;
            } else { cout << color::b_green << "\n\t  Factory Reset Cancelled.\n" << color::reset; }
        }

};


// ==========================================
// 5. MAIN FUNCTION
// ==========================================
int main()
 {
    UI::showWelcomeScreen();

    ParkingSystem myParking;
    int choice;

    while (true) {

        while (true) {
            UI::printHeader();

            if (myParking.systemLogin()) {
                UI::clearScreen();
                cout << color::b_white << "\t=========================================================\n";
                cout << color::b_cyan << "\t   >>> LOGGED IN AS: ADMINISTRATOR <<<\n";
                cout << color::b_white << "\t=========================================================\n";
                cout << color::b_yellow;
                cout << "\n\t\t1 > View Members Database\n";
                cout << "\n\t\t2 > View Weekly Stats\n";
                cout << "\n\t\t3 > Factory Reset / Clear Data\n";
                cout << "\n\t\t4 > Return to Main Menu\n";
                cout << color::b_white << "\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

                choice = UI::getIntInput("\tEnter choice (1-4): ");

                if      (choice == 1) myParking.viewAllMembers();
                else if (choice == 2) myParking.adminPanel();
                else if (choice == 3) myParking.clearAllData();
                else if (choice == 4) {
                    UI::showLoading(" \tReturning to Main Menu", 1000);
                    break;
                }
                else { cout << color::b_red << "\tInvalid Option!\n"; }

            }
			 else {
                UI::clearScreen();
                cout << color::b_white << "\t=========================================================\n";
                cout << color::b_cyan << "\t   >>> LOGGED IN AS: STANDARD USER <<<\n";
                cout << color::b_white << "\t=========================================================\n";
                cout << color::b_yellow;
                cout << "\n\t\t1 > Park a Vehicle\n";
                cout << "\n\t\t2 > Checkout / Remove Vehicle\n";
                cout << "\n\t\t3 > View Currently Parked Vehicles\n";
                cout << "\n\t\t4 > Register New Membership\n";
                cout << "\n\t\t5 > View Fee Structure & Discounts\n";
                cout << "\n\t\t6 > Return to Main Menu\n";
                cout << color::b_white << "\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

                choice = UI::getIntInput("Enter choice (1-6): ");

                if      (choice == 1) myParking.parkVehicle();
                else if (choice == 2) myParking.exitVehicle();
                else if (choice == 3) myParking.viewParkedVehicles();
                else if (choice == 4) myParking.registerMember();
                else if (choice == 5) myParking.showFeeStructure();
                else if (choice == 6) {
                    UI::showLoading(" Returning to Main Menu", 1000);
                    break;
                }
                else { cout << color::red << "Invalid Option!\n" << color::reset; }
            }

            cout << "\n\t\t Press Enter to return to Dashboard...";
            cin.get();
        }
    }
    return 0;
 }
