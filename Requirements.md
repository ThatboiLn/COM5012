# SLMS Requirements

## 1. Functional Requirements

| ID | Requirement | Priority |
|----|-------------|----------|
| FR01 | The system shall allow Members to log in using a unique member ID and PIN | High |
| FR02 | The system shall allow Members to search for books by title or author | High |
| FR03 | The system shall allow Members to borrow a book if it is available and they have fewer than 5 active loans | High |
| FR04 | The system shall allow Members to return a borrowed book | High |
| FR05 | The system shall allow Members to reserve a book that is currently on loan | Medium |
| FR06 | The system shall update a book's status automatically upon borrowing (→ "Borrowed") and returning (→ "Available") | High |
| FR07 | The system shall set a due date (14 days from borrow date) when a book is borrowed | High |
| FR08 | The system shall allow Librarians to add new books to the catalogue | High |
| FR09 | The system shall allow Librarians to remove books from the catalogue | High |
| FR10 | The system shall allow Librarians to update book details (title, author, ISBN) | Medium |
| FR11 | The system shall allow Librarians to view a list of all overdue books | High |
| FR12 | The system shall allow Librarians to process borrowing and return requests on behalf of Members | Medium |
| FR13 | The system shall allow Administrators to create and delete Member accounts | High |
| FR14 | The system shall allow Administrators to create and delete Librarian accounts | High |
| FR15 | The system shall allow Administrators to set the maximum number of books a Member may borrow at once (default: 5) | Medium |
| FR16 | The system shall allow Administrators to set the daily late-return penalty (default: £0.20/day) | Medium |
| FR17 | The system shall expire a reservation automatically after 3 days if the book is not collected | Medium |
| FR18 | The system shall display a Member's current loans and reservations when requested | Medium |

## 2. Non-Functional Requirements

| ID | Requirement | Category |
|----|-------------|----------|
| NFR01 | The system shall respond to any user action (search, borrow, return) within 1 second | Performance |
| NFR02 | All member PINs shall be stored as hashed values; no plaintext credentials in memory beyond authentication | Security |
| NFR03 | The system shall handle invalid inputs (e.g., non-existent IDs, incorrect types) without crashing | Reliability |
| NFR04 | The system shall be implemented using OO principles: inheritance, encapsulation, and polymorphism | Maintainability |
| NFR05 | All classes shall have clear, consistent naming conventions following C++ best practices | Maintainability |
| NFR06 | The codebase shall include inline comments for all public member functions | Maintainability |
| NFR07 | The system shall run on any POSIX-compliant system (Linux/macOS) with a C++17-compatible compiler | Portability |

## 3. Scope & Assumptions

- **In scope:** Member login, book search, borrow/return/reserve, Librarian book management, Administrator account management, overdue reporting.
- **Out of scope:** Online/web interface, payment processing, email notifications, persistent database (flat-file simulation used instead).
- **Assumptions:**
  - One copy of each book is tracked per ISBN entry.
  - The system is used interactively via a CLI menu.
  - Dates are simulated using integer day counters for portability.
  - All data is held in-memory for this partial implementation.
