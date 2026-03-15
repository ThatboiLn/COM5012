// =============================================================================
// users.h — Member, Librarian, Administrator
// All inherit from User, overriding displayInfo() and getRole()
// =============================================================================

#ifndef USERS_H
#define USERS_H

#include "slms.h"

// Forward declarations
class Catalogue;
class UserManager;

// =============================================================================
// Member — a library patron who can borrow, return, and reserve books
// =============================================================================
class Member : public User {
private:
    int               borrowLimit;      // max concurrent loans (default 5)
    float             fineBalance;      // accumulated late-return fines
    std::vector<int>  activeLoanIds;    // book IDs currently on loan
    std::vector<int>  reservationIds;   // book IDs currently reserved

public:
    Member(int id, const std::string& name, const std::string& email, const std::string& pin)
        : User(id, name, email, pin), borrowLimit(5), fineBalance(0.0f) {}

    // ── Getters / Setters ─────────────────────
    int   getBorrowLimit()           const { return borrowLimit; }
    void  setBorrowLimit(int limit)        { borrowLimit = limit; }
    float getFineBalance()           const { return fineBalance; }
    void  addFine(float amount)            { fineBalance += amount; }
    int   getActiveLoanCount()       const { return static_cast<int>(activeLoanIds.size()); }

    /// Attempt to borrow a book; enforces borrow limit
    bool borrowBook(Book& book, int currentDay) {
        if (static_cast<int>(activeLoanIds.size()) >= borrowLimit) {
            std::cout << "  [!] Borrow limit (" << borrowLimit << ") reached.\n";
            return false;
        }
        if (book.borrow(userId, currentDay)) {
            activeLoanIds.push_back(book.getBookId());
            std::cout << "  [✓] \"" << book.getTitle() << "\" borrowed. Due on day "
                      << book.getDueDay() << ".\n";
            return true;
        }
        return false;
    }

    /// Return a borrowed book; applies fine if overdue
    void returnBook(Book& book, int currentDay, float penaltyPerDay) {
        if (book.getBorrowedByMemberId() != userId) {
            std::cout << "  [!] You did not borrow this book.\n";
            return;
        }
        if (book.isOverdue(currentDay)) {
            int   daysLate = currentDay - book.getDueDay();
            float fine     = daysLate * penaltyPerDay;
            fineBalance   += fine;
            std::cout << "  [!] Book is " << daysLate << " day(s) overdue. Fine: £"
                      << std::fixed << std::setprecision(2) << fine << " added.\n";
        }
        // Remove from active loans list
        activeLoanIds.erase(
            std::remove(activeLoanIds.begin(), activeLoanIds.end(), book.getBookId()),
            activeLoanIds.end());
        book.returnBook();
        std::cout << "  [✓] \"" << book.getTitle() << "\" returned successfully.\n";
    }

    /// Reserve a book that is currently on loan
    bool reserveBook(Book& book, int currentDay) {
        if (book.reserve(userId, currentDay)) {
            reservationIds.push_back(book.getBookId());
            std::cout << "  [✓] \"" << book.getTitle()
                      << "\" reserved. Collect within 3 days of it becoming available.\n";
            return true;
        }
        return false;
    }

    /// Polymorphic display — shows member-specific info
    void displayInfo() override {
        std::cout << "  Member #" << userId << ": " << name
                  << " | Email: " << email
                  << " | Active Loans: " << activeLoanIds.size()
                  << "/" << borrowLimit
                  << " | Fine Balance: £" << std::fixed << std::setprecision(2) << fineBalance << "\n";
        if (!activeLoanIds.empty()) {
            std::cout << "  Loan IDs: ";
            for (int id : activeLoanIds) std::cout << id << " ";
            std::cout << "\n";
        }
    }

    std::string getRole() override { return "Member"; }
};

// =============================================================================
// Librarian — staff who manage the catalogue and process requests
// =============================================================================
class Catalogue; // forward declare for method signatures

class Librarian : public User {
private:
    std::string staffCode;

public:
    Librarian(int id, const std::string& name, const std::string& email,
              const std::string& pin, const std::string& staffCode)
        : User(id, name, email, pin), staffCode(staffCode) {}

    std::string getStaffCode() const { return staffCode; }

    /// Add a new book to the catalogue
    void addBook(Catalogue& cat, const Book& book);

    /// Remove a book by ID; returns false if not found
    bool removeBook(Catalogue& cat, int bookId);

    /// Update a book's title and author
    bool updateBook(Catalogue& cat, int bookId,
                    const std::string& newTitle, const std::string& newAuthor);

    /// Print all overdue books
    void generateOverdueReport(Catalogue& cat, int currentDay);

    void displayInfo() override {
        std::cout << "  Librarian #" << userId << ": " << name
                  << " | Staff Code: " << staffCode << "\n";
    }

    std::string getRole() override { return "Librarian"; }
};

// =============================================================================
// Administrator — manages user accounts and system-wide settings
// =============================================================================
class Administrator : public User {
public:
    Administrator(int id, const std::string& name,
                  const std::string& email, const std::string& pin)
        : User(id, name, email, pin) {}

    void setBorrowLimit(Member& member, int limit) {
        member.setBorrowLimit(limit);
        std::cout << "  [✓] Borrow limit for " << member.getName()
                  << " set to " << limit << ".\n";
    }

    void displayInfo() override {
        std::cout << "  Administrator #" << userId << ": " << name
                  << " | Email: " << email << "\n";
    }

    std::string getRole() override { return "Administrator"; }
};

#endif // USERS_H
