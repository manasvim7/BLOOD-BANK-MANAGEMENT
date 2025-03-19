#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

#define MAX_DONORS 100
#define MAX_PATIENTS 100

class BloodBank;
void main_menu();
//Person
class Person {
protected:
    string name, contact;
    int age;
public:
    Person(string n, int a, string c) : name(n), age(a), contact(c) {}
    virtual void display() = 0;
    friend class BloodBank;
};

//Donor: hierarchial inheritance
class Donor : public Person {
private:
    string bloodGroup;
    int bloodBags;
public:
    Donor() : Person("", 0, ""), bloodGroup(""), bloodBags(0) {}
    Donor(string n, string bg, int a, string c, int bags) : Person(n, a, c), bloodGroup(bg), bloodBags(bags) {}
    
    void display() override {
        cout << left << setw(20) << name << setw(15) << bloodGroup << setw(10) << age << setw(15) << contact << setw(10) << bloodBags << "\n";
    }
    
    void saveToFile() {
        ofstream file("donor2.txt", ios::app);
        file << name << "," << bloodGroup << "," << age << "," << contact << "," << bloodBags << "\n";
        file.close();
    }
    
    string getBloodGroup() { return bloodGroup; }
    int getBloodBags() { return bloodBags; }
    void reduceBags(int num) { bloodBags -= num; }
};

//Patient: hierarchial inheritance
class Patient : public Person {
private:
    string bloodGroup;
public:
    Patient() : Person("", 0, ""), bloodGroup("") {}
    Patient(string n, string bg, int a, string c) : Person(n, a, c), bloodGroup(bg) {}

    void display() override {
        cout << left << setw(20) << name << setw(15) << bloodGroup << setw(10) << age << setw(15) << contact << "\n";
    }
    
    void saveToFile() {
        ofstream file("patient_list.txt", ios::app);
        file << name << "," << bloodGroup << "," << age << "," << contact << "\n";
        file.close();
    }
};

//BloodBank
class BloodBank {
private:
    Donor donors[MAX_DONORS];
    Patient patients[MAX_PATIENTS];
    int donorCount;
    int patientCount;
    int inventory[8];
    string bloodGroups[8] = {"O-", "O+", "A-", "A+", "B-", "B+", "AB-", "AB+"};

public:
    BloodBank() : donorCount(0), patientCount(0) {
        for (int &i : inventory) i = 0;
        cout << "Blood Bank system initialized.\n";
        loadDonorsFromFile();
        loadPatientsFromFile();
    }
    
    //Add
    void addDonor(string name, string bg, int age, string contact, int bags) {
        if (donorCount >= MAX_DONORS) {
            cout << "Donor list is full!\n";
            return;
        }
        donors[donorCount++] = Donor(name, bg, age, contact, bags);
        updateInventory(bg, bags);
        donors[donorCount - 1].saveToFile();
    }

    void addPatient(string name, string bg, int age, string contact) {
        if (patientCount >= MAX_PATIENTS) {
            cout << "Patient list is full!\n";
            return;
        }
        patients[patientCount++] = Patient(name, bg, age, contact);
        patients[patientCount - 1].saveToFile();
    }
    
    //update inventory
    void updateInventory(string bg, int bags) {
        for (int i = 0; i < 8; i++) {
            if (bloodGroups[i] == bg) {
                inventory[i] += bags;
                return;
            }
        }
    }

	//load
    void loadDonorsFromFile() {
        ifstream file("donor2.txt");
        if (!file) return;
        string name, bloodGroup, contact;
        int age, bags;
        while (file >> ws, getline(file, name, ',') && getline(file, bloodGroup, ',') && file >> age && file.ignore() && getline(file, contact, ',') && file >> bags) {
            donors[donorCount++] = Donor(name, bloodGroup, age, contact, bags);
            updateInventory(bloodGroup, bags);
        }
        file.close();
    }

    void loadPatientsFromFile() {
        ifstream file("patient_list.txt");
        if (!file) return;
        string name, bloodGroup, contact;
        int age;
        while (file >> ws, getline(file, name, ',') && getline(file, bloodGroup, ',') && file >> age && file.ignore() && getline(file, contact, ',')) {
            patients[patientCount++] = Patient(name, bloodGroup, age, contact);
        }
        file.close();
    }

	//show
    void showDonors() {
        system("cls");
        system("color 0E");
        cout << "\n========================================\n";
        cout << "          LIST OF DONORS";
        cout << "\n========================================\n";
        cout << left << setw(20) << "Name" << setw(15) << "Blood Group" << setw(10) << "Age" << setw(15) << "Contact" << setw(10) << "Bags" << "\n";
        cout << "-------------------------------------------------------------\n";
        for (int i = 0; i < donorCount; i++) {
            donors[i].display();
        }
    }

    void showPatients() {
        system("cls");
        system("color 0E");
        cout << "\n========================================\n";
        cout << "          LIST OF PATIENTS";
        cout << "\n========================================\n";
        cout << left << setw(20) << "Name" << setw(15) << "Blood Group" << setw(10) << "Age" << setw(15) << "Contact" << "\n";
        cout << "-------------------------------------------------------------\n";
        for (int i = 0; i < patientCount; i++) {
            patients[i].display();
        }
    }

    void showInventory() {
        system("cls");
        system("color 0C");
        cout << "\n========================================\n";
        cout << "          BLOOD INVENTORY";
        cout << "\n========================================\n";
        for (int i = 0; i < 8; i++) {
            cout << bloodGroups[i] << ": " << inventory[i] << " bags\n";
        }
    }
	
	//blood buying
    void buyBlood(string bg, string patientName, string patientContact) {
        for (int i = 0; i < 8; i++) {
            if (bloodGroups[i] == bg) {
                if (inventory[i] > 0) {
                    system("color 0A");
                    cout << "Blood provided successfully to patient: " << patientName << "\n";
                    system("pause");
                    inventory[i]--;
                    showInventory();
                    return;
                } else {
                    system("color 0C");
                    cout << "Sorry, no blood available for blood group " << bg << "\n";
                    system("pause");
                    return;
                }
            }
        }
        system("color 0C");
        cout << "Invalid blood group.\n";
    }
};

//admin login
bool adminLogin() {
    string temp, pwd;
    cout << "Enter Admin Password: ";
    cin >> temp;
    ifstream Ad;
    Ad.open("adminpwd.txt", ios::in);
    Ad >> pwd;
    Ad.close();
    if (temp == pwd) {
        system("color 0A");
        cout << "Verified!" << endl;
        return true;
    }
    return false;
}

//donation checking
int canDonate(int tattoo,int allergy,float bmi){
        if (tattoo) {
            cout << "Donor cannot donate due to tattoo.\n";
            return 0;
        }
		if (allergy) {
            cout << "Donor cannot donate due to allergy.\n";
            return 0;
        }
        if (bmi < 18.5 || bmi > 24.9) {
            cout << "BMI is not in a healthy range. Donor cannot donate.\n";
            return 0;
        }

        return 1;
}

//donor menu
void donor_menu() {
    system("color 0A");
    BloodBank bank;
    int choice;
    do {
        system("cls");
        cout << "1. Add Donor\n2. Show Donors\n3. Back to main menu\n4. Exit\nEnter choice: ";
        cin >> choice;
        cin.ignore();
        switch (choice) {
        case 1: {
            string name, bloodGroup, contact;
            int age, bags;
            float height, weight;
            int tattoo, allergy;
            cout << "Enter weight(in kgs): ";
            cin >> weight;
            cout << "Enter height(in m): ";
            cin >> height;
            cout << "Do you have any tattoo(1-yes/0-no): ";
            cin >> tattoo;
            cout << "Do you have any allergy(1-yes/0-no): ";
            cin >> allergy;
            float bmi = weight / (height * height);
            int f = canDonate(tattoo, allergy, bmi);
            if (!f) 
			{
                system("pause");
                main_menu();
                return;
            }
            cout << "Enter Name: ";
            cin>>name;
            cout << "Enter Blood Group: ";
            cin >> bloodGroup;
            cout << "Enter Age: ";
            cin >> age;
            cout << "Enter Contact: ";
            cin.ignore();
            getline(cin, contact);
            cout << "Enter number of blood bags: ";
            cin >> bags;
            bank.addDonor(name, bloodGroup, age, contact, bags);
            break;
        }
        case 2:
            bank.showDonors();
            break;
        case 3: {
            main_menu();
            return;
        }
        break;
        case 4:
            exit(0);
        }
        system("pause");
    } while (choice <= 3);
}

//patient menu
void patient_menu() {
    system("color 0A");
    BloodBank bank;
    int choice,age;
    string bloodGroup, name, contact;
    do {
        system("cls");
        cout << "1. Enter patient info\n2. Show patients\n3. Back to main menu\n4. Exit\nEnter choice: ";
        cin >> choice;
        switch (choice) {
        case 1:
            cout << "Enter required Blood Group: ";
            cin >> bloodGroup;
            bank.showInventory();
            cout << "Enter Name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter Contact: ";
            getline(cin, contact);
            cout << "Enter Age: ";
            cin>>age;
            bank.addPatient(name, bloodGroup, age, contact); // Add patient without age as it's not necessary in this context
            bank.buyBlood(bloodGroup, name, contact);
            break;
        case 2: bank.showPatients(); 
        break;
        case 3: {
            main_menu();
            return;
        }
        break;
        case 4:
            exit(0);
        }
        system("pause");
    } while (choice <= 2);
}

//main menu
void main_menu() {
    system("cls");
    system("color 0B");
    int userType;
    cout << "Blood Bank system initialized.\n";
    do {
        cout << "For a:\n1. Donor\n2. Patient\n3. Exit\nEnter choice: ";
        cin >> userType;
        cin.ignore();
        if (userType == 1)
            donor_menu();
        else if (userType == 2)
            patient_menu();
        else
            exit(0);
    } while (userType >= 3);
    system("pause");
    return;
}

//main
int main() {
    system("color 0B");
    cout << "\n\t\t\t************************************************************************" << endl;
    cout << setw(80) << "**********Welcome to our Blood Bank!*********" << endl;
    cout << setw(90) << "**********Be a Part of Something Bigger. Donate Blood**********" << endl;
    cout << "\t\t\t************************************************************************" << endl << endl;
    if (!adminLogin()) {
        system("color 0C");
        cout << setw(80) << "Invalid Admin Credentials! Exiting...\n";
        return 0;
    }
    system("pause");
    main_menu();
    return 0;
}