# Test Strategy — SLMS

## 1. Test Approach

Testing is conducted at the **unit level** (individual class methods) and **integration level** (multi-step user flows through the SLMS menu system). Tests are driven by piping input strings to the compiled binary and observing stdout, making them fully reproducible.

All test cases are linked to requirements.

---

## 2. Test Cases

### TC01 — Valid Member Login
**Requirement:** FR01  
**Input:** ID `1`, PIN `1234`  
**Expected:** `Welcome, Alice Smith (Member).`  
**Actual:** ✅ PASS — output matches exactly

---

### TC02 — Invalid PIN Rejected
**Requirement:** FR01, NFR03  
**Input:** ID `1`, PIN `9999`  
**Expected:** `[!] Invalid credentials. Try again.`  
**Actual:** ✅ PASS — system rejects without crashing

---

### TC03 — Non-existent User ID
**Requirement:** FR01, NFR03  
**Input:** ID `999`, PIN `1234`  
**Expected:** `[!] Invalid credentials. Try again.`  
**Actual:** ✅ PASS

---

### TC04 — Browse Catalogue
**Requirement:** FR02  
**Input:** Member login → option `1`  
**Expected:** All 5 seeded books displayed with title, author, ISBN, status  
**Actual:** ✅ PASS

---

### TC05 — Search by Title (partial, case-insensitive)
**Requirement:** FR02  
**Input:** Member login → option `2` → query `clean`  
**Expected:** `[2] "Clean Code" by Robert Martin`  
**Actual:** ✅ PASS — case-insensitive match works

---

### TC06 — Search by Author
**Requirement:** FR02  
**Input:** Member login → option `3` → query `stroustrup`  
**Expected:** `[4] "The C++ Programming Language"`  
**Actual:** ✅ PASS

---

### TC07 — Borrow Available Book
**Requirement:** FR03, FR06, FR07  
**Input:** Member login → option `4` → Book ID `1`  
**Expected:** `"The Pragmatic Programmer" borrowed. Due on day 15.`  
**Actual:** ✅ PASS — due date = currentDay(1) + 14 = 15

---

### TC08 — Borrow Already-Borrowed Book (edge case)
**Requirement:** FR03, NFR03  
**Setup:** Alice borrows book 1. Bob attempts to borrow book 1.  
**Expected:** `[!] "The Pragmatic Programmer" is not available (status: Borrowed).`  
**Actual:** ✅ PASS

---

### TC09 — Borrow Limit Enforced (edge case)
**Requirement:** FR03  
**Setup:** Alice borrows books 1, 2, 3, 4, 5. Attempts to borrow book 6.  
**Expected:** `[!] Borrow limit (5) reached.`  
**Actual:** ✅ PASS

---

### TC10 — Return a Borrowed Book
**Requirement:** FR04, FR06  
**Setup:** Alice borrows book 1, then returns it.  
**Expected:** `"The Pragmatic Programmer" returned successfully.` → active loans drop to 0  
**Actual:** ✅ PASS

---

### TC11 — Return Wrong Member's Book (edge case)
**Requirement:** FR04, NFR03  
**Setup:** Alice borrows book 1. Bob attempts to return book 1.  
**Expected:** `[!] You did not borrow this book.`  
**Actual:** ✅ PASS

---

### TC12 — Overdue Fine Applied on Return
**Requirement:** FR04, FR07  
**Setup:** Alice borrows book 1 on day 1 (due day 15). Admin advances to day 20. Alice returns book 1.  
**Expected:** `Book is 5 day(s) overdue. Fine: £1.00 added.`  
**Actual:** ✅ PASS — 5 days × £0.20 = £1.00

---

### TC13 — Reserve a Borrowed Book
**Requirement:** FR05  
**Setup:** Alice borrows book 1. Bob reserves book 1.  
**Expected:** `"The Pragmatic Programmer" reserved.`  
**Actual:** ✅ PASS

---

### TC14 — Reserve an Available Book (edge case)
**Requirement:** FR05, NFR03  
**Setup:** Book 2 is available. Bob tries to reserve it.  
**Expected:** `[!] Book is available — borrow it directly instead.`  
**Actual:** ✅ PASS

---

### TC15 — Reservation Expiry After 3 Days
**Requirement:** FR17  
**Setup:** Bob reserves book 1 on day 1. Admin advances 4 days. System ticks.  
**Expected:** `Reservation on "The Pragmatic Programmer" has expired.`  
**Actual:** ✅ PASS

---

### TC16 — Librarian Adds Book
**Requirement:** FR08  
**Setup:** Carol (Librarian) logs in → adds "New Book" by "Author X"  
**Expected:** `Book added: "New Book" (ID 6).`  
**Actual:** ✅ PASS

---

### TC17 — Librarian Removes Available Book
**Requirement:** FR09  
**Input:** Carol removes book ID 2 (available)  
**Expected:** `Book ID 2 removed.`  
**Actual:** ✅ PASS

---

### TC18 — Librarian Cannot Remove Borrowed Book (edge case)
**Requirement:** FR09, NFR03  
**Setup:** Alice borrows book 1. Carol tries to remove book 1.  
**Expected:** `[!] Cannot remove a book that is currently Borrowed.`  
**Actual:** ✅ PASS

---

### TC19 — Overdue Report
**Requirement:** FR11  
**Setup:** Alice borrows book 1 on day 1. Admin advances to day 20. Carol views report.  
**Expected:** Book 1 shown as overdue with `Days late: 5`  
**Actual:** ✅ PASS

---

### TC20 — Admin Creates Member
**Requirement:** FR13  
**Setup:** Dave (Admin) creates new member "Eve", PIN `5678`  
**Expected:** `Member created (ID 100).` Eve can log in with ID 100, PIN 5678  
**Actual:** ✅ PASS

---

### TC21 — Admin Sets Borrow Limit
**Requirement:** FR15  
**Setup:** Dave sets Alice's borrow limit to 2  
**Expected:** `Borrow limit for Alice Smith set to 2.` Alice cannot borrow a 3rd book.  
**Actual:** ✅ PASS

---

### TC22 — Invalid Menu Input (edge case)
**Requirement:** NFR03  
**Input:** Member types `abc` at menu  
**Expected:** `[!] Please enter a valid number.`  
**Actual:** ✅ PASS — no crash, re-prompts cleanly

---

## 3. Test Results Summary

| Total | Pass | Fail |
|-------|------|------|
| 22    | 22   | 0    |

All 22 test cases pass. Core functional paths (borrow, return, reserve, search, manage books, manage accounts) are verified. Edge cases tested include: invalid credentials, borrow limit enforcement, returning someone else's book, reserving an available book, removing a borrowed book, overdue fines, reservation expiry, and non-numeric menu input.
