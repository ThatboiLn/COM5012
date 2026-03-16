// =============================================================================
// catalogue.h — Catalogue and UserManager
// =============================================================================

#ifndef CATALOGUE_H
#define CATALOGUE_H

#include "slms.h"
#include "users.h"

// =============================================================================
// Catalogue — manages the collection of Book objects
// =============================================================================
class Catalogue {
private:
    std::vector<Book> books;
    int               nextBookId;

public:
    Catalogue() : nextBookId(1) {}

    /// Add a book; auto-assigns an ID
    void addBook(const std::string& title, const std::string& author,
                 const std::string& isbn) {
        books.emplace_back(nextBookId++, title, author, isbn);
        std::cout << "  [✓] Book added: \"" << title << "\" (ID " << (nextBookId-1) << ").\n";
    }

    /// Add a pre-constructed book (used by Librarian and seeding)
    void addBook(const Book& book) {
        books.push_back(book);
    }

    /// Remove a book by ID; returns false if not found or currently on loan
    bool removeBook(int bookId) {
        auto it = std::find_if(books.begin(), books.end(),
            [bookId](const Book& b) { return b.getBookId() == bookId; });
        if (it == books.end()) {
            std::cout << "  [!] Book ID " << bookId << " not found.\n";
            return false;
        }
        if (it->getStatus() != BookStatus::AVAILABLE) {
            std::cout << "  [!] Cannot remove a book that is currently "
                      << statusToString(it->getStatus()) << ".\n";
            return false;
        }
        books.erase(it);
        std::cout << "  [✓] Book ID " << bookId << " removed.\n";
        return true;
    }

    /// Find a book by ID; returns nullptr if not found
    Book* findById(int bookId) {
        for (auto& b : books)
            if (b.getBookId() == bookId) return &b;
        return nullptr;
    }

    /// Case-insensitive title search
    std::vector<Book*> searchByTitle(const std::string& query) {
        std::vector<Book*> results;
        for (auto& b : books)
            if (containsIgnoreCase(b.getTitle(), query))
                results.push_back(&b);
        return results;
    }

    /// Case-insensitive author search
    std::vector<Book*> searchByAuthor(const std::string& query) {
        std::vector<Book*> results;
        for (auto& b : books)
            if (containsIgnoreCase(b.getAuthor(), query))
                results.push_back(&b);
        return results;
    }

    /// Return all books that are past their due date
    std::vector<Book*> getOverdueBooks(int currentDay) {
        std::vector<Book*> results;
        for (auto& b : books)
            if (b.isOverdue(currentDay))
                results.push_back(&b);
        return results;
    }

    /// Expire stale reservations
    void tickReservations(int currentDay) {
        for (auto& b : books)
            b.checkReservationExpiry(currentDay);
    }

    /// Update a book's title and author by ID
    bool updateBook(int bookId, const std::string& title, const std::string& author) {
        Book* b = findById(bookId);
        if (!b) {
            std::cout << "  [!] Book ID " << bookId << " not found.\n";
            return false;
        }
        b->setTitle(title);
        b->setAuthor(author);
        std::cout << "  [✓] Book ID " << bookId << " updated.\n";
        return true;
    }

    void displayAll() const {
        if (books.empty()) {
            std::cout << "  No books in catalogue.\n";
            return;
        }
        std::cout << "  ── Catalogue (" << books.size() << " books) ──\n";
        for (const auto& b : books) b.displayInfo();
    }

    int getNextBookId() const { return nextBookId; }
};

// ── Librarian method implementations (need Catalogue fully defined) ───────────

inline void Librarian::addBook(Catalogue& cat, const Book& book) {
    cat.addBook(book);
    std::cout << "  [✓] Librarian " << name << " added a book.\n";
}

inline bool Librarian::removeBook(Catalogue& cat, int bookId) {
    return cat.removeBook(bookId);
}

inline bool Librarian::updateBook(Catalogue& cat, int bookId,
                                  const std::string& newTitle,
                                  const std::string& newAuthor) {
    return cat.updateBook(bookId, newTitle, newAuthor);
}

inline void Librarian::generateOverdueReport(Catalogue& cat, int currentDay) {
    auto overdue = cat.getOverdueBooks(currentDay);
    std::cout << "\n  ── Overdue Report (day " << currentDay << ") ──\n";
    if (overdue.empty()) {
        std::cout << "  No overdue books.\n";
        return;
    }
    for (auto* b : overdue)
        std::cout << "  Overdue: " << b->getTitle()
                  << " | Due day: " << b->getDueDay()
                  << " | Days late: " << (currentDay - b->getDueDay()) << "\n";
}

// =============================================================================
// UserManager — owns and manages all User objects (polymorphic container)
// =============================================================================
class UserManager {
private:
    std::vector<User*> users;   // owning pointers
    int                nextUserId;

public:
    UserManager() : nextUserId(100) {}

    ~UserManager() {
        for (auto* u : users) delete u;
    }

    /// Add a heap-allocated user (takes ownership)
    void addUser(User* user) {
        users.push_back(user);
    }

    /// Remove a user by ID; returns false if not found
    bool removeUser(int userId) {
        auto it = std::find_if(users.begin(), users.end(),
            [userId](User* u) { return u->getUserId() == userId; });
        if (it == users.end()) {
            std::cout << "  [!] User ID " << userId << " not found.\n";
            return false;
        }
        delete *it;
        users.erase(it);
        std::cout << "  [✓] User ID " << userId << " removed.\n";
        return true;
    }

    /// Find a user by ID; returns nullptr if not found
    User* findById(int userId) {
        for (auto* u : users)
            if (u->getUserId() == userId) return u;
        return nullptr;
    }

    /// Authenticate a user: verifies ID exists and PIN matches
    User* authenticate(int userId, const std::string& pin) {
        User* u = findById(userId);
        if (!u) return nullptr;
        return u->verifyPin(pin) ? u : nullptr;
    }

    /// Display all members using polymorphic displayInfo()
    void displayAllUsers() {
        std::cout << "  ── All Users ──\n";
        for (auto* u : users) u->displayInfo();
    }

    int getNextId() { return nextUserId++; }
};

#endif // CATALOGUE_H
