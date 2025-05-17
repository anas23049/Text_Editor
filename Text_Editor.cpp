#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <conio.h>
#include<fstream>
using namespace std;
enum Mode { NORMAL, INSERT };
struct EditorStatus {
    Mode currentMode;
    size_t cursorLine;
    size_t cursorColumn;
    size_t totalLines;
    string lastCommand;
    EditorStatus() : currentMode(NORMAL), cursorLine(1), cursorColumn(1), totalLines(1), lastCommand("") {}
};

struct Node
{
    char data;
    Node* next;
    Node* prev;
    Node(char ch) : data(ch), next(nullptr), prev(nullptr) {}
};
class Line {
private:
    Node* head;
    Node* tail;
    Node* cursor;

public:
    class Iterator {
    private:
        Node* current;

    public:
        Iterator(Node* start = nullptr) : current(start) {}

        char operator*() const {
            return current->data;
        }

        Iterator& operator++() {
            if (current) current = current->next;
            return *this;
        }

        Iterator& operator--() {
            if (current) current = current->prev;
            return *this;
        }

        bool operator==(const Iterator& other) const {
            return current == other.current;
        }

        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
        Node* getNode() const
        {
            return current;
        }
    };

    Line() : head(nullptr), cursor(nullptr) {}

    Iterator begin() const {
        return Iterator(head);
    }

    Iterator end() const {
        return Iterator(nullptr);
    }
    char getFirstChar() const {
        if (head) {
            return head->data;
        }
        return '\0';
    }
    void deleteFirstChar() {
        if (head) {
            Node* temp = head;
            head = head->next;
            if (head) {
                head->prev = nullptr;
            }
            delete temp;
        }
    }
    
    size_t distance(Iterator start, Iterator end) const {
        size_t globalDistance = 0;

      
        Node* currentNode = start.getNode();

        while (currentNode != end.getNode()) {
            ++globalDistance;  
            currentNode = currentNode->next;
        }

        return globalDistance;
    }

    Iterator eraseChar(Iterator& it) {
        Node* nodeToDelete = it.getNode();
        if (!nodeToDelete) return Iterator(nullptr);


        if (nodeToDelete->prev) {
            nodeToDelete->prev->next = nodeToDelete->next;
        }
        else {

            head = nodeToDelete->next;
        }

        if (nodeToDelete->next) {
            nodeToDelete->next->prev = nodeToDelete->prev;
        }


        Node* nextNode = nodeToDelete->next;
        delete nodeToDelete;

        return Iterator(nextNode);
    }
    void insertCharAt(Iterator& it, char ch) {
        Node* newNode = new Node(ch);

        Node* currentNode = it.getNode();

        if (!currentNode) {

            Node* tail = getTail();
            if (tail) {
                tail->next = newNode;
                newNode->prev = tail;
            }
            else {

                head = newNode;
            }
        }
        else {

            newNode->next = currentNode;
            newNode->prev = currentNode->prev;

            if (currentNode->prev) {
                currentNode->prev->next = newNode;
            }
            else {

                head = newNode;
            }

            currentNode->prev = newNode;
        }

        it = Iterator(newNode);
    }

    void insertCharAtStart(const std::string& str) {
        for (char ch : str) {
            Node* newNode = new Node(ch);
            if (!head) {
                head = newNode;
                cursor = newNode;
            }
            else {
                newNode->next = head;
                head->prev = newNode;
                head = newNode;
            }
        }
    }
    size_t size() const {
        size_t count = 0;
        Node* temp = head;
        while (temp) {
            ++count;
            temp = temp->next;
        }
        return count;
    }
    std::string getContent() const {
        std::string content;
        Node* temp = head;
        while (temp) {
            content += temp->data;
            temp = temp->next;
        }
        return content;
    }

    void insertChar(char ch) {
        Node* newNode = new Node(ch);
        if (!head) {
            head = newNode;
            cursor = newNode;
        }
        else if (cursor == nullptr) {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
            cursor = newNode;
        }
        else {
            newNode->next = cursor->next;
            newNode->prev = cursor;
            if (cursor->next) {
                cursor->next->prev = newNode;
            }
            cursor->next = newNode;
            cursor = newNode;
        }
    }
    Node* getTail() {
        Node* temp = head;
        while (temp && temp->next) {
            temp = temp->next;
        }
        return temp;
    }
    void moveRight() {
        if (cursor && cursor->next)
        {
            cursor = cursor->next;
        }
        else if (cursor == getTail())
        {
            cursor = head;
        }
    }

    void moveLeft() {
        if (cursor && cursor->prev) {
            cursor = cursor->prev;
        }
        else if (cursor == head) {

            cursor = getTail();
        }
    }

    void resetCursor()
    {
        cursor = head;
    }

    void deleteToEndOfLine() {
        while (cursor && cursor->next) {
            Node* temp = cursor->next;
            cursor->next = temp->next;
            if (temp->next)
                temp->next->prev = cursor;
            delete temp;
        }
    }

    void deleteCharacterAtCursor()
    {
        if (cursor)
        {
            if (cursor == head) {
                head = cursor->next;
                if (head) head->prev = nullptr;
                delete cursor;
                cursor = head;
            }
            else
            {
                Node* temp = cursor;
                cursor->prev->next = cursor->next;
                if (cursor->next) cursor->next->prev = cursor->prev;
                cursor = cursor->next;
                delete temp;
            }
        }
    }

    void backspace()
    {
        if (cursor == head)
        {
            deleteCharacterAtCursor();

        }
        else if (cursor)
        {
            moveLeft();
            deleteCharacterAtCursor();
        }


    }

    bool isWordCharacter(char c) const {
        return isalnum(c);
    }

    void moveToStartOfLine() {
        cursor = head;
    }
    size_t length() const {
        size_t len = 0;
        Node* temp = head;
        while (temp) {
            len++;
            temp = temp->next;
        }
        return len;
    }
    void moveToEndOfLine() {
        while (cursor && cursor->next) {
            cursor = cursor->next;
        }
    }
    Node* getHead() {
        return head;
    }
    void moveToNextWord() {
        while (cursor && !isWordCharacter(cursor->data)) {
            cursor = cursor->next;
        }
        while (cursor && isWordCharacter(cursor->data)) {
            cursor = cursor->next;
        }
        while (cursor && !isWordCharacter(cursor->data)) {
            cursor = cursor->next;
        }
    }

    void moveToPreviousWord() {
        while (cursor && !isWordCharacter(cursor->data)) {
            cursor = cursor->prev;
        }
        while (cursor && isWordCharacter(cursor->data)) {
            cursor = cursor->prev;
        }
        if (cursor && !isWordCharacter(cursor->data)) {
            cursor = cursor->next;
        }
        if (cursor == nullptr && head) {
            cursor = head;
        }
    }

    void moveToWordEnd() {
        while (cursor && isWordCharacter(cursor->data)) {
            cursor = cursor->next;
        }
        if (cursor && cursor->prev && isWordCharacter(cursor->prev->data)) {
            cursor = cursor->prev;
        }
    }

    size_t getCursorColumn() const {
        size_t col = 1;
        Node* temp = head;
        while (temp && temp != cursor) {
            col++;
            temp = temp->next;
        }
        return col;
    }

    void display(bool displayCursor, size_t lineNumber) const {
        cout << lineNumber << " | ";
        Node* temp = head;
        while (temp) {
            if (displayCursor && temp == cursor) cout << "|";
            cout << temp->data;
            temp = temp->next;
        }
        if (displayCursor && cursor == nullptr) cout << "|";
        cout << endl;
    }

    bool isEmpty() const {
        return head == nullptr;
    }

};
class FileManager {
private:
    std::string currentFileName;
    bool modified = false;

public:
    FileManager() : modified(false) {}

    bool loadFile(const std::string& fileName, std::vector<Line>& lines) {
        std::ifstream file(fileName);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << fileName << std::endl;
            return false;
        }

        currentFileName = fileName;
        lines.clear();
        modified = false;

        std::string line;
        while (std::getline(file, line)) {
            Line newLine;
            for (char ch : line) {
                newLine.insertChar(ch);
            }
            lines.push_back(newLine);
        }

        file.close();
        return true;
    }

    bool saveFile(const std::string& fileName, std::vector<Line>& lines) {
        std::ofstream file(fileName);
        if (!file.is_open()) {
            std::cerr << "Error: Could not save file " << fileName << std::endl;
            return false;
        }

        currentFileName = fileName;


        for (size_t colno = 0; colno < lines.size(); ++colno) {
            auto currentLine = lines[colno].getHead();
            while (currentLine != nullptr) {
                file.put(currentLine->data);
                currentLine = currentLine->next;
            }
            file << std::endl;
        }

        modified = false;
        file.close();
        return true;
    }



    bool hasUnsavedChanges() const {
        return modified;
    }

    void markAsModified() {
        modified = true;
    }

    std::string getCurrentFileName() const {
        return currentFileName;
    }
};
class SearchEngine {
private:
    std::string lastPattern;
    int lastMatchLine;
    size_t lastMatchColumn;

public:
    SearchEngine() : lastMatchLine(0), lastMatchColumn(0) {}

    bool search(const std::string& pattern, std::vector<Line>& lines, int& currentLine)
    {
        lastPattern = pattern;

        for (int i = 0; i < lines.size(); ++i) {
            Line::Iterator it = lines[i].begin();
            size_t col = 0;

            while (it != lines[i].end()) {
                if (matchPattern(it, lines[i].end(), pattern)) {
                    lastMatchLine = i;
                    lastMatchColumn = col + 1;
                    currentLine = i;
                    moveCursorTo(lines[i], col + 1);
                    return true;
                }
                ++it;
                ++col;
            }
        }

        std::cout << "Pattern not found.\n";
        return false;
    }
    bool findNext(std::vector<Line>& lines, int& currentLine) {
        for (int i = lastMatchLine; i < lines.size(); ++i) {
            Line::Iterator it = lines[i].begin();
            size_t col = 0;

            if (i == lastMatchLine) {

                for (size_t j = 0; j < lastMatchColumn; ++j) ++it, ++col;
            }

            while (it != lines[i].end()) {
                if (matchPattern(it, lines[i].end(), lastPattern)) {
                    lastMatchLine = i;
                    lastMatchColumn = col + 1;
                    currentLine = i;
                    moveCursorTo(lines[i], col + 1);
                    return true;
                }
                ++it;
                ++col;
            }
        }

        std::cout << "No further occurrences found.\n";
        return false;
    }
    bool findPrevious(std::vector<Line>& lines, int& currentLine) {
        int globalCol = 0;  // To store the global column position across all lines

        // Start searching from the last match line
        for (int i = lastMatchLine; i >= 0; --i) {
            Line::Iterator it = lines[i].end(); // Start from the end of the current line
            size_t col = lines[i].size();       // Start from the last column in this line (0-based)

            if (i == lastMatchLine) {
                // Start from one column before the last match
                col = lastMatchColumn - 1;
                it = lines[i].begin();
                for (size_t j = 0; j < col; ++j) {
                    ++it; // Move iterator to the column before the last match
                }
            }

            // Traverse the line backwards
            while (col > 0) {
                --it;
                --col;

                // Calculate global column (position) from the beginning of the document
                globalCol++;

                if (matchPatternReverse(it, lines[i].begin(), lastPattern)) {
                    // Update match location
                    lastMatchLine = i;

                    // Use the 'distance' function to calculate the column number within the line
                    lastMatchColumn = lines[i].distance(lines[i].begin(), it) + 1; // Convert to 1-based index
                    currentLine = i;

                    // Move cursor to the found position
                    moveCursorTo(lines[i], lastMatchColumn);
                    return true;
                }
            }

            // Update global position for the next line
            globalCol += lines[i].size();  // Add the number of characters in this line to the global position

            // If no match found in this line, move to the previous line
            if (i > 0) {
                lastMatchLine = i - 1;
                lastMatchColumn = lines[lastMatchLine].size();  // Reset to the last column of the new line
            }
        }

        std::cout << "No previous occurrences found.\n";
        return false;
    }






    bool replace(const std::string& oldPattern, const std::string& newStr, std::vector<Line>& lines, bool global = false) {
        bool replaced = false;

        for (size_t i = 0; i < lines.size(); ++i) {
            Line::Iterator it = lines[i].begin();

            while (it != lines[i].end()) {

                if (matchPattern(it, lines[i].end(), oldPattern)) {

                    replaceAt(it, oldPattern.length(), newStr, lines[i]);
                    replaced = true;


                    if (!global) {
                        return true;
                    }
                }
                else {
                    ++it;
                }
            }
        }

        return replaced;
    }





private:

    void moveCursorTo(Line& line, size_t column) {
        line.moveToStartOfLine();
        for (size_t i = 1; i < column; ++i) {
            line.moveRight();
        }
    }


    bool matchPattern(Line::Iterator start, Line::Iterator end, const std::string& pattern) {
        auto it = start;
        for (char ch : pattern) {
            if (it == end || *it != ch) return false;
            ++it;
        }
        return true;
    }

    bool matchPatternReverse(Line::Iterator it, Line::Iterator begin, const std::string& pattern) {

        auto subStrIt = it;
        for (auto patIt = pattern.rbegin(); patIt != pattern.rend(); ++patIt) {
            if (subStrIt == begin || *subStrIt != *patIt) {
                return false;
            }
            --subStrIt;
        }
        return true;
    }



    void replaceAt(Line::Iterator& it, size_t oldPatternLength, const std::string& newStr, Line& line) {

        for (size_t i = 0; i < oldPatternLength; ++i) {
            it = line.eraseChar(it);
        }


        for (char ch : newStr) {
            line.insertCharAt(it, ch);
            ++it;
        }
    }

};
class TextEditor {
private:
    vector<Line> lines;
    int currentLine;
    bool insertMode;
    string copyBuffer;
    EditorStatus status;
    vector<string> commandHistory;
    FileManager fileManager;

    SearchEngine searchEngine;

public:
    TextEditor() : currentLine(0), insertMode(false) {
        lines.emplace_back();
        updateStatusLine();
    }
    void insertChar(char ch) {
        lines[currentLine].insertChar(ch);
        logCommand("Insert character: " + string(1, ch));
        updateStatusLine();

    }
    void updateStatusLine() {
        status.currentMode = insertMode ? INSERT : NORMAL;
        status.cursorLine = currentLine + 1;
        status.cursorColumn = lines[currentLine].getCursorColumn();
        status.totalLines = lines.size();
    }
    string getStatusLineText() const {

        string modeText = (status.currentMode == INSERT) ? "INSERT MODE" : "NORMAL MODE";


        string fileName = fileManager.getCurrentFileName();
        if (fileName.empty() || fileManager.hasUnsavedChanges() || status.currentMode == INSERT) {
            fileName = "[+]";
        }

        return modeText + " | File: " + fileName +
            " | Line: " + to_string(status.cursorLine) +
            ", Col: " + to_string(status.cursorColumn) +
            " | Total Lines: " + to_string(status.totalLines);
    }
    void deleteCurrentLine() {
        if (lines.size() > 1)
        {
            lines.erase(lines.begin() + currentLine);
            if (currentLine >= lines.size()) {
                currentLine = lines.size() - 1;
            }
        }
        else {
            lines[currentLine] = Line();
        }
        logCommand("Delete current line");
    }
    void deleteToEndOfLine() {
        lines[currentLine].deleteToEndOfLine();
        logCommand("Delete to end of line");
        updateStatusLine();
    }
    void deleteCharacterAtCursor() {
        lines[currentLine].deleteCharacterAtCursor();
        logCommand("Delete character at cursor");
        updateStatusLine();
    }
    void backspace() {
        if (lines[currentLine].isEmpty() || lines[currentLine].getCursorColumn() == 1) {
            if (currentLine > 0) {
                lines[currentLine - 1].moveToEndOfLine();
                lines.erase(lines.begin() + currentLine);
                --currentLine;
            }
            else
            {
                std::cout << "Cannot backspace: Already at the first line." << std::endl;
            }
        }
        else
        {
            lines[currentLine].backspace();
        }

        logCommand("Backspace");
        updateStatusLine();
    }


    void yankLine() {
        if (!lines[currentLine].isEmpty())
        {
            copyBuffer.clear();
            for (auto it = lines[currentLine].begin(); it != lines[currentLine].end(); ++it) {
                copyBuffer += *it;
            }
        }
        logCommand("Yank line");
    }
    void yanktwoLine() {

        copyBuffer.clear();

        if (!lines[currentLine].isEmpty()) {

            for (auto it = lines[currentLine].begin(); it != lines[currentLine].end(); ++it) {
                copyBuffer += *it;
            }
            copyBuffer += '\n';
        }


        if (static_cast<size_t>(currentLine + 1) < lines.size() && !lines[currentLine + 1].isEmpty()) {
            for (auto it = lines[currentLine + 1].begin(); it != lines[currentLine + 1].end(); ++it) {
                copyBuffer += *it;
            }
        }

        logCommand("Yank two lines");
    }


    void pasteAfter() {
        if (!copyBuffer.empty()) {

            lines.insert(lines.begin() + currentLine + 1, Line());


            for (char ch : copyBuffer) {
                lines[currentLine + 1].insertChar(ch);
            }


            currentLine++;
        }
        logCommand("Paste After");
    }

    void pasteBefore() {
        if (!copyBuffer.empty()) {

            lines.insert(lines.begin() + currentLine, Line());


            for (char ch : copyBuffer) {
                lines[currentLine].insertChar(ch);
            }
        }
        logCommand("Paste Before");
    }


    void moveToStartOfLine() {
        lines[currentLine].moveToStartOfLine();
        logCommand("Move to start of line");
    }

    void moveToEndOfLine() {
        lines[currentLine].moveToEndOfLine();
        logCommand("Move to end of line");
    }

    void moveToNextWord() {
        lines[currentLine].moveToNextWord();
        logCommand("Move to next of word");
    }

    void moveToPreviousWord() {
        lines[currentLine].moveToPreviousWord();
        logCommand("Move to previuos of word");
    }

    void moveToWordEnd() {
        lines[currentLine].moveToWordEnd();
        logCommand("Move to end of word");
    }

    void moveRight() {
        lines[currentLine].moveRight();
        logCommand("Move right");
        updateStatusLine();

    }

    void moveLeft() {
        lines[currentLine].moveLeft();
        logCommand("Move left");
        updateStatusLine();
    }

    void displayText() const {
        for (size_t i = 0; i < lines.size(); i++) {
            bool displayCursor = (i == currentLine);
            lines[i].display(displayCursor, i + 1);
        }
    }

    void displayStatusLine() const {
        cout << getStatusLineText() << endl;
    }
    void enterInsertMode() {
        insertMode = true;
        logCommand("Enter Insert Mode");
        updateStatusLine();
    }

    void exitInsertMode() {
        insertMode = false;
        logCommand("Exit Insert Mode");
        updateStatusLine();
    }
    bool isInsertMode() const {
        return insertMode;
    }
    void moveUp() {
        if (currentLine > 0) {
            currentLine--;
            lines[currentLine].resetCursor();
        }
        logCommand("Move Up");
        updateStatusLine();
    }

    void moveDown() {
        if (currentLine < lines.size() - 1) {
            currentLine++;
            lines[currentLine].resetCursor();
        }
        logCommand("Move Down");
    }
    void newLine()
    {
        lines.insert(lines.begin() + currentLine + 1, Line());
        currentLine++;
        logCommand("New Line");
        updateStatusLine();
    }
    void display() {
        system("cls");
        cout << "-----------------\n";
        for (size_t i = 0; i < lines.size(); ++i) {
            lines[i].display(i == currentLine, i + 1);
        }
        cout << "-----------------\n";
        cout << getStatusLineText() << endl;
        cout << "Press 'h' to view command history." << endl;
    }

    std::vector<Line>& getLines() {
        return lines;
    }

    int& getCurrentLine()
    {
        return currentLine;
    }
    void deleteTwoLines() {
        deleteCurrentLine();


        if (currentLine < lines.size()) {
            deleteCurrentLine();
        }
        logCommand("Del two lines");
    }
    void deleteThreeLines() {
        deleteCurrentLine();
        deleteCurrentLine();

        if (currentLine < lines.size())
        {
            deleteCurrentLine();
        }
        logCommand("Del three lines");
    }
    void logCommand(const string& command) {
        commandHistory.push_back(command);
    }
    void displayCommandHistory() const {
        cout << "Command History:" << endl;
        for (size_t i = 0; i < commandHistory.size(); ++i) {
            cout << i + 1 << ": " << commandHistory[i] << endl;
        }
        cout << "Press any key to continue..." << endl;
        _getch();
    }
    bool handleFileCommand(const std::string& cmd) {


        if (cmd.rfind(":w ", 0) == 0) {
            string fileName = cmd.substr(3);
            bool result = fileManager.saveFile(fileName, lines);
            updateStatusLine();
            return result;
        }

        else if (cmd == ":wq") {

            if (!fileManager.hasUnsavedChanges() || fileManager.saveFile(fileManager.getCurrentFileName(), lines)) {
                fileManager.saveFile(fileManager.getCurrentFileName(), lines);
                updateStatusLine();
                cout << "Exiting..." << endl;
                return true;
            }
        }

        else if (cmd == ":q") {
            if (!fileManager.hasUnsavedChanges()) {
                logCommand("Warning: Use :q! to force quit without saving.");
                return false;
            }
            cout << "Exiting\n";
            return true;
        }

        else if (cmd == ":q!") {
            cout << "Exiting without saving...\n";
            return true;
        }

        else if (cmd.rfind(":e ", 0) == 0) {
            string fileName = cmd.substr(3);
            bool result = fileManager.loadFile(fileName, lines);
            updateStatusLine();
            return result;
        }

        return false;
    }
    void moveDown(int count) {

        if (lines.empty()) {
            lines.emplace_back(Line());
            currentLine = 0;
        }


        while (currentLine + count >= static_cast<int>(lines.size())) {
            lines.emplace_back(Line());
        }


        currentLine = std::min(static_cast<int>(lines.size() - 1), currentLine + count);

        logCommand("Move down " + std::to_string(count) + " lines");
    }


    void updateModifiedStatus()
    {
        fileManager.markAsModified();
    }

    bool handleSearchReplaceCommand(const std::string& cmd, int& currentLine)
    {

        if (cmd.rfind("/ ", 0) == 0)
        {
            std::string pattern = cmd.substr(2);
            return searchEngine.search(pattern, lines, currentLine);
            logCommand("Search");
        }

        else if (cmd == "m")
        {
            return searchEngine.findNext(lines, currentLine);
            logCommand("find Next");
        }

        if (cmd == "N")
        {
            return searchEngine.findPrevious(lines, currentLine);
        }

        else if (cmd.rfind(":s/", 0) == 0)
        {
            size_t firstSlash = cmd.find('/', 3);
            size_t secondSlash = cmd.find('/', firstSlash + 1);

            if (firstSlash == std::string::npos || secondSlash == std::string::npos)
            {
                std::cout << "Invalid replace command syntax.\n";
                return false;
            }


            std::string oldPattern = cmd.substr(3, firstSlash - 3);
            std::string newStr = cmd.substr(firstSlash + 1, secondSlash - firstSlash - 1);


            bool global = (cmd.substr(secondSlash + 1) == "g");

            return searchEngine.replace(oldPattern, newStr, lines, global);
        }

    }

    bool handleCommand(const std::string& cmd)
    {
        int currentLine = 0;
        if (cmd[0] == '/') {
            return handleSearchReplaceCommand(cmd, currentLine);
        }
        else {
            return handleFileCommand(cmd);
        }
    }

    void deleteLine(int lineNumber)
    {
        if (lineNumber >= 1 && lineNumber <= static_cast<int>(lines.size())) {
            lines.erase(lines.begin() + lineNumber - 1);
            currentLine = std::min(currentLine, static_cast<int>(lines.size()) - 1);
            logCommand("Delete line " + std::to_string(lineNumber));
        }
        else {
            std::cout << "Error: Line number out of range" << std::endl;
        }
    }
    void indentLine() {
        if (!lines.empty()) {
            const std::string indent = "    ";
            lines[currentLine].insertCharAtStart(indent);
            logCommand("Indent current line");
        }
        else {
            std::cout << "Error: No lines to indent" << std::endl;
        }
    }
    void unindentLine() {
        if (!lines.empty())
        {
            const std::string indent = "    ";
            for (int i = 0; i < static_cast<int>(indent.length()) && !lines[currentLine].isEmpty(); ++i) {
                if (lines[currentLine].getFirstChar() == ' ') {
                    lines[currentLine].deleteFirstChar();
                }
                else {
                    break;
                }
            }
            logCommand("Unindent current line");
        }
        else {
            std::cout << "Error: No lines to unindent" << std::endl;
        }
    }
    void joinLines() {
        if (currentLine < static_cast<int>(lines.size()) - 1)
        {
            std::string nextLineContent = lines[currentLine + 1].getContent();
            for (char ch : nextLineContent) {
                lines[currentLine].insertChar(ch);
            }
            lines.erase(lines.begin() + currentLine + 1);
            logCommand("Join current line with next line");
        }
        else {
            std::cout << "Error: No next line to join" << std::endl;
        }
    }





};



int getChar() {
    int ch = _getch();
    if (ch == 0 || ch == 224) {
        int arrowKey = _getch();
        switch (arrowKey) {
        case 72: return 65;
        case 80: return 66;
        case 77: return 67;
        case 75: return 68;
        }
    }
    return ch;
}
int main()
{
    TextEditor editor;
    SearchEngine searchEngine;
    int cmd;

    while (true)
    {
        editor.display();
        cmd = getChar();


        if (cmd == ':')
        {
            std::string command;
            std::cout << "Enter command: ";
            std::getline(std::cin, command);

            if (editor.handleFileCommand(command))
            {
                if (command == ":q" || command == ":q!" || command == ":wq") {
                    break;
                }
            }
            else
            {
                std::cout << "Invalid command or operation failed.\n";
            }
            continue;
        }
        if (cmd == 27) {
            editor.exitInsertMode();
            continue;
        }
        if (editor.isInsertMode()) {
            if (cmd >= 65 && cmd <= 68) {
                switch (cmd) {
                case 65: editor.moveUp(); break;
                case 66: editor.moveDown(); break;
                case 67: editor.moveRight(); break;
                case 68: editor.moveLeft(); break;
                }
            }
            else {
                switch (cmd) {
                case 13: editor.newLine(); break;
                case 8: editor.backspace(); break;
                default: editor.insertChar(static_cast<char>(cmd)); break;
                }
            }
        }
        else {
            switch (cmd) {
            case 'i':
                editor.enterInsertMode();
                break;
            case 65: editor.moveUp(); break;
            case 66: editor.moveDown(); break;
            case 'a': editor.moveDown(5);
            case 'z':
                int a;
                cout << "Enter the line you want to delale: ";
                cin >> a;
                {
                    editor.deleteLine(a);
                }
                break;
            case 67: editor.moveRight(); break;
            case 68: editor.moveLeft(); break;
            case 'n': editor.newLine(); break;
            case 'y': editor.yankLine(); break;
            case 'Y': editor.yanktwoLine(); break;
            case 'p': editor.pasteAfter(); break;
            case 'P': editor.pasteBefore(); break;
            case 'x': editor.deleteCharacterAtCursor(); break;
            case 'd': editor.deleteToEndOfLine(); break;
            case '0': editor.moveToStartOfLine(); break;
            case '$': editor.moveToEndOfLine(); break;
            case 'w': editor.moveToNextWord(); break;
            case 'b': editor.moveToPreviousWord(); break;
            case 'J': editor.joinLines(); break;
            case '>':editor.indentLine(); break;
            case '<':editor.unindentLine(); break;

            case 'e': editor.moveToWordEnd(); break;
            case 'h': editor.displayCommandHistory(); break;
            case 'l': editor.deleteTwoLines(); break;
            case 'k': editor.deleteThreeLines(); break;




            case '/':
                std::cout << "Enter search pattern: ";
                {
                    std::string pattern;
                    std::getline(std::cin, pattern);
                    if (!editor.handleSearchReplaceCommand("/ " + pattern, editor.getCurrentLine())) {
                        std::cout << "Pattern not found." << std::endl;
                    }
                }
                break;
            case 'm':
                if (!editor.handleSearchReplaceCommand("m", editor.getCurrentLine()))
                {
                    std::cout << "No further matches." << std::endl;
                }
                break;
            case 'N':
                if (!editor.handleSearchReplaceCommand("N", editor.getCurrentLine()))
                {
                    std::cout << "No previous matches." << std::endl;
                }
                break;

            case 'q':
                return 0;
            case 'f':
                if (getChar() == 'f') {
                    editor.deleteCurrentLine();
                }
                break;
            case ';':
            { // Replace command
                std::cout << "Enter replace command (e.g., :s/old/new/g): ";
                std::string replaceCmd;
                std::getline(std::cin, replaceCmd);
                if (!editor.handleSearchReplaceCommand(replaceCmd, editor.getCurrentLine())) {
                    std::cout << "Replace command failed.\n";
                }
                else {
                    std::cout << "Replace command executed successfully.\n";
                }
                break;
            }
            default:
                break;
            }
        }
    }

    return 0;
}