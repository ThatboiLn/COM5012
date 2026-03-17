# UML Class Diagram

```mermaid
classDiagram
    class User {
        #int userId
        #string name
        #string email
        #string hashedPin
        +User(int id, string name, string email, string pin)
        +getUserId() int
        +getName() string
        +getEmail() string
        +verifyPin(string pin) bool
        +displayInfo()* void
        +getRole()* string
    }

    class Member {
        -int borrowLimit
        -float fineBalance
        -vector~int~ activeLoanIds
        -vector~int~ reservationIds
        +Member(int id, string name, string email, string pin)
        +borrowBook(Book& book) bool
        +returnBook(Book& book) void
        +reserveBook(Book& book) bool
        +getActiveLoanCount() int
        +getFineBalance() float
        +addFine(float amount) void
        +displayInfo() void
        +getRole() string
    }

    class Librarian {
        -string staffCode
        +Librarian(int id, string name, string email, string pin, string staffCode)
        +addBook(Catalogue& cat, Book book) void
        +removeBook(Catalogue& cat, int bookId) bool
        +updateBook(Catalogue& cat, int bookId, string title, string author) bool
        +generateOverdueReport(Catalogue& cat, int currentDay) void
        +displayInfo() void
        +getRole() string
    }

    class Administrator {
        +Administrator(int id, string name, string email, string pin)
        +createMember(UserManager& um, string name, string email, string pin) void
        +deleteMember(UserManager& um, int memberId) bool
        +setBorrowLimit(Member& m, int limit) void
        +setLatePenalty(float penaltyPerDay) void
        +displayInfo() void
        +getRole() string
    }

    class Book {
        -int bookId
        -string title
        -string author
        -string isbn
        -BookStatus status
        -int borrowedByMemberId
        -int dueDay
        -int reservedByMemberId
        -int reservationExpiry
        +Book(int id, string title, string author, string isbn)
        +getBookId() int
        +getTitle() string
        +getAuthor() string
        +getStatus() BookStatus
        +setStatus(BookStatus s) void
        +borrow(int memberId, int currentDay) bool
        +returnBook() void
        +reserve(int memberId, int currentDay) bool
        +isOverdue(int currentDay) bool
        +getDueDay() int
        +displayInfo() void
    }

    class Catalogue {
        -vector~Book~ books
        -int nextBookId
        +addBook(Book book) void
        +removeBook(int bookId) bool
        +findById(int bookId) Book*
        +searchByTitle(string query) vector~Book*~
        +searchByAuthor(string query) vector~Book*~
        +getOverdueBooks(int currentDay) vector~Book*~
        +displayAll() void
    }

    class UserManager {
        -vector~User*~ users
        -int nextUserId
        +addUser(User* user) void
        +removeUser(int userId) bool
        +findById(int userId) User*
        +authenticate(int userId, string pin) User*
        +displayAllMembers() void
    }

    class SLMS {
        -Catalogue catalogue
        -UserManager userManager
        -int currentDay
        -float latePenaltyPerDay
        +SLMS()
        +run() void
        -memberMenu(Member* m) void
        -librarianMenu(Librarian* l) void
        -adminMenu(Administrator* a) void
        -advanceDay(int days) void
        -seedData() void
    }

    User <|-- Member : inherits
    User <|-- Librarian : inherits
    User <|-- Administrator : inherits
    SLMS "1" *-- "1" Catalogue : owns
    SLMS "1" *-- "1" UserManager : owns
    Catalogue "1" *-- "0..*" Book : contains
    UserManager "1" o-- "0..*" User : manages
    Member ..> Book : borrows/reserves
    Librarian ..> Catalogue : manages
    Administrator ..> UserManager : manages
```

## Key Design Decisions

- **Inheritance hierarchy:** `User` is the abstract base class. `Member`, `Librarian`, and `Administrator` inherit from it and override `displayInfo()` and `getRole()` — demonstrating **polymorphism**.
- **Encapsulation:** All data attributes are private or protected. Access is strictly through public getter/setter methods.
- **Composition:** `SLMS` owns `Catalogue` and `UserManager`. `Catalogue` owns a collection of `Book` objects.
- **BookStatus** is an enum: `AVAILABLE`, `BORROWED`, `RESERVED`.
