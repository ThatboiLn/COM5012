// =============================================================================
// main.cpp — SLMS entry point and interactive menu system
// =============================================================================

#include "catalogue.h"
#include <limits>

// ─────────────────────────────────────────────
// Input helpers
// ─────────────────────────────────────────────
static int getInt(const std::string& prompt) {
    int val;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return val;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  [!] Please enter a valid number.\n";
    }
}

static std::string getString(const std::string& prompt) {
    std::string val;
    std::cout << prompt;
    std::getline(std::cin, val);
    return val;
}

// =============================================================================
// SLMS — top-level application class
// =============================================================================
class SLMS {
private:
    Catalogue   catalogue;
    UserManager userManager;
    int         currentDay;
    float       latePenaltyPerDay;

    // ── Seed initial data ─────────────────────
    void seedData() {
        // Books
        catalogue.addBook("The Pragmatic Programmer", "David Thomas",   "978-0135957059");
        catalogue.addBook("Clean Code",                "Robert Martin",  "978-0132350884");
        catalogue.addBook("Design Patterns",           "Gang of Four",   "978-0201633610");
        catalogue.addBook("The C++ Programming Language","Bjarne Stroustrup","978-0321563842");
        catalogue.addBook("Algorithms",                "Robert Sedgewick","978-0321573513");

        // Users — IDs assigned manually for predictable login
        userManager.addUser(new Member      (1,  "Alice Smith",   "alice@example.com",  "1234"));
        userManager.addUser(new Member      (2,  "Bob Jones",     "bob@example.com",    "2345"));
        userManager.addUser(new Librarian   (10, "Carol White",   "carol@lib.com",      "9999", "LIB-01"));
        userManager.addUser(new Administrator(20, "Dave Admin",   "dave@admin.com",     "0000"));

        std::cout << "\n  [i] System seeded. Accounts:\n"
                  << "      Member     ID 1  PIN 1234 (Alice)\n"
                  << "      Member     ID 2  PIN 2345 (Bob)\n"
                  << "      Librarian  ID 10 PIN 9999 (Carol)\n"
                  << "      Admin      ID 20 PIN 0000 (Dave)\n\n";
    }

    // ── Member menu ───────────────────────────
    void memberMenu(Member* m) {
        while (true) {
            std::cout << "\n  ┌─ Member Menu (" << m->getName() << ") ─────────────────\n"
                      << "  │  1. Browse all books\n"
                      << "  │  2. Search by title\n"
                      << "  │  3. Search by author\n"
                      << "  │  4. Borrow a book\n"
                      << "  │  5. Return a book\n"
                      << "  │  6. Reserve a book\n"
                      << "  │  7. View my account\n"
                      << "  │  0. Logout\n"
                      << "  └──────────────────────────────────────────\n";
            int choice = getInt("  Choice: ");
            switch (choice) {
                case 1:
                    catalogue.displayAll();
                    break;
                case 2: {
                    auto q       = getString("  Search title: ");
                    auto results = catalogue.searchByTitle(q);
                    if (results.empty()) std::cout << "  No results found.\n";
                    else for (auto* b : results) b->displayInfo();
                    break;
                }
                case 3: {
                    auto q       = getString("  Search author: ");
                    auto results = catalogue.searchByAuthor(q);
                    if (results.empty()) std::cout << "  No results found.\n";
                    else for (auto* b : results) b->displayInfo();
                    break;
                }
                case 4: {
                    catalogue.displayAll();
                    int id = getInt("  Book ID to borrow: ");
                    Book* b = catalogue.findById(id);
                    if (!b) std::cout << "  [!] Book not found.\n";
                    else    m->borrowBook(*b, currentDay);
                    break;
                }
                case 5: {
                    int id = getInt("  Book ID to return: ");
                    Book* b = catalogue.findById(id);
                    if (!b) std::cout << "  [!] Book not found.\n";
                    else    m->returnBook(*b, currentDay, latePenaltyPerDay);
                    break;
                }
                case 6: {
                    catalogue.displayAll();
                    int id = getInt("  Book ID to reserve: ");
                    Book* b = catalogue.findById(id);
                    if (!b) std::cout << "  [!] Book not found.\n";
                    else    m->reserveBook(*b, currentDay);
                    break;
                }
                case 7:
                    m->displayInfo();
                    break;
                case 0:
                    std::cout << "  Logged out.\n";
                    return;
                default:
                    std::cout << "  [!] Invalid option.\n";
            }
        }
    }

    // ── Librarian menu ────────────────────────
    void librarianMenu(Librarian* l) {
        while (true) {
            std::cout << "\n  ┌─ Librarian Menu (" << l->getName() << ") ─────────────\n"
                      << "  │  1. Browse all books\n"
                      << "  │  2. Add a book\n"
                      << "  │  3. Remove a book\n"
                      << "  │  4. Update a book\n"
                      << "  │  5. Overdue report\n"
                      << "  │  0. Logout\n"
                      << "  └──────────────────────────────────────────\n";
            int choice = getInt("  Choice: ");
            switch (choice) {
                case 1:
                    catalogue.displayAll();
                    break;
                case 2: {
                    auto title  = getString("  Title: ");
                    auto author = getString("  Author: ");
                    auto isbn   = getString("  ISBN: ");
                    catalogue.addBook(title, author, isbn);
                    break;
                }
                case 3: {
                    catalogue.displayAll();
                    int id = getInt("  Book ID to remove: ");
                    l->removeBook(catalogue, id);
                    break;
                }
                case 4: {
                    catalogue.displayAll();
                    int  id     = getInt("  Book ID to update: ");
                    auto title  = getString("  New title: ");
                    auto author = getString("  New author: ");
                    l->updateBook(catalogue, id, title, author);
                    break;
                }
                case 5:
                    l->generateOverdueReport(catalogue, currentDay);
                    break;
                case 0:
                    std::cout << "  Logged out.\n";
                    return;
                default:
                    std::cout << "  [!] Invalid option.\n";
            }
        }
    }

    // ── Admin menu ────────────────────────────
    void adminMenu(Administrator* a) {
        while (true) {
            std::cout << "\n  ┌─ Admin Menu (" << a->getName() << ") ──────────────────\n"
                      << "  │  1. List all users\n"
                      << "  │  2. Add member\n"
                      << "  │  3. Remove user\n"
                      << "  │  4. Set member borrow limit\n"
                      << "  │  5. Set late penalty (per day)\n"
                      << "  │  6. Advance simulated day counter\n"
                      << "  │  0. Logout\n"
                      << "  └──────────────────────────────────────────\n";
            int choice = getInt("  Choice: ");
            switch (choice) {
                case 1:
                    userManager.displayAllUsers();
                    break;
                case 2: {
                    auto name  = getString("  Name: ");
                    auto email = getString("  Email: ");
                    auto pin   = getString("  PIN: ");
                    int  id    = userManager.getNextId();
                    userManager.addUser(new Member(id, name, email, pin));
                    std::cout << "  [✓] Member created (ID " << id << ").\n";
                    break;
                }
                case 3: {
                    int id = getInt("  User ID to remove: ");
                    userManager.removeUser(id);
                    break;
                }
                case 4: {
                    int   id    = getInt("  Member ID: ");
                    User* u     = userManager.findById(id);
                    if (!u || u->getRole() != "Member") {
                        std::cout << "  [!] Member not found.\n";
                        break;
                    }
                    int limit = getInt("  New borrow limit: ");
                    a->setBorrowLimit(*static_cast<Member*>(u), limit);
                    break;
                }
                case 5: {
                    float p;
                    std::cout << "  New penalty per day (£): ";
                    std::cin >> p;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    latePenaltyPerDay = p;
                    std::cout << "  [✓] Penalty set to £" << std::fixed
                              << std::setprecision(2) << latePenaltyPerDay << "/day.\n";
                    break;
                }
                case 6: {
                    int days = getInt("  Advance by how many days? ");
                    currentDay += days;
                    catalogue.tickReservations(currentDay);
                    std::cout << "  [✓] Day counter is now " << currentDay << ".\n";
                    break;
                }
                case 0:
                    std::cout << "  Logged out.\n";
                    return;
                default:
                    std::cout << "  [!] Invalid option.\n";
            }
        }
    }

public:
    SLMS() : currentDay(1), latePenaltyPerDay(0.20f) {
        seedData();
    }

    /// Main run loop — authenticate then dispatch to role menu
    void run() {
        std::cout << "╔══════════════════════════════════════════════╗\n"
                  << "║   Smart Library Management System (SLMS)    ║\n"
                  << "║         COM5012 — CW1 Implementation        ║\n"
                  << "╚══════════════════════════════════════════════╝\n";

        while (true) {
            std::cout << "\n  Current day: " << currentDay << "\n";
            int uid = getInt("  User ID (0 to quit): ");
            if (uid == 0) break;

            auto pin  = getString("  PIN: ");
            User* user = userManager.authenticate(uid, pin);

            if (!user) {
                std::cout << "  [!] Invalid credentials. Try again.\n";
                continue;
            }

            std::cout << "  Welcome, " << user->getName()
                      << " (" << user->getRole() << ").\n";

            if (user->getRole() == "Member")
                memberMenu(static_cast<Member*>(user));
            else if (user->getRole() == "Librarian")
                librarianMenu(static_cast<Librarian*>(user));
            else if (user->getRole() == "Administrator")
                adminMenu(static_cast<Administrator*>(user));
        }

        std::cout << "\n  Goodbye!\n";
    }
};

// =============================================================================
// Entry point
// =============================================================================
int main() {
    SLMS app;
    app.run();
    return 0;
}
