#include "ConsoleUtils.h"
#include "AVL.cpp"
#include <iostream>
#include <filesystem>
#include <string>
using namespace std;
namespace fs = std::filesystem;

/*
    Explanation:
    The loadDirectory function recursively traverses the given directory path and its subdirectories. For each regular file found, it extracts the filename and full path, then inserts this information into the AVL tree index. This allows for efficient O(log n) search performance when looking up files by name later on.

*/
void loadDirectory(const fs::path& directoryPath, AVL& index) {
    if (!fs::is_directory(directoryPath)) {
        ConsoleUtils::setForegroundColor(BrightRed);
        cout << "  [!] Error: Not a valid directory path.\n";
        ConsoleUtils::setDefaultColor();
        return;
    }

    for (const auto& entry : fs::directory_iterator{ directoryPath }) {
        fs::path entryPath = entry.path();

        if (fs::is_regular_file(entryPath)) {
            string fname    = entryPath.filename().string();
            string fullPath = fs::absolute(entryPath).string();
            index.insert(fname, fullPath);
        }
        else if (fs::is_directory(entryPath)) {
            loadDirectory(entryPath, index);
        }
    }
}

/*
    Explanation:
    The refresh method first frees all existing nodes in the tree to clear the current index, then calls the loadDirectory function to repopulate the tree with files from the specified directory. This allows users to update the index if files have been added, removed, or changed since the last indexing operation.
*/
void AVL::refresh(string rootPath) {
    freeTree(root);
    root = nullptr;
    loadDirectory(rootPath, *this);
}

/*
    Explanation:
    The drawHeader function is responsible for displaying the application header with a styled rectangle and title.
*/
void drawHeader() {
    ConsoleUtils::setForegroundColor(BrightCyan);
    ConsoleUtils::DrawDoubleLineRectangle(1, 1, 50, 4);
    ConsoleUtils::moveCursor(14, 2);
    ConsoleUtils::setForegroundColor(BrightWhite);
    cout << "Turbo Path  -  Path Finder";
    ConsoleUtils::moveCursor(11, 3);
    ConsoleUtils::setForegroundColor(Cyan);
    cout << "AVL-Powered O(logn) File Search";
    ConsoleUtils::moveCursor(35, 5);
    ConsoleUtils::setForegroundColor(BrightYellow);
    cout << "By: Jawad Ahmed";
    ConsoleUtils::moveCursor(1, 8);
    ConsoleUtils::setDefaultColor();
}

/*
    Explanation:
    The drawDivider function prints a horizontal divider line to visually separate different sections of the console output, enhancing readability.
*/
void drawDivider() {
    ConsoleUtils::setForegroundColor(BrightBlack);
    cout << "  -----------------------------------------------------------------\n";
    ConsoleUtils::setDefaultColor();
}

/*
    Explanation:
    The printHelp function displays a list of available commands and their descriptions to the user.
*/
void printHelp() {
    ConsoleUtils::setForegroundColor(BrightBlack);
    cout << "  Commands:  (filename) search  |  refresh - re-index  |  exit - quit\n";
    ConsoleUtils::setDefaultColor();
}

/*
    Explanation:
    The printResult function displays the search results for a given filename, showing all matching file paths or an error message if no matches are found.
*/
void printResult(node* result, string fname) {
    if (result == nullptr) {
        ConsoleUtils::setForegroundColor(BrightRed);
        cout << "\n  [!] File not found: " << fname << "\n\n";
        ConsoleUtils::setDefaultColor();
        return;
    }

    ConsoleUtils::setForegroundColor(BrightGreen);
    cout << "\n  [+] Found " << result->paths.size() << " match(es) for \"" << fname << "\":\n";
    ConsoleUtils::setDefaultColor();

    for (int i = 0; i < result->paths.size(); i++) {
        ConsoleUtils::setForegroundColor(BrightYellow);
        cout << "      [" << i + 1 << "] ";
        ConsoleUtils::setForegroundColor(White);
        cout << result->paths[i] << "\n";
    }
    cout << "\n";
    ConsoleUtils::setDefaultColor();
}

/*
    Explanation:
    The main function initializes the application, prompts the user for a root directory path, indexes the files in that directory, and then enters a loop to handle user commands for searching or refreshing the index.
*/
int main() {
    
    ConsoleUtils::enableVirtualTerminal();
    ConsoleUtils::clearConsole();
    ConsoleUtils::hideCursor();
    
    drawHeader();

    ConsoleUtils::setForegroundColor(BrightWhite);
    cout << "  Enter root directory path to index:\n";
    
    ConsoleUtils::setForegroundColor(BrightMagenta);
    string rootPath = getValidInput<string>("  >> ");
    cout << "\n";
    
    ConsoleUtils::setForegroundColor(Yellow);
    cout << "  [~] Indexing... please wait.\n";
    ConsoleUtils::setDefaultColor();

    AVL index;
    loadDirectory(rootPath, index);

    if (index.isEmpty()) {
        ConsoleUtils::setForegroundColor(BrightRed);
        cout << "  [!] No files found in that directory. Exiting.\n";
        ConsoleUtils::setDefaultColor();
        ConsoleUtils::showCursor();
        system("pause>0");
        return 1;
    }

    ConsoleUtils::setForegroundColor(BrightGreen);
    cout << "  [+] Indexing complete. Ready to search.\n\n";
    ConsoleUtils::setDefaultColor();

    drawDivider();
    printHelp();
    drawDivider();

    
    ConsoleUtils::showCursor();
    string query;
    while (true) {
        ConsoleUtils::setForegroundColor(BrightCyan);
        cout << "\n  Search >> ";
        ConsoleUtils::setDefaultColor();
        cin >> query;

        if (query == "exit" || query == "quit") break;

        if (query == "refresh" || query == "re-index") {
            ConsoleUtils::setForegroundColor(Yellow);
            cout << "\n  [*] Refreshing index...\n";
            ConsoleUtils::setDefaultColor();
            index.refresh(rootPath);
            ConsoleUtils::setForegroundColor(BrightGreen);
            cout << "  [+] Index refreshed.\n";
            ConsoleUtils::setDefaultColor();
            continue;
        }

        printResult(index.search(query), query);
    }

    ConsoleUtils::setForegroundColor(BrightBlack);
    cout << "\n  Allah Hafiz.\n\n";
    ConsoleUtils::setDefaultColor();

    system("pause>0");
    return 0;
}