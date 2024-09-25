#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

const int BOARD_WIDTH = 80;
const int BOARD_HEIGHT = 25;

class Shapes {
protected:
    int id;
    double x, y;
public:
    Shapes(int id, double x, double y) : id(id), x(x), y(y) {}
    virtual string getInfo() const = 0;
    int getID() const { return id; }
    double getX() const { return x; }
    double getY() const { return y; }
    virtual void drawOnBoard(vector<vector<char>>& grid) const = 0;
    virtual ~Shapes() {}
};

class Triangle : public Shapes {
private:
    double base, height;
public:
    Triangle(int id, double x, double y, double b, double h)
        : Shapes(id, x, y), base(b), height(h) {}

    string getInfo() const override {
        return "Triangle: ID=" + to_string(id) + " Base=" + to_string(base) +
            " Height=" + to_string(height) + " at (" + to_string(x) + "," + to_string(y) + ")";
    }

    // Оновлена функція для відображення трикутника
    void drawOnBoard(vector<vector<char>>& grid) const override {
        int startX = static_cast<int>(x);
        int startY = static_cast<int>(y);
        int heightInt = static_cast<int>(height);

        // Малюємо сторони трикутника
        for (int i = 0; i < heightInt; ++i) {
            int leftMost = startX - i;
            int rightMost = startX + i;
            int posY = startY + i;

            if (posY < BOARD_HEIGHT) {
                if (leftMost >= 0 && leftMost < BOARD_WIDTH)
                    grid[posY][leftMost] = '*';  // Ліва сторона

                if (rightMost >= 0 && rightMost < BOARD_WIDTH && leftMost != rightMost)
                    grid[posY][rightMost] = '*';  // Права сторона
            }
        }

        // Малюємо основу трикутника
        for (int j = 0; j < 2 * heightInt - 1; ++j) {
            int baseX = startX - heightInt + 1 + j;
            int baseY = startY + heightInt - 1;
            if (baseX >= 0 && baseX < BOARD_WIDTH && baseY < BOARD_HEIGHT)
                grid[baseY][baseX] = '*';
        }
    }
};

class Circle : public Shapes {
private:
    double radius;
public:
    Circle(int id, double x, double y, double r) : Shapes(id, x, y), radius(r) {}

    string getInfo() const override {
        return "Circle: ID=" + to_string(id) + " Radius=" + to_string(radius) +
            " at (" + to_string(x) + "," + to_string(y) + ")";
    }

    void drawOnBoard(vector<vector<char>>& grid) const override {
        // Спрощена логіка відображення кола
        for (int i = -radius; i <= radius; ++i) {
            for (int j = -radius; j <= radius; ++j) {
                int posX = static_cast<int>(x + i);
                int posY = static_cast<int>(y + j);
                if (posX >= 0 && posX < BOARD_WIDTH && posY >= 0 && posY < BOARD_HEIGHT) {
                    double dist = i * i + j * j;
                    if (dist >= (radius - 0.5) * (radius - 0.5) && dist <= (radius + 0.5) * (radius + 0.5)) {
                        grid[posY][posX] = '*';
                    }
                }
            }
        }
    }
};

class Square : public Shapes {
private:
    double side;
public:
    Square(int id, double x, double y, double s) : Shapes(id, x, y), side(s) {}

    string getInfo() const override {
        return "Square: ID=" + to_string(id) + " Side=" + to_string(side) +
            " at (" + to_string(x) + "," + to_string(y) + ")";
    }

    void drawOnBoard(vector<vector<char>>& grid) const override {
        int startX = static_cast<int>(x);
        int startY = static_cast<int>(y);
        int sideInt = static_cast<int>(side);

        // Draw top and bottom edges
        for (int i = 0; i < sideInt; ++i) {
            if (startX + i < BOARD_WIDTH && startY < BOARD_HEIGHT)
                grid[startY][startX + i] = '*';  // Top edge

            if (startX + i < BOARD_WIDTH && startY + sideInt - 1 < BOARD_HEIGHT)
                grid[startY + sideInt - 1][startX + i] = '*';  // Bottom edge
        }

        // Draw left and right edges
        for (int i = 0; i < sideInt; ++i) {
            if (startY + i < BOARD_HEIGHT && startX < BOARD_WIDTH)
                grid[startY + i][startX] = '*';  // Left edge

            if (startY + i < BOARD_HEIGHT && startX + sideInt - 1 < BOARD_WIDTH)
                grid[startY + i][startX + sideInt - 1] = '*';  // Right edge
        }
    }

};

class Board {
private:
    vector<vector<char>> grid;  // 2D сітка для дошки
    vector<Shapes*> shapes;
    int nextID;
public:
    Board() : grid(BOARD_HEIGHT, vector<char>(BOARD_WIDTH, ' ')), nextID(1) {}

    ~Board() {
        for (auto shape : shapes) {
            delete shape;
        }
    }
    void print() {
        // Верхня межа
        std::cout << "+";
        for (int i = 0; i < BOARD_WIDTH; ++i) {
            std::cout << "-";
        }
        std::cout << "+" << "\n";

        // Основна частина дошки з лівою і правою рамкою
        for (auto& row : grid) {
            std::cout << "|";  // Ліва межа
            for (char c : row) {
                std::cout << c;
            }
            std::cout << "|" << "\n";  // Права межа
        }

        // Нижня межа
        std::cout << "+";
        for (int i = 0; i < BOARD_WIDTH; ++i) {
            std::cout << "-";
        }
        std::cout << "+" << "\n";
    }
    void draw() {
        for (auto& row : grid) {
            fill(row.begin(), row.end(), ' ');
        }

        // Малюємо кожну фігуру на сітці
        for (const auto& shape : shapes) {
            shape->drawOnBoard(grid);
        }
    }

    void list() {
        if (shapes.empty()) {
            cout << "No shapes added yet.\n";
        }
        else {
            cout << "List of shapes on the board:\n";
            for (const auto& shape : shapes) {
                cout << shape->getInfo() << endl;
            }
        }
    }

    void addCircle(double x, double y, double r) {
        shapes.push_back(new Circle(nextID++, x, y, r));
    }

    void addSquare(double x, double y, double s) {
        shapes.push_back(new Square(nextID++, x, y, s));
    }

    void addTriangle(double x, double y, double b, double h) {
        shapes.push_back(new Triangle(nextID++, x, y, b, h));
    }

    void undo() {
        if (!shapes.empty()) {
            delete shapes.back();
            shapes.pop_back();
        }
        else {
            cout << "No shapes to undo.\n";
        }
    }

    void clear() {
        for (auto shape : shapes) {
            delete shape;
        }
        shapes.clear();
        cout << "Board cleared.\n";
    }
    void save(const string& filename) {
        ofstream file(filename);
        if (!file) {
            cerr << "Error: Could not open file for writing.\n";
            return;
        }
        file << shapes.size() << endl;  // Записуємо кількість фігур
        for (const auto& shape : shapes) {
            file << shape->getInfo() << endl;  // Записуємо інформацію про фігуру
        }
        file.close();
        cout << "Blackboard saved to " << filename << ".\n";
    }

    // Метод для завантаження дошки з файлу
    void load(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cerr << "Error: Could not open file for reading.\n";
            return;
        }

        clear();  // Очищуємо поточну дошку

        int shapeCount;
        file >> shapeCount;  // Читаємо кількість фігур
        for (int i = 0; i < shapeCount; ++i) {
            string shapeType;
            file >> shapeType;
            if (shapeType == "Circle:") {
                int id;
                double x, y, r;
                file >> id >> x >> y >> r;
                addCircle(x, y, r);
            }
            else if (shapeType == "Square:") {
                int id;
                double x, y, s;
                file >> id >> x >> y >> s;
                addSquare(x, y, s);
            }
            else if (shapeType == "Triangle:") {
                int id;
                double x, y, b, h;
                file >> id >> x >> y >> b >> h;
                addTriangle(x, y, b, h);
            }
        }
        file.close();
        cout << "Blackboard loaded from " << filename << ".\n";
    }
};
class CommandLine {
private:
    Board board;
public:
    void run() {
        string command;
        while (true) {
            cout << "> ";
            cin >> command;
            if (command == "draw") {
                board.draw();
                board.print();
            }
            else if (command == "list") {
                board.list();
            }
            else if (command == "add") {
                string shapeType;
                cin >> shapeType;
                if (shapeType == "circle") {
                    double x, y, r;
                    cin >> x >> y >> r;
                    board.addCircle(x, y, r);
                }
                else if (shapeType == "square") {
                    double x, y, s;
                    cin >> x >> y >> s;
                    board.addSquare(x, y, s);
                }
                else if (shapeType == "triangle") {
                    double x, y, b, h;
                    cin >> x >> y >> b >> h;
                    board.addTriangle(x, y, b, h);
                }
            }
            else if (command == "undo") {
                board.undo();
            }
            else if (command == "clear") {
                board.clear();
            }
            else if (command == "save") {
                string filepath;
                cin >> filepath;
                board.save(filepath);
            }
            else if (command == "load") {
                string filepath;
                cin >> filepath;
                board.load(filepath);
            }
            else if (command == "exit") {
                break;
            }
            else {
                cout << "Unknown command.\n";
            }
        }
    }
};

int main(){
    CommandLine cmd;
    cmd.run();
    return 0;
}