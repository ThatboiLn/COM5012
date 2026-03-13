# Pseudo-code — Detailed Design

## Book::borrow(memberId, currentDay)
```
FUNCTION borrow(memberId, currentDay):
    IF status IS NOT AVAILABLE THEN
        PRINT "Book not available"
        RETURN false
    END IF
    SET status = BORROWED
    SET borrowedByMemberId = memberId
    SET dueDay = currentDay + 14
    SET reservedByMemberId = -1
    RETURN true
END FUNCTION
```

## Book::returnBook()
```
FUNCTION returnBook():
    SET status = AVAILABLE
    SET borrowedByMemberId = -1
    SET dueDay = -1
END FUNCTION
```

## Book::reserve(memberId, currentDay)
```
FUNCTION reserve(memberId, currentDay):
    IF status IS AVAILABLE THEN
        PRINT "Book is available — borrow it instead"
        RETURN false
    END IF
    IF reservedByMemberId IS NOT -1 THEN
        PRINT "Book already reserved"
        RETURN false
    END IF
    SET status = RESERVED
    SET reservedByMemberId = memberId
    SET reservationExpiry = currentDay + 3
    RETURN true
END FUNCTION
```

## Book::isOverdue(currentDay)
```
FUNCTION isOverdue(currentDay):
    RETURN (status IS BORROWED) AND (currentDay > dueDay)
END FUNCTION
```

## Member::borrowBook(book)
```
FUNCTION borrowBook(book):
    IF activeLoanIds.size() >= borrowLimit THEN
        PRINT "Borrow limit reached"
        RETURN false
    END IF
    IF book.borrow(userId, currentDay) THEN
        ADD book.getBookId() TO activeLoanIds
        RETURN true
    END IF
    RETURN false
END FUNCTION
```

## Member::returnBook(book)
```
FUNCTION returnBook(book):
    IF book.getBorrowedByMemberId() != userId THEN
        PRINT "This book was not borrowed by this member"
        RETURN
    END IF
    IF book.isOverdue(currentDay) THEN
        daysLate = currentDay - book.getDueDay()
        fine = daysLate * LATE_PENALTY_PER_DAY
        fineBalance += fine
        PRINT "Overdue fine applied: £" + fine
    END IF
    REMOVE book.getBookId() FROM activeLoanIds
    book.returnBook()
END FUNCTION
```

## Catalogue::searchByTitle(query)
```
FUNCTION searchByTitle(query):
    results = empty list
    FOR EACH book IN books DO
        IF book.getTitle() CONTAINS query (case-insensitive) THEN
            ADD &book TO results
        END IF
    END FOR
    RETURN results
END FUNCTION
```

## Librarian::generateOverdueReport(catalogue, currentDay)
```
FUNCTION generateOverdueReport(catalogue, currentDay):
    overdueBooks = catalogue.getOverdueBooks(currentDay)
    IF overdueBooks IS EMPTY THEN
        PRINT "No overdue books."
        RETURN
    END IF
    PRINT "--- Overdue Report ---"
    FOR EACH book IN overdueBooks DO
        PRINT book.getTitle(), "due day:", book.getDueDay(), "current day:", currentDay
    END FOR
END FUNCTION
```

## UserManager::authenticate(userId, pin)
```
FUNCTION authenticate(userId, pin):
    user = findById(userId)
    IF user IS NULL THEN
        RETURN NULL
    END IF
    IF user.verifyPin(pin) THEN
        RETURN user
    END IF
    RETURN NULL
END FUNCTION
```

## SLMS::run() — Main Program Flow
```
FUNCTION run():
    seedData()   // populate sample books and users
    LOOP:
        PRINT "Enter User ID (0 to quit):"
        INPUT userId
        IF userId == 0 THEN EXIT LOOP
        PRINT "Enter PIN:"
        INPUT pin
        user = userManager.authenticate(userId, pin)
        IF user IS NULL THEN
            PRINT "Invalid credentials"
            CONTINUE
        END IF
        IF user.getRole() == "Member" THEN
            memberMenu(cast user to Member*)
        ELSE IF user.getRole() == "Librarian" THEN
            librarianMenu(cast user to Librarian*)
        ELSE IF user.getRole() == "Administrator" THEN
            adminMenu(cast user to Administrator*)
        END IF
    END LOOP
END FUNCTION
```

## SLMS::memberMenu(member)
```
FUNCTION memberMenu(member):
    LOOP:
        PRINT "1. Search books  2. Borrow  3. Return  4. Reserve  5. My Loans  6. Logout"
        INPUT choice
        SWITCH choice:
            CASE 1: INPUT query; PRINT catalogue.searchByTitle(query)
            CASE 2: INPUT bookId; member.borrowBook(*catalogue.findById(bookId))
            CASE 3: INPUT bookId; member.returnBook(*catalogue.findById(bookId))
            CASE 4: INPUT bookId; member.reserveBook(*catalogue.findById(bookId))
            CASE 5: member.displayInfo()
            CASE 6: RETURN
        END SWITCH
    END LOOP
END FUNCTION
```
