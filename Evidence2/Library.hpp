//
//  Library.hpp
//  LibraryManagement
//
//  Created by Developer on 10/06/25.
//

#ifndef Library_hpp
#define Library_hpp

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <memory>
#include <chrono>
#include <random>
#include <iomanip>
#include <queue>
#include <set>

// Forward declarations
class Book;
class Member;
class Loan;
class Reservation;
class Fine;
class Librarian;

enum class BookStatus {
    AVAILABLE,
    BORROWED,
    RESERVED,
    DAMAGED,
    LOST,
    PROCESSING
};

enum class MemberType {
    STUDENT,
    FACULTY,
    STAFF,
    PUBLIC,
    PREMIUM
};

enum class LoanStatus {
    ACTIVE,
    RETURNED,
    OVERDUE,
    RENEWED,
    LOST
};

struct Date {
    int day, month, year;
    
    Date() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto tm = *std::localtime(&time_t);
        day = tm.tm_mday;
        month = tm.tm_mon + 1;
        year = tm.tm_year + 1900;
    }
    
    Date(int d, int m, int y) : day(d), month(m), year(y) {}
    
    std::string toString() const {
        return std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year);
    }
    
    bool isAfter(const Date& other) const {
        if (year != other.year) return year > other.year;
        if (month != other.month) return month > other.month;
        return day > other.day;
    }
    
    int daysDifference(const Date& other) const {
        // Simplified calculation
        return abs((year * 365 + month * 30 + day) - (other.year * 365 + other.month * 30 + other.day));
    }
    
    Date addDays(int days) const {
        // Simplified - just add to day (in real implementation would handle month/year overflow)
        return Date(day + days, month, year);
    }
};

class Book {
private:
    static int nextBookId;
    int bookId;
    std::string isbn;
    std::string title;
    std::string author;
    std::string publisher;
    int publicationYear;
    std::string genre;
    int totalCopies;
    int availableCopies;
    BookStatus status;
    Date dateAdded;
    double price;
    std::string location;
    int totalLoans;
    double averageRating;
    std::vector<std::string> keywords;
    
public:
    Book(const std::string& isbnCode, const std::string& bookTitle, const std::string& bookAuthor,
         const std::string& pub, int pubYear, const std::string& bookGenre, int copies = 1)
        : bookId(nextBookId++), isbn(isbnCode), title(bookTitle), author(bookAuthor),
          publisher(pub), publicationYear(pubYear), genre(bookGenre), totalCopies(copies),
          availableCopies(copies), status(BookStatus::AVAILABLE), price(0.0),
          totalLoans(0), averageRating(0.0) {
        dateAdded = Date();
        location = "Section-" + std::to_string((bookId % 10) + 1);
    }
    
    ~Book() {}
    
    bool borrowCopy() {
        if (availableCopies > 0 && status == BookStatus::AVAILABLE) {
            availableCopies--;
            totalLoans++;
            if (availableCopies == 0) {
                status = BookStatus::BORROWED;
            }
            return true;
        }
        return false;
    }
    
    void returnCopy() {
        if (availableCopies < totalCopies) {
            availableCopies++;
            if (status == BookStatus::BORROWED && availableCopies > 0) {
                status = BookStatus::AVAILABLE;
            }
        }
    }
    
    void addCopies(int copies) {
        if (copies > 0) {
            totalCopies += copies;
            availableCopies += copies;
            if (status == BookStatus::BORROWED && availableCopies > 0) {
                status = BookStatus::AVAILABLE;
            }
        }
    }
    
    void removeCopies(int copies) {
        if (copies > 0 && copies <= availableCopies) {
            totalCopies -= copies;
            availableCopies -= copies;
            if (availableCopies == 0) {
                status = BookStatus::BORROWED;
            }
        }
    }
    
    bool isAvailable() const {
        return availableCopies > 0 && (status == BookStatus::AVAILABLE || status == BookStatus::RESERVED);
    }
    
    void addRating(double rating) {
        if (rating >= 1.0 && rating <= 5.0) {
            // Simplified average calculation
            averageRating = (averageRating + rating) / 2;
        }
    }
    
    void addKeyword(const std::string& keyword) {
        if (std::find(keywords.begin(), keywords.end(), keyword) == keywords.end()) {
            keywords.push_back(keyword);
        }
    }
    
    bool matchesSearch(const std::string& query) const {
        std::string lowerQuery = query;
        std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);
        
        std::string lowerTitle = title;
        std::transform(lowerTitle.begin(), lowerTitle.end(), lowerTitle.begin(), ::tolower);
        
        std::string lowerAuthor = author;
        std::transform(lowerAuthor.begin(), lowerAuthor.end(), lowerAuthor.begin(), ::tolower);
        
        return lowerTitle.find(lowerQuery) != std::string::npos ||
               lowerAuthor.find(lowerQuery) != std::string::npos ||
               isbn.find(query) != std::string::npos ||
               genre.find(lowerQuery) != std::string::npos;
    }
    
    double getPopularityScore() const {
        return totalLoans * 0.7 + averageRating * 0.3;
    }
    
    // Getters
    int getId() const { return bookId; }
    std::string getIsbn() const { return isbn; }
    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
    std::string getPublisher() const { return publisher; }
    int getPublicationYear() const { return publicationYear; }
    std::string getGenre() const { return genre; }
    int getTotalCopies() const { return totalCopies; }
    int getAvailableCopies() const { return availableCopies; }
    BookStatus getStatus() const { return status; }
    Date getDateAdded() const { return dateAdded; }
    double getPrice() const { return price; }
    std::string getLocation() const { return location; }
    int getTotalLoans() const { return totalLoans; }
    double getAverageRating() const { return averageRating; }
    std::vector<std::string> getKeywords() const { return keywords; }
    
    // Setters
    void setTitle(const std::string& bookTitle) { title = bookTitle; }
    void setAuthor(const std::string& bookAuthor) { author = bookAuthor; }
    void setPublisher(const std::string& pub) { publisher = pub; }
    void setPublicationYear(int year) { publicationYear = year; }
    void setGenre(const std::string& bookGenre) { genre = bookGenre; }
    void setStatus(BookStatus bookStatus) { status = bookStatus; }
    void setPrice(double bookPrice) { price = bookPrice; }
    void setLocation(const std::string& loc) { location = loc; }
    
    std::string getStatusString() const {
        switch (status) {
            case BookStatus::AVAILABLE: return "Available";
            case BookStatus::BORROWED: return "Borrowed";
            case BookStatus::RESERVED: return "Reserved";
            case BookStatus::DAMAGED: return "Damaged";
            case BookStatus::LOST: return "Lost";
            case BookStatus::PROCESSING: return "Processing";
            default: return "Unknown";
        }
    }
    
    void printBookInfo() const {
        std::cout << "Book [" << bookId << "] " << title << std::endl;
        std::cout << "  Author: " << author << std::endl;
        std::cout << "  ISBN: " << isbn << std::endl;
        std::cout << "  Publisher: " << publisher << " (" << publicationYear << ")" << std::endl;
        std::cout << "  Genre: " << genre << std::endl;
        std::cout << "  Copies: " << availableCopies << "/" << totalCopies << std::endl;
        std::cout << "  Status: " << getStatusString() << std::endl;
        std::cout << "  Location: " << location << std::endl;
        std::cout << "  Loans: " << totalLoans << " | Rating: " 
                  << std::fixed << std::setprecision(1) << averageRating << "/5.0" << std::endl;
        if (price > 0) {
            std::cout << "  Price: $" << std::fixed << std::setprecision(2) << price << std::endl;
        }
    }
    
    void printCompactInfo() const {
        std::cout << std::setw(5) << bookId << " | " 
                  << std::setw(30) << title.substr(0, 30) << " | "
                  << std::setw(20) << author.substr(0, 20) << " | "
                  << std::setw(4) << availableCopies << "/" << totalCopies << " | "
                  << std::setw(12) << getStatusString() << std::endl;
    }
};

int Book::nextBookId = 1;

class Member {
private:
    static int nextMemberId;
    int memberId;
    std::string firstName;
    std::string lastName;
    std::string email;
    std::string phone;
    std::string address;
    MemberType memberType;
    Date joinDate;
    Date expiryDate;
    bool isActive;
    int maxLoans;
    int currentLoans;
    double totalFines;
    double paidFines;
    int totalBooksRead;
    std::vector<std::string> favoriteGenres;
    
public:
    Member(const std::string& first, const std::string& last, const std::string& emailAddr,
           const std::string& phoneNum, MemberType type)
        : memberId(nextMemberId++), firstName(first), lastName(last), email(emailAddr),
          phone(phoneNum), memberType(type), isActive(true), currentLoans(0),
          totalFines(0.0), paidFines(0.0), totalBooksRead(0) {
        joinDate = Date();
        expiryDate = joinDate.addDays(365); // 1 year membership
        
        // Set max loans based on member type
        switch (type) {
            case MemberType::STUDENT: maxLoans = 5; break;
            case MemberType::FACULTY: maxLoans = 10; break;
            case MemberType::STAFF: maxLoans = 7; break;
            case MemberType::PUBLIC: maxLoans = 3; break;
            case MemberType::PREMIUM: maxLoans = 15; break;
            default: maxLoans = 3; break;
        }
    }
    
    ~Member() {}
    
    bool canBorrow() const {
        return isActive && currentLoans < maxLoans && (totalFines - paidFines) < 50.0;
    }
    
    void borrowBook() {
        if (canBorrow()) {
            currentLoans++;
        }
    }
    
    void returnBook() {
        if (currentLoans > 0) {
            currentLoans--;
            totalBooksRead++;
        }
    }
    
    void addFine(double amount) {
        if (amount > 0) {
            totalFines += amount;
        }
    }
    
    void payFine(double amount) {
        if (amount > 0) {
            paidFines += amount;
            if (paidFines > totalFines) {
                paidFines = totalFines;
            }
        }
    }
    
    double getOutstandingFines() const {
        return totalFines - paidFines;
    }
    
    bool isExpired() const {
        Date today;
        return today.isAfter(expiryDate);
    }
    
    void renewMembership(int days = 365) {
        Date today;
        expiryDate = today.addDays(days);
    }
    
    void addFavoriteGenre(const std::string& genre) {
        if (std::find(favoriteGenres.begin(), favoriteGenres.end(), genre) == favoriteGenres.end()) {
            favoriteGenres.push_back(genre);
        }
    }
    
    bool isFavoriteGenre(const std::string& genre) const {
        return std::find(favoriteGenres.begin(), favoriteGenres.end(), genre) != favoriteGenres.end();
    }
    
    std::string getFullName() const {
        return firstName + " " + lastName;
    }
    
    // Getters
    int getId() const { return memberId; }
    std::string getFirstName() const { return firstName; }
    std::string getLastName() const { return lastName; }
    std::string getEmail() const { return email; }
    std::string getPhone() const { return phone; }
    std::string getAddress() const { return address; }
    MemberType getMemberType() const { return memberType; }
    Date getJoinDate() const { return joinDate; }
    Date getExpiryDate() const { return expiryDate; }
    bool getIsActive() const { return isActive; }
    int getMaxLoans() const { return maxLoans; }
    int getCurrentLoans() const { return currentLoans; }
    double getTotalFines() const { return totalFines; }
    double getPaidFines() const { return paidFines; }
    int getTotalBooksRead() const { return totalBooksRead; }
    std::vector<std::string> getFavoriteGenres() const { return favoriteGenres; }
    
    // Setters
    void setFirstName(const std::string& first) { firstName = first; }
    void setLastName(const std::string& last) { lastName = last; }
    void setEmail(const std::string& emailAddr) { email = emailAddr; }
    void setPhone(const std::string& phoneNum) { phone = phoneNum; }
    void setAddress(const std::string& addr) { address = addr; }
    void setIsActive(bool active) { isActive = active; }
    
    std::string getMemberTypeString() const {
        switch (memberType) {
            case MemberType::STUDENT: return "Student";
            case MemberType::FACULTY: return "Faculty";
            case MemberType::STAFF: return "Staff";
            case MemberType::PUBLIC: return "Public";
            case MemberType::PREMIUM: return "Premium";
            default: return "Unknown";
        }
    }
    
    void printMemberInfo() const {
        std::cout << "Member [" << memberId << "] " << getFullName() << std::endl;
        std::cout << "  Type: " << getMemberTypeString() << std::endl;
        std::cout << "  Email: " << email << " | Phone: " << phone << std::endl;
        std::cout << "  Joined: " << joinDate.toString() << " | Expires: " << expiryDate.toString() << std::endl;
        std::cout << "  Status: " << (isActive ? "Active" : "Inactive") << std::endl;
        std::cout << "  Loans: " << currentLoans << "/" << maxLoans << std::endl;
        std::cout << "  Books Read: " << totalBooksRead << std::endl;
        std::cout << "  Fines: $" << std::fixed << std::setprecision(2) << getOutstandingFines() 
                  << " outstanding ($" << totalFines << " total)" << std::endl;
        if (!favoriteGenres.empty()) {
            std::cout << "  Favorite Genres: ";
            for (size_t i = 0; i < favoriteGenres.size(); ++i) {
                std::cout << favoriteGenres[i];
                if (i < favoriteGenres.size() - 1) std::cout << ", ";
            }
            std::cout << std::endl;
        }
    }
};

int Member::nextMemberId = 1;

class Loan {
private:
    static int nextLoanId;
    int loanId;
    Member* member;
    Book* book;
    Date loanDate;
    Date dueDate;
    Date returnDate;
    LoanStatus status;
    double fineAmount;
    bool isRenewed;
    int renewalCount;
    
public:
    Loan(Member* memberPtr, Book* bookPtr, int loanDays = 14)
        : loanId(nextLoanId++), member(memberPtr), book(bookPtr), 
          status(LoanStatus::ACTIVE), fineAmount(0.0), isRenewed(false), renewalCount(0) {
        loanDate = Date();
        dueDate = loanDate.addDays(loanDays);
    }
    
    ~Loan() {}
    
    void returnBook() {
        returnDate = Date();
        status = LoanStatus::RETURNED;
        
        if (book) {
            book->returnCopy();
        }
        
        if (member) {
            member->returnBook();
        }
        
        // Calculate fine if overdue
        if (returnDate.isAfter(dueDate)) {
            int overdueDays = returnDate.daysDifference(dueDate);
            fineAmount = overdueDays * 0.50; // $0.50 per day
            if (member) {
                member->addFine(fineAmount);
            }
        }
    }
    
    bool renewLoan(int additionalDays = 14) {
        if (renewalCount < 2 && !isOverdue() && status == LoanStatus::ACTIVE) {
            dueDate = dueDate.addDays(additionalDays);
            isRenewed = true;
            renewalCount++;
            status = LoanStatus::RENEWED;
            return true;
        }
        return false;
    }
    
    bool isOverdue() const {
        Date today;
        return today.isAfter(dueDate) && status == LoanStatus::ACTIVE;
    }
    
    void markAsLost() {
        status = LoanStatus::LOST;
        if (book) {
            book->setStatus(BookStatus::LOST);
            // Add replacement cost as fine
            if (member) {
                member->addFine(book->getPrice() > 0 ? book->getPrice() : 25.0);
            }
        }
    }
    
    int getDaysOverdue() const {
        if (!isOverdue()) return 0;
        Date today;
        return today.daysDifference(dueDate);
    }
    
    int getDaysUntilDue() const {
        Date today;
        if (today.isAfter(dueDate)) return 0;
        return dueDate.daysDifference(today);
    }
    
    // Getters
    int getId() const { return loanId; }
    Member* getMember() const { return member; }
    Book* getBook() const { return book; }
    Date getLoanDate() const { return loanDate; }
    Date getDueDate() const { return dueDate; }
    Date getReturnDate() const { return returnDate; }
    LoanStatus getStatus() const { return status; }
    double getFineAmount() const { return fineAmount; }
    bool getIsRenewed() const { return isRenewed; }
    int getRenewalCount() const { return renewalCount; }
    
    std::string getStatusString() const {
        switch (status) {
            case LoanStatus::ACTIVE: return isOverdue() ? "OVERDUE" : "Active";
            case LoanStatus::RETURNED: return "Returned";
            case LoanStatus::OVERDUE: return "Overdue";
            case LoanStatus::RENEWED: return "Renewed";
            case LoanStatus::LOST: return "Lost";
            default: return "Unknown";
        }
    }
    
    void printLoanInfo() const {
        std::cout << "Loan [" << loanId << "]" << std::endl;
        std::cout << "  Member: " << (member ? member->getFullName() : "None") 
                  << " (" << (member ? member->getId() : 0) << ")" << std::endl;
        std::cout << "  Book: " << (book ? book->getTitle() : "None") 
                  << " (" << (book ? book->getId() : 0) << ")" << std::endl;
        std::cout << "  Loan Date: " << loanDate.toString() << std::endl;
        std::cout << "  Due Date: " << dueDate.toString() << std::endl;
        if (status == LoanStatus::RETURNED) {
            std::cout << "  Return Date: " << returnDate.toString() << std::endl;
        }
        std::cout << "  Status: " << getStatusString() << std::endl;
        if (isRenewed) {
            std::cout << "  Renewals: " << renewalCount << std::endl;
        }
        if (fineAmount > 0) {
            std::cout << "  Fine: $" << std::fixed << std::setprecision(2) << fineAmount << std::endl;
        }
        if (isOverdue()) {
            std::cout << "  Days Overdue: " << getDaysOverdue() << std::endl;
        }
    }
};

int Loan::nextLoanId = 1;

class LibrarySystem {
private:
    std::vector<std::unique_ptr<Book>> books;
    std::vector<std::unique_ptr<Member>> members;
    std::vector<std::unique_ptr<Loan>> loans;
    std::vector<std::unique_ptr<Loan>> loanHistory;
    std::string libraryName;
    std::string address;
    int totalBooks;
    int totalMembers;
    int totalActiveLoans;
    
public:
    LibrarySystem(const std::string& name, const std::string& addr)
        : libraryName(name), address(addr), totalBooks(0), totalMembers(0), totalActiveLoans(0) {}
    
    ~LibrarySystem() {}
    
    void addBook(std::unique_ptr<Book> book) {
        if (book) {
            books.push_back(std::move(book));
            totalBooks++;
        }
    }
    
    void addMember(std::unique_ptr<Member> member) {
        if (member) {
            members.push_back(std::move(member));
            totalMembers++;
        }
    }
    
    Book* findBook(int bookId) {
        auto it = std::find_if(books.begin(), books.end(),
            [bookId](const std::unique_ptr<Book>& book) {
                return book->getId() == bookId;
            });
        return (it != books.end()) ? it->get() : nullptr;
    }
    
    Member* findMember(int memberId) {
        auto it = std::find_if(members.begin(), members.end(),
            [memberId](const std::unique_ptr<Member>& member) {
                return member->getId() == memberId;
            });
        return (it != members.end()) ? it->get() : nullptr;
    }
    
    std::vector<Book*> searchBooks(const std::string& query) {
        std::vector<Book*> results;
        for (const auto& book : books) {
            if (book->matchesSearch(query)) {
                results.push_back(book.get());
            }
        }
        return results;
    }
    
    std::vector<Book*> getBooksByGenre(const std::string& genre) {
        std::vector<Book*> results;
        for (const auto& book : books) {
            if (book->getGenre() == genre) {
                results.push_back(book.get());
            }
        }
        return results;
    }
    
    bool borrowBook(int memberId, int bookId) {
        Member* member = findMember(memberId);
        Book* book = findBook(bookId);
        
        if (!member || !book) {
            std::cout << "Error: Member or book not found." << std::endl;
            return false;
        }
        
        if (!member->canBorrow()) {
            std::cout << "Error: Member cannot borrow more books." << std::endl;
            return false;
        }
        
        if (!book->isAvailable()) {
            std::cout << "Error: Book is not available." << std::endl;
            return false;
        }
        
        // Create loan
        auto loan = std::make_unique<Loan>(member, book);
        book->borrowCopy();
        member->borrowBook();
        
        loans.push_back(std::move(loan));
        totalActiveLoans++;
        
        std::cout << "Book borrowed successfully!" << std::endl;
        return true;
    }
    
    bool returnBook(int loanId) {
        auto it = std::find_if(loans.begin(), loans.end(),
            [loanId](const std::unique_ptr<Loan>& loan) {
                return loan->getId() == loanId;
            });
        
        if (it != loans.end()) {
            (*it)->returnBook();
            loanHistory.push_back(std::move(*it));
            loans.erase(it);
            totalActiveLoans--;
            std::cout << "Book returned successfully!" << std::endl;
            return true;
        }
        
        std::cout << "Error: Loan not found." << std::endl;
        return false;
    }
    
    void checkOverdueBooks() {
        std::cout << "=== Overdue Books ===" << std::endl;
        bool hasOverdue = false;
        
        for (const auto& loan : loans) {
            if (loan->isOverdue()) {
                hasOverdue = true;
                loan->printLoanInfo();
                std::cout << std::endl;
            }
        }
        
        if (!hasOverdue) {
            std::cout << "No overdue books found." << std::endl;
        }
    }
    
    void printLibraryStats() {
        std::cout << "=== " << libraryName << " Statistics ===" << std::endl;
        std::cout << "Address: " << address << std::endl;
        std::cout << "Total Books: " << totalBooks << std::endl;
        std::cout << "Total Members: " << totalMembers << std::endl;
        std::cout << "Active Loans: " << totalActiveLoans << std::endl;
        std::cout << "Books in History: " << loanHistory.size() << std::endl;
        
        // Calculate available books
        int availableBooks = 0;
        for (const auto& book : books) {
            availableBooks += book->getAvailableCopies();
        }
        std::cout << "Available Copies: " << availableBooks << std::endl;
        
        // Most popular books
        std::cout << "\n=== Most Popular Books ===" << std::endl;
        std::vector<Book*> popularBooks;
        for (const auto& book : books) {
            popularBooks.push_back(book.get());
        }
        
        std::sort(popularBooks.begin(), popularBooks.end(),
            [](Book* a, Book* b) {
                return a->getTotalLoans() > b->getTotalLoans();
            });
        
        int count = 0;
        for (Book* book : popularBooks) {
            if (count >= 5) break;
            if (book->getTotalLoans() > 0) {
                std::cout << (count + 1) << ". " << book->getTitle() 
                          << " (" << book->getTotalLoans() << " loans)" << std::endl;
                count++;
            }
        }
    }
    
    void printAllBooks() {
        std::cout << "=== All Books ===" << std::endl;
        std::cout << std::setw(5) << "ID" << " | " 
                  << std::setw(30) << "Title" << " | "
                  << std::setw(20) << "Author" << " | "
                  << std::setw(7) << "Copies" << " | "
                  << std::setw(12) << "Status" << std::endl;
        std::cout << std::string(80, '-') << std::endl;
        
        for (const auto& book : books) {
            book->printCompactInfo();
        }
    }
    
    void printAllMembers() {
        std::cout << "=== All Members ===" << std::endl;
        for (const auto& member : members) {
            std::cout << "[" << member->getId() << "] " << member->getFullName() 
                      << " (" << member->getMemberTypeString() << ") - "
                      << member->getCurrentLoans() << "/" << member->getMaxLoans() << " loans" << std::endl;
        }
    }
    
    void printActiveLoans() {
        std::cout << "=== Active Loans ===" << std::endl;
        if (loans.empty()) {
            std::cout << "No active loans." << std::endl;
            return;
        }
        
        for (const auto& loan : loans) {
            std::cout << "[" << loan->getId() << "] " 
                      << loan->getMember()->getFullName() << " - "
                      << loan->getBook()->getTitle() << " (Due: " 
                      << loan->getDueDate().toString() << ")" << std::endl;
        }
    }
};