// =============================================================================
// SLMS - Smart Library Management System
// COM5012 Object Oriented Programming — CW1
// =============================================================================

#ifndef SLMS_H
#define SLMS_H

#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <functional>
#include <iostream>
#include <iomanip>

// ─────────────────────────────────────────────
// Simple hash for PIN storage (not cryptographic,
// but avoids storing plaintext as per NFR02)
// ─────────────────────────────────────────────
inline std::size_t hashPin(const std::string& pin) {
    return std::hash<std::string>{}(pin);
}

// Case-insensitive substring search helper
inline bool containsIgnoreCase(const std::string& haystack, const std::string& needle) {
    std::string h = haystack, n = needle;
    std::transform(h.begin(), h.end(), h.begin(), ::tolower);
    std::transform(n.begin(), n.end(), n.begin(), ::tolower);
    return h.find(n) != std::string::npos;
}

// ─────────────────────────────────────────────
// BookStatus enum
// ─────────────────────────────────────────────
enum class BookStatus { AVAILABLE, BORROWED, RESERVED };

inline std::string statusToString(BookStatus s) {
    switch (s) {
        case BookStatus::AVAILABLE: return "Available";
        case BookStatus::BORROWED:  return "Borrowed";
        case BookStatus::RESERVED:  return "Reserved";
    }
    return "Unknown";
}

// =============================================================================
// Book — encapsulates all data and behaviour for a single library book
// =============================================================================
class Book {
private:
    int         bookId;
    std::string title;
    std::string author;
    std::string isbn;
    BookStatus  status;
    int         borrowedByMemberId;   // -1 if not borrowed
    int         dueDay;               // simulated day counter
    int         reservedByMemberId;   // -1 if not reserved
    int         reservationExpiry;    // day reservation expires

public:
    Book(int id, const std::string& title, const std::string& author, const std::string& isbn)
        : bookId(id), title(title), author(author), isbn(isbn),
          status(BookStatus::AVAILABLE), borrowedByMemberId(-1),
          dueDay(-1), reservedByMemberId(-1), reservationExpiry(-1) {}

    // ── Getters ──────────────────────────────
    int         getBookId()             const { return bookId; }
    std::string getTitle()              const { return title; }
    std::string getAuthor()             const { return author; }
    std::string getIsbn()               const { return isbn; }
    BookStatus  getStatus()             const { return status; }
    int         getBorrowedByMemberId() const { return borrowedByMemberId; }
    int         getDueDay()             const { return dueDay; }
    int         getReservedByMemberId() const { return reservedByMemberId; }

    // ── Setters ──────────────────────────────
    void setTitle(const std::string& t)  { title = t; }
    void setAuthor(const std::string& a) { author = a; }
    void setIsbn(const std::string& i)   { isbn = i; }

    // ── Core operations ──────────────────────

    /// Borrow the book; returns false if not available
    bool borrow(int memberId, int currentDay) {
        if (status != BookStatus::AVAILABLE) {
            std::cout << "  [!] \"" << title << "\" is not available (status: "
                      << statusToString(status) << ").\n";
            return false;
        }
        status             = BookStatus::BORROWED;
        borrowedByMemberId = memberId;
        dueDay             = currentDay + 14;
        reservedByMemberId = -1;
        return true;
    }

    /// Return the book, resetting loan fields
    void returnBook() {
        status             = BookStatus::AVAILABLE;
        borrowedByMemberId = -1;
        dueDay             = -1;
    }

    /// Reserve the book if it is currently on loan
    bool reserve(int memberId, int currentDay) {
        if (status == BookStatus::AVAILABLE) {
            std::cout << "  [!] Book is available — borrow it directly instead.\n";
            return false;
        }
        if (reservedByMemberId != -1) {
            std::cout << "  [!] Book is already reserved by another member.\n";
            return false;
        }
        reservedByMemberId = memberId;
        reservationExpiry  = currentDay + 3;
        // Note: status stays BORROWED until the loan is returned
        return true;
    }

    /// Expire a reservation if the deadline has passed
    void checkReservationExpiry(int currentDay) {
        if (reservedByMemberId != -1 && currentDay > reservationExpiry) {
            std::cout << "  [i] Reservation on \"" << title << "\" has expired.\n";
            reservedByMemberId = -1;
            reservationExpiry  = -1;
        }
    }

    /// Returns true if the book is overdue
    bool isOverdue(int currentDay) const {
        return (status == BookStatus::BORROWED) && (currentDay > dueDay);
    }

    /// Display a formatted summary of the book
    void displayInfo() const {
        std::cout << "  [" << bookId << "] \"" << title << "\" by " << author
                  << " | ISBN: " << isbn
                  << " | Status: " << statusToString(status);
        if (status == BookStatus::BORROWED)
            std::cout << " (due day " << dueDay << ")";
        std::cout << "\n";
    }
};

// =============================================================================
// User — abstract base class demonstrating encapsulation and polymorphism
// =============================================================================
class User {
protected:
    int         userId;
    std::string name;
    std::string email;
    std::size_t hashedPin;   // stored as hash, never plaintext (NFR02)

public:
    User(int id, const std::string& name, const std::string& email, const std::string& pin)
        : userId(id), name(name), email(email), hashedPin(hashPin(pin)) {}

    virtual ~User() = default;

    // ── Getters ──────────────────────────────
    int         getUserId() const { return userId; }
    std::string getName()   const { return name; }
    std::string getEmail()  const { return email; }

    /// Verify a supplied PIN against the stored hash
    bool verifyPin(const std::string& pin) const {
        return hashPin(pin) == hashedPin;
    }

    // ── Pure virtual interface (polymorphism) ─
    virtual void        displayInfo() = 0;
    virtual std::string getRole()     = 0;
};

#endif // SLMS_H
