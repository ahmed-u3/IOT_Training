#include <iostream>
#include <string>
#include <vector>

class Book {
private:
    std::string title;
    std::string author;
    int book_id;

public:
    Book(std::string title, std::string author, int book_id)
        : title(title), author(author), book_id(book_id) {}

    int getBookID() const {
        return book_id;
    }

    void displayBookDetails() const {
        std::cout << "Book ID: " << book_id << std::endl;
        std::cout << "Title: " << title << std::endl;
        std::cout << "Author: " << author << std::endl;
    }
};

class Library {
private:
    std::vector<Book> books;

public:
    void addBook(const Book& book) {
        books.push_back(book);
        std::cout << "Book added successfully." << std::endl;
    }

    void removeBookByID(int book_id) {
        auto it = std::find_if(books.begin(), books.end(), [book_id](const Book& book) {
            return book.getBookID() == book_id;
        });

        if (it != books.end()) {
            books.erase(it);
            std::cout << "Book with ID " << book_id << " removed successfully." << std::endl;
        } else {
            std::cout << "Book with ID " << book_id << " not found in the library." << std::endl;
        }
    }

    void displayAllBooks() const {
        std::cout << "---- Library Books ----" << std::endl;
        for (const Book& book : books) {
            book.displayBookDetails();
            std::cout << "-----------------------" << std::endl;
        }
    }
};

// Example Usage:
int main() {
    Library library;

    // Add books to the library
    library.addBook(Book("Title1", "Author1", 101));
    library.addBook(Book("Title2", "Author2", 102));
    library.addBook(Book("Title3", "Author3", 103));

    // Display all books in the library
    library.displayAllBooks();

    // Remove a book by ID
    library.removeBookByID(102);

    // Display all books after removal
    library.displayAllBooks();

    return 0;
}