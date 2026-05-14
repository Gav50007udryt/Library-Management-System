// Name: Gavin Hatcher
// Program: Library Management System
// Purpose: The purpose of this program is to create a virtual library with all the normal library functions.
#include <iostream>
#include <string>
#include <list>
#include <fstream>

using namespace std;

//Structs and Classes
// First the date class to track loans and return dates.
struct date {
	int day;
	int month;
	int year;
	void datevalidator() {
		if (day > 31) {
			cout << "invalid date";
		}
		if (month>12){
			cout << "invalid date";
		}
		// The year not being older than 1970 is in reference to how Unix measurees time.
		if (year<1970){
			cout << "invalid date";
		}
	if (year >2099){
		cout << "invalid date";
}
	
	}
};
//The library item class serves as a base class for all the subclasses of items in library. 
class libraryItem {
protected:
	int id;
	string title;
	string author;
	bool isborrowed;
	int duedate;
public:
	// Defualt Constructor 
	libraryItem(int id, string title, string author) : id(id), title(title), author (author), isborrowed(false),duedate(0){}
	virtual~libraryItem(){}
	// These functions serve to get information about the library item.
	int getId() const {
		return id;
	}
	string getTitle() const {
		return title; 
	}
	string getAuthor() const {
		return author;
	}
	bool getStatus() const {	
		return isborrowed;
	}
	void setborrowedstatus(bool status, int date = 0) {
		isborrowed = status;
		duedate = date + setloanperiod();

	}
	virtual string gettype() const = 0;
	virtual int setloanperiod() const = 0;
	virtual double calculatefine(int returndate) {
		int date;
		date = returndate - duedate;
		return 0.00 * date;
		
	}
	// This function creates a univerisal output for seeing items in the library.
		virtual void display() const {
			cout << "[" << gettype() << "] ID: " << id << " | Title: " << title << " | Author: " << author
				<< " | Status: " << (isborrowed ? "Borrowed" : "Available");
		
		}
	
};

class book : public libraryItem {
public:
	book(int id, string t,string z) : libraryItem(id, t,z) {}
	int setloanperiod() const override {
		return 20;
	}
	string gettype() const override {
		return "Book";
	}
	// Unique fine for every class.
	virtual double calculatefine(int returndate) override {
		int date;
		date = returndate - duedate;
		return 0.75 * date;

	}
};

class magazine : public libraryItem {
public:
	magazine(int id, string t, string z) : libraryItem(id, t, z) {}
	int setloanperiod() const override {
		return 20;
	}
	string gettype() const override {
		return "Magazine";
	}
	
	double calculatefine(int returndate) override {
			int date;
			date = returndate - duedate;
			return 1.00*date;
	}
};

class cd : public libraryItem {
public:
	cd(int id, string t, string z) : libraryItem(id, t, z) {}
	int setloanperiod() const override {
		return 14;
	}
	string gettype() const override {
		return "CD";
	}
	
	virtual double calculatefine(int returndate) override {
		int date;
		date = returndate - duedate;
		return 2.00 * date;

	}
};
class DVD : public libraryItem {
public:
	DVD(int id, string t, string z) : libraryItem(id, t, z) {}
	int setloanperiod() const override {
		return 14;
	}
	string gettype() const override {
		return "DVD";
	}
	virtual double calculatefine(int returndate) override {
		int date;
		date = returndate - duedate;
		return 1.75 * date;

	}
};
class Member {
private:
	int memberId;
	//I implemented a linked list to make storing members easier
	string name;
	list<int> borroweditemmemberIDs;
	list<string> history;    

public:
	Member(int id, string n) : memberId(id), name(n) {}

	int getId() const {
		return memberId; 
	}
	string getName() const { 
		return name; 
	}


	void addLoan(int iteMemberID) {
		borroweditemmemberIDs.push_back(iteMemberID);
	}

	
	void returnLoan(int iteMemberID) {
		borroweditemmemberIDs.remove(iteMemberID);
	}

	void addHistory(string record) {
		history.push_back(record);
	}

	void viewBorrowed() {
		cout << "Items out at this time to " << name << ": ";
		if (borroweditemmemberIDs.empty()) {
			cout << "None";
		}
		else {
			for (int id : borroweditemmemberIDs) {
				cout << id << " ";
			}
		}
		cout << endl;
	}

	void viewHistory() {
		cout << "Loan History for " << name << ":" << endl;
		for (const auto& entry : history) {
			cout << "- " << entry << endl;

	

		}
	}
};
// This class is essientally the brain of the library. 
class LibrarySystem {
	list<libraryItem*> Inventory;
	list<Member> members;
	int currentDate = 0; // Simulated internal clock.

public:
	~LibrarySystem() {
		for (auto item : Inventory) delete item;
	}

	// Librarian Functions
	void addItem(int type, int id, string title,string author) {
		if (type == 1) Inventory.push_back(new book(id, title,author));
		else if (type == 2) Inventory.push_back(new magazine(id, title,author));
		else if (type == 3) Inventory.push_back(new cd(id, title,author));
		else if (type == 4) Inventory.push_back(new DVD(id, title,author)); 
		cout << "Item added successfully"<<endl;
	}

	void removeItem(int id) {
		for (auto it = Inventory.begin(); it != Inventory.end(); ++it) {
			if ((*it)->getId() == id) {
				delete* it;
				Inventory.erase(it);
				cout << "Item removed."<<endl;
				return;
			}
		}
		cout << "Item not found."<<endl;
	}

	void searchItem(string query) {
		for (auto item : Inventory) {
			if (to_string(item->getId()) == query || item->getTitle() == query || item -> getAuthor()==query) {
				item->display(); cout << endl;
			}
		}
	}

	void listItems(bool availableOnly = false) {
		ofstream outFile("output2.txt");
		for (auto item : Inventory) {
			item->display();
			cout << endl;
			// This creates a document for the librarian to have and keep record of. 
			cout << "A document listing all items has been created as well."<<endl;
			if (outFile.is_open()) {
				outFile <<"[" << item->gettype()<< "]" << " | " << item->getTitle() << " | " << item->getAuthor() << " | " << (item->getStatus() ? "Borrowed" : "Available") << endl;
			}
		}
		outFile.close();
	}

	
	void registerMember(int id, string name) {
		members.push_back(Member(id, name));
		cout << "Member registered."<<endl;
	}
	// This allows for the member to borrow items from the library.
	void borrowProcess(int MemberID, int ItemID) {
		Member* m = findMember(MemberID);
		libraryItem* item = findItem(ItemID);

		if (m && item && !item->getStatus()) {
			item->setborrowedstatus(true, currentDate);
			m->addLoan(ItemID);
			m->addHistory("Borrowed " + item->getTitle() + " on day " + to_string(currentDate));
			cout << "Loan successful! Due in " << item->setloanperiod() << " days."<<endl;
		}
		else {
			cout << "Borrowing failed. Check IDs or availability."<<endl;
		}
	}
	// This function allows for the return of items back to the library.
	void returnProcess(int MemberID, int ItemID, int returnDay) {
		Member* m = findMember(MemberID);
		libraryItem* item = findItem(ItemID);

		if (m && item && item->getStatus()) {
			double fine = item->calculatefine(returnDay);
			item->setborrowedstatus(false);
			m->returnLoan(ItemID);
			m->addHistory("Returned " + item->getTitle() + " on day " + to_string(returnDay));
			cout << "Item returned. Fine: $" << fixed << fine << endl;
		}
	}


	Member* findMember(int id) {
		for (auto& m : members) if (m.getId() == id) return &m;
		return nullptr;
	}
	libraryItem* findItem(int id) {
		for (auto i : Inventory) if (i->getId() == id) return i;
		return nullptr;
	}
};

// Menu or user interface 
int main() {
	LibrarySystem lib;
	int choice, subChoice;

	while (true) {
		cout << "[Gavin's Digital Library]" << endl;
		cout << "1. Librarian Menu" << endl;
		cout << "2. Member Menu" << endl;
		cout << "3. Exit" << endl;
		cout << "Choice:" << endl;
		
		cin >> choice;

		if (choice == 1) {
			cout << "1. Add Item" << endl;
			cout << "2. Remove Item" << endl;
			cout << "3. Search" << endl;
			cout << "4. List All" << endl;
			cout << "Choice: ";

			cin >> subChoice;

			
			if (subChoice == 1) {
				int id, t; string title; string author;
				cout << "Type (1:Book, 2:Mag, 3:CD, 4:DVD): "; cin >> t;
				cout << "ID: "; cin >> id;
				cout << "Title: "; cin.ignore(); getline(cin, title);
				cout << "Author: "; getline(cin, author);
				lib.addItem(t, id, title, author);
			}

			//Removes item
			else if (subChoice == 2) {
				int id;
				cout << "Enter ID of the item to remove: ";
				cin >> id;
				lib.removeItem(id);
			
			}

			//Searches
			else if (subChoice == 3) {
				string query;
				cout << "Enter Title or Author to search for: ";
				cin.ignore(); 
				getline(cin, query);
				lib.searchItem(query);
				
			}

			else if (subChoice == 4) {
				lib.listItems();
			}
		}
		else if (choice == 2) {
			cout << "1. Register" << endl; 
			cout << "2. Borrow" << endl;
			cout << "3. Return" << endl;
			cout << "4. View History" << endl; 
			cout << "Choice:" << endl;
			cin >> subChoice;
			if (subChoice == 1) {
				int id; string name;
				cout << "ID: "; cin >> id;
				cout << "Name: "; cin.ignore(); getline(cin, name);
				lib.registerMember(id, name);
			}
			else if (subChoice == 2) {
				int MemberID, ItemID;
				cout << "Member ID: "; cin >> MemberID;
				cout << "Item ID: "; cin >> ItemID;
				lib.borrowProcess(MemberID, ItemID);
			}
			else if (subChoice == 3) {
				int MemberID, IteMemberID,returnDay;
				cout << "Member ID: "; cin >> MemberID;
				cout << "Item ID: "; cin >> IteMemberID;
				cout << "Current Day (e.g. 120): "; cin >> returnDay;
				lib.returnProcess(MemberID, IteMemberID,returnDay);
			}
		}
		else break;
	}
	return 0;
}
