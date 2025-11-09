#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

using namespace std;

void listFiles(const fs::path& path) {
    cout << "\nCurrent directory: " << path << endl;
    try {
        for (const auto& entry : fs::directory_iterator(path)) {
            cout << (entry.is_directory() ? "[DIR] " : "      ") << entry.path().filename().string() << endl;
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "Error listing files: " << e.what() << endl;
    }
}

void createFile(const fs::path& path) {
    string filename;
    cout << "Enter file name: ";
    getline(cin, filename);
    ofstream file(path / filename);
    if (file) {
        cout << "✅ File created successfully.\n";
    } else {
        cerr << "❌ Error creating file.\n";
    }
}

void deleteFile(const fs::path& path) {
    string filename;
    cout << "Enter file name to delete: ";
    getline(cin, filename);
    try {
        if (fs::remove(path / filename))
            cout << "✅ File deleted successfully.\n";
        else
            cerr << "❌ File not found.\n";
    } catch (const fs::filesystem_error& e) {
        cerr << "Error deleting file: " << e.what() << endl;
    }
}

void copyFile(const fs::path& path) {
    string source, destination;
    cout << "Enter source file name: ";
    getline(cin, source);
    cout << "Enter destination file name: ";
    getline(cin, destination);
    try {
        fs::copy(path / source, path / destination, fs::copy_options::overwrite_existing);
        cout << "✅ File copied successfully.\n";
    } catch (const fs::filesystem_error& e) {
        cerr << "Error copying file: " << e.what() << endl;
    }
}

void moveFile(const fs::path& path) {
    string source, destination;
    cout << "Enter source file name: ";
    getline(cin, source);
    cout << "Enter destination path: ";
    getline(cin, destination);
    try {
        fs::rename(path / source, path / destination);
        cout << "✅ File moved successfully.\n";
    } catch (const fs::filesystem_error& e) {
        cerr << "Error moving file: " << e.what() << endl;
    }
}

void changeDirectory(fs::path& path) {
    string dir;
    cout << "Enter directory path: ";
    getline(cin, dir);
    fs::path newPath = path / dir;
    if (fs::exists(newPath) && fs::is_directory(newPath)) {
        path = fs::canonical(newPath);
        cout << "📂 Changed directory to: " << path << endl;
    } else {
        cerr << "❌ Invalid directory.\n";
    }
}

void searchFile(const fs::path& path) {
    string filename;
    cout << "Enter file name to search: ";
    getline(cin, filename);
    bool found = false;
    for (const auto& entry : fs::recursive_directory_iterator(path)) {
        if (entry.path().filename() == filename) {
            cout << "✅ Found: " << entry.path() << endl;
            found = true;
        }
    }
    if (!found) cout << "❌ File not found.\n";
}

void filePermissions(const fs::path& path) {
    string filename;
    cout << "Enter file name: ";
    getline(cin, filename);
    fs::path filePath = path / filename;
    if (fs::exists(filePath)) {
        auto perms = fs::status(filePath).permissions();
        cout << "Permissions for " << filePath.filename() << ": ";
        cout << ((perms & fs::perms::owner_read) != fs::perms::none ? "r" : "-");
        cout << ((perms & fs::perms::owner_write) != fs::perms::none ? "w" : "-");
        cout << ((perms & fs::perms::owner_exec) != fs::perms::none ? "x" : "-");
        cout << endl;
    } else {
        cerr << "❌ File not found.\n";
    }
}

int main() {
    fs::path currentPath = fs::current_path();
    int choice;

    while (true) {
        cout << "\n==== Console File Explorer ====\n";
        cout << "1. List files in directory\n";
        cout << "2. Change directory\n";
        cout << "3. Create file\n";
        cout << "4. Delete file\n";
        cout << "5. Copy file\n";
        cout << "6. Move file\n";
        cout << "7. Search file\n";
        cout << "8. File permissions\n";
        cout << "9. Exit\n";
        cout << "---------------------------------\n";
        cout << "Enter your choice: ";

        string input;
        getline(cin, input);

        if (input.empty()) continue;
        choice = stoi(input);

        switch (choice) {
            case 1: listFiles(currentPath); break;
            case 2: changeDirectory(currentPath); break;
            case 3: createFile(currentPath); break;
            case 4: deleteFile(currentPath); break;
            case 5: copyFile(currentPath); break;
            case 6: moveFile(currentPath); break;
            case 7: searchFile(currentPath); break;
            case 8: filePermissions(currentPath); break;
            case 9:
                cout << "👋 Exiting File Explorer..." << endl;
                return 0;
            default:
                cerr << "❌ Invalid choice.\n";
        }
    }
}
