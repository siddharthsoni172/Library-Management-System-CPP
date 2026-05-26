#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstring>

using namespace std;

// Class to manage Book records
class Book {
private:
    int bookID;
    char title[50];
    char author[50];
    bool isIssued;
    int issuedToMemberID;

public:
    void addBook() {
        cout << "\nEnter Book ID: ";
        cin >> bookID;
        cin.ignore(); // Clear buffer

        cout << "Enter Book Title: ";
        cin.getline(title, 50);

        cout << "Enter Author Name: ";
        cin.getline(author, 50);

        isIssued = false;
        issuedToMemberID = -1;
        cout << "\nBook added to catalog successfully!";
    }

    void displayBook() const {
        cout << "\n------------------------------------";
        cout << "\nBook ID    : " << bookID;
        cout << "\nTitle      : " << title;
        cout << "\nAuthor     : " << author;
        cout << "\nStatus     : " << (isIssued ? "Issued" : "Available");
        if (isIssued) {
            cout << "\nIssued To  : Member ID " << issuedToMemberID;
        }
        cout << "\n------------------------------------";
    }

    // Setters and Getters
    int getBookID() const { return bookID; }
    const char* getTitle() const { return title; }
    const char* getAuthor() const { return author; }
    bool getIssueStatus() const { return isIssued; }
    
    void issueBookTo(int memberID) {
        isIssued = true;
        issuedToMemberID = memberID;
    }

    void returnBook() {
        isIssued = false;
        issuedToMemberID = -1;
    }
};

// Class to manage Member records
class Member {
private:
    int memberID;
    char name[50];

public:
    void addMember() {
        cout << "\nEnter Member ID: ";
        cin >> memberID;
        cin.ignore();

        cout << "Enter Member Name: ";
        cin.getline(name, 50);
        cout << "\nMember registered successfully!";
    }

    void displayMember() const {
        cout << "\nID: " << memberID << " | Name: " << name;
    }

    int getMemberID() const { return memberID; }
};

// File Operation Function Declarations
void writeBook();
void writeMember();
void searchBook();
void issueBook();
void returnBookRecord();
void displayAllBooks();

int main() {
    int choice;
    do {
        cout << "\n======================================";
        cout << "\n      LIBRARY MANAGEMENT SYSTEM       ";
        cout << "\n======================================";
        cout << "\n1. Add New Book";
        cout << "\n2. Register New Member";
        cout << "\n3. Search Book (Title/Author)";
        cout << "\n4. Issue a Book";
        cout << "\n5. Return a Book";
        cout << "\n6. Display All Books";
        cout << "\n7. Exit";
        cout << "\n======================================";
        cout << "\nEnter your choice (1-7): ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a valid number.\n";
            continue;
        }

        switch (choice) {
            case 1: writeBook(); break;
            case 2: writeMember(); break;
            case 3: searchBook(); break;
            case 4: issueBook(); break;
            case 5: returnBookRecord(); break;
            case 6: displayAllBooks(); break;
            case 7: cout << "\nExiting Library System. Goodbye!\n"; break;
            default: cout << "\nInvalid choice! Please select between 1-7.\n";
        }
    } while (choice != 7);

    return 0;
}

void writeBook() {
    Book b;
    ofstream outFile("books.dat", ios::binary | ios::app);
    if (!outFile) {
        cout << "\nError processing file tracking database.";
        return;
    }
    b.addBook();
    outFile.write(reinterpret_cast<char*>(&b), sizeof(Book));
    outFile.close();
}

void writeMember() {
    Member m;
    ofstream outFile("members.dat", ios::binary | ios::app);
    if (!outFile) {
        cout << "\nError processing file tracking database.";
        return;
    }
    m.addMember();
    outFile.write(reinterpret_cast<char*>(&m), sizeof(Member));
    outFile.close();
}

void searchBook() {
    Book b;
    ifstream inFile("books.dat", ios::binary);
    if (!inFile) {
        cout << "\nNo books available in the catalog.";
        return;
    }

    cin.ignore();
    char searchQuery[50];
    cout << "\nEnter Book Title or Author name to search: ";
    cin.getline(searchQuery, 50);

    bool found = false;
    while (inFile.read(reinterpret_cast<char*>(&b), sizeof(Book))) {
        // Case-insensitive check using basic strstr conversion check
        if (listsubstr(b.getTitle(), searchQuery) || listsubstr(b.getAuthor(), searchQuery)) {
            b.displayBook();
            found = true;
        }
    }
    inFile.close();
    if (!found) cout << "\nNo matching books found for '" << searchQuery << "'.";
}

// Helper utility matching substring logic
bool listsubstr(const char* str, const char* target) {
    return (strstr(str, target) != nullptr);
}

void issueBook() {
    Book b;
    Member m;
    int targetBookID, targetMemberID;
    bool bookFound = false, memberFound = false;

    cout << "\nEnter Book ID to issue: ";
    cin >> targetBookID;
    cout << "Enter Member ID: ";
    cin >> targetMemberID;

    // Verify member registration existence
    ifstream memFile("members.dat", ios::binary);
    while (memFile.read(reinterpret_cast<char*>(&m), sizeof(Member))) {
        if (m.getMemberID() == targetMemberID) {
            memberFound = true;
            break;
        }
    }
    memFile.close();

    if (!memberFound) {
        cout << "\nTransaction Denied: Member ID not registered.";
        return;
    }

    // Process transactional update on book entity matching index block
    fstream bookFile("books.dat", ios::binary | ios::in | ios::out);
    while (bookFile.read(reinterpret_cast<char*>(&b), sizeof(Book))) {
        if (b.getBookID() == targetBookID) {
            bookFound = true;
            if (b.getIssueStatus()) {
                cout << "\nTransaction Denied: Book is already issued to another member.";
            } else {
                b.issueBookTo(targetMemberID);
                int pos = -1 * static_cast<int>(sizeof(Book));
                bookFile.seekp(pos, ios::cur);
                bookFile.write(reinterpret_cast<char*>(&b), sizeof(Book));
                cout << "\nBook ID " << targetBookID << " successfully issued to Member ID " << targetMemberID;
            }
            break;
        }
    }
    bookFile.close();
    if (!bookFound) cout << "\nBook ID record not found.";
}

void returnBookRecord() {
    Book b;
    int targetBookID;
    bool found = false;

    cout << "\nEnter Book ID to return: ";
    cin >> targetBookID;

    fstream bookFile("books.dat", ios::binary | ios::in | ios::out);
    while (bookFile.read(reinterpret_cast<char*>(&b), sizeof(Book))) {
        if (b.getBookID() == targetBookID) {
            found = true;
            if (!b.getIssueStatus()) {
                cout << "\nLog Error: This book is already present in the catalog layout.";
            } else {
                b.returnBook();
                int pos = -1 * static_cast<int>(sizeof(Book));
                bookFile.seekp(pos, ios::cur);
                bookFile.write(reinterpret_cast<char*>(&b), sizeof(Book));
                cout << "\nBook ID " << targetBookID << " returned successfully. Inventory updated.";
            }
            break;
        }
    }
    bookFile.close();
    if (!found) cout << "\nBook record entry mismatch.";
}

void displayAllBooks() {
    Book b;
    ifstream inFile("books.dat", ios::binary);
    if (!inFile) {
        cout << "\nNo books registered in library inventory.";
        return;
    }

    cout << "\n-------------------------------------------------------------";
    cout << "\n" << left << setw(10) << "ID" << setw(25) << "Title" << setw(18) << "Author" << "Status";
    cout << "\n-------------------------------------------------------------";
    while (inFile.read(reinterpret_cast<char*>(&b), sizeof(Book))) {
        cout << "\n" << left << setw(10) << b.getBookID()
             << setw(25) << b.getTitle()
             << setw(18) << b.getAuthor()
             << (b.getIssueStatus() ? "Issued" : "Available");
    }
    cout << "\n-------------------------------------------------------------\n";
    inFile.close();
}
