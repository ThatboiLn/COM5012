# Smart Library Management System (SLMS)

**Module:** COM5012 Object Oriented Programming — CW1  
**Language:** C++

## Overview

The SLMS is a command-line application simulating a library management system for members, librarians, and administrators. It demonstrates core OOP principles:

- **Inheritance** — `Member`, `Librarian`, and `Administrator` all derive from the abstract `User` base class.
- **Encapsulation** — All class data is private/protected; access is controlled through public interfaces.
- **Polymorphism** — `displayInfo()` and `getRole()` are pure virtual in `User` and overridden in each subclass. The `UserManager` holds a `vector<User*>` and calls these methods polymorphically.

## Build & Run

```bash
cd src
g++ -std=c++17 -Wall -Wextra -o slms main.cpp
./slms
```

## Default Login Credentials (seeded on startup)

| Role          | ID | PIN  | Name  |
|---------------|----|------|-------|
| Member        | 1  | 1234 | Alice |
| Member        | 2  | 2345 | Bob   |
| Librarian     | 10 | 9999 | Carol |
| Administrator | 20 | 0000 | Dave  |

## File Structure

```
src/
  slms.h       — Book, User base class, utilities
  users.h      — Member, Librarian, Administrator
  catalogue.h  — Catalogue, UserManager
  main.cpp     — SLMS app class and entry point
wiki/
  Requirements.md
  Class-Diagram.md
  Pseudo-code.md
  Test-Strategy.md
```

## Wiki

Full design documentation (requirements, class diagram, pseudo-code, test strategy) is available in the [wiki](../../wiki).
