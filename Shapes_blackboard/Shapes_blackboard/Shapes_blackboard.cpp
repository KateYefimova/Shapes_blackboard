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
    string shape;
public:
    Shapes(string s, int id, double x, double y) : shape(s), id(id), x(x), y(y) {}
    virtual string getInfo() const = 0;
    virtual string getShape() const { return shape; }
    int getID() const { return id; }
    double getX() const { return x; }
    double getY() const { return y; }
    virtual void drawOnBoard(vector<vector<char>>& grid) const = 0;
    virtual string getLoad() const = 0;
    virtual ~Shapes() {}
};

class Triangle : public Shapes {
private:
    double base, height;
public:
    Triangle(int id, double x, double y, double b, double h)
        : Shapes( "Triangle", id, x, y), base(b), height(h) {}

    string getShape() const override {
        return "triangle";
    }

    string getInfo() const override {
        return "Triangle: ID=" + to_string(id) + " Base=" + to_string(base) +
            " Height=" + to_string(height) + " at (" + to_string(x) + "," + to_string(y) + ")";
    }
    string getLoad() const override {
        return "Triangle: " + to_string(id) + " " + to_string(x) + " " + to_string(y) + " " + to_string(base) + " " + to_string(height);
    }
    // Оновлена функція для відображення трикутника
    void drawOnBoard(vector<vector<char>>& grid) const override {
        int startX = static_cast<int>(x);
        int startY = static_cast<int>(y);
        int heightInt = static_cast<int>(height);

        if (startY + heightInt - 1 >= BOARD_HEIGHT || startY < 0 ||
            startX - heightInt < 0 || startX + heightInt >= BOARD_WIDTH) {
            return;  // Triangle is out of bounds, do not draw
        }

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
    Circle( int id, double x, double y, double r) : Shapes("Circle", id, x, y), radius(r) {}

    string getInfo() const override {
        return "Circle: ID=" + to_string(id) + " Radius=" + to_string(radius) +
            " at (" + to_string(x) + "," + to_string(y) + ")";
    }
    string getLoad() const override {
        return "Circle: " + to_string(id) + " " + to_string(x) + " " + to_string(y) + " " + to_string(radius);
    }
    string getShape() const override {
        return "circle";
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
    Square(int id, double x, double y, double s) : Shapes("Square", id, x, y), side(s) {}

    string getInfo() const override {
        return "Square: ID=" + to_string(id) + " Side=" + to_string(side) +
            " at (" + to_string(x) + "," + to_string(y) + ")";
    }
    string getLoad() const override {
        return "Square: " + to_string(id) + " " + to_string(x) + " " + to_string(y) + " " + to_string(side);
    }
    string getShape() const override {
        return "square";
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
class Rectangle : public Shapes {
private:
    double  width, height;
public:
    Rectangle(double id, double x, double y, double w, double h) : Shapes("Rectangle", id, x, y), width(w), height(h) {}

    string getInfo() const override {
        return "Rectangle: top-left corner (" + std::to_string(x) + ", " + std::to_string(y) +
            "), width " + std::to_string(width) + ", height " + std::to_string(height);
    }
    string getLoad() const override {
        return "Rectangle: " + std::to_string(getID()) + " " +
            std::to_string(getX()) + " " +
            std::to_string(getY()) + " " +
            std::to_string(width) + " " +
            std::to_string(height);
    }
    string getShape() const override {
        return "square";
    }
    void drawOnBoard(vector<vector<char>>& grid) const override {
        int startX = static_cast<int>(x);
        int startY = static_cast<int>(y);
        int heightInt = static_cast<int>(height);
        int widthInt = static_cast<int>(width);
        

        // Малюємо верхню і нижню межі
        for (int i = 0; i < widthInt; ++i) {
            if (startX + i < grid[0].size() && startY < grid.size())
                grid[startY][startX + i] = '*';  // Верхня межа

            if (startX + i < grid[0].size() && startY + heightInt - 1 < grid.size())
                grid[startY + heightInt - 1][startX + i] = '*';  // Нижня межа
        }

        // Малюємо ліву і праву межі
        for (int i = 0; i < heightInt; ++i) {
            if (startY + i < grid.size() && startX < grid[0].size())
                grid[startY + i][startX] = '*';  // Ліва межа

            if (startY + i < grid.size() && startX + widthInt - 1 < grid[0].size())
                grid[startY + i][startX + widthInt - 1] = '*';  // Права межа
        }
    }

};

class Line : public Shapes {
private:
    int x1, y1, x2, y2; // координати двох точок, які визначають лінію

public:
    // Конструктор
    Line(int id, int x1, int y1, int x2, int y2)
        : Shapes("Line", id, x1, y1), x1(x1), y1(y1), x2(x2), y2(y2) {}

    // Метод для малювання лінії
    void drawOnBoard(vector<vector<char>>& grid) const override {
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;

        int x = x1;
        int y = y1;

        while (true) {
            // Перевірка меж сітки перед малюванням
            if (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT)
                grid[y][x] = '*';

            if (x == x2 && y == y2) break;

            int e2 = err * 2;

            if (e2 > -dy) {
                err -= dy;
                x += sx;
            }

            if (e2 < dx) {
                err += dx;
                y += sy;
            }
        }
    }

    // Метод для отримання інформації про фігуру
    string getInfo() const override {
        return "Line: from (" + std::to_string(x1) + ", " + std::to_string(y1) + ") to (" +
            std::to_string(x2) + ", " + std::to_string(y2) + ")";
    }
    string getLoad() const override {
        return "Line: " + std::to_string(id) + " " + std::to_string(x1) + " " +
            std::to_string(y1) + " " + std::to_string(x2) + " " + std::to_string(y2);
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

    bool isOccupied(double x, double y, const string& type, double param1 = 0, double param2 = 0) {
        for (const auto& shape : shapes) {
            if (shape->getShape() == type) {
                double shapeX = shape->getX();
                double shapeY = shape->getY();

                if (shapeX == x && shapeY == y) {
                    if (type == "circle") {
                        const Circle* circle = dynamic_cast<const Circle*>(shape);
                        if (circle && circle->getShape() == type && circle->getX() == x && circle->getY() == y && circle->getInfo().find(to_string(param1)) != string::npos) {
                            return true;
                        }
                    }
                    else if (type == "square") {
                        const Square* square = dynamic_cast<const Square*>(shape);
                        if (square && square->getShape() == type && square->getX() == x && square->getY() == y && square->getInfo().find(to_string(param1)) != string::npos) {
                            return true;
                        }
                    }
                    else if (type == "triangle") {
                        const Triangle* triangle = dynamic_cast<const Triangle*>(shape);
                        if (triangle && triangle->getShape() == type && triangle->getX() == x && triangle->getY() == y && triangle->getInfo().find(to_string(param1)) != string::npos && triangle->getInfo().find(to_string(param2)) != string::npos) {
                            return true;
                        }
                    }
                    else if (type == "rectangle") {
                        const Rectangle* rectangle = dynamic_cast<const Rectangle*>(shape);
                        if (rectangle && rectangle->getShape() == type && rectangle->getX() == x && rectangle->getY() == y &&
                            rectangle->getInfo().find(to_string(param1)) != string::npos && rectangle->getInfo().find(to_string(param2)) != string::npos) {
                            return true;
                        }
                    }
                    // Додавання перевірки для лінії
                    else if (type == "line") {
                        const Line* line = dynamic_cast<const Line*>(shape);
                        if (line && line->getShape() == type &&
                            line->getInfo().find(to_string(x)) != string::npos &&
                            line->getInfo().find(to_string(y)) != string::npos &&
                            line->getInfo().find(to_string(param1)) != string::npos &&
                            line->getInfo().find(to_string(param2)) != string::npos) {
                            return true;
                        }
                    }
                }
            }
        }
        return false;
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

    bool isInBounds(double x, double y) const {
        if (x < 0 || y < 0) {
            cout << "err1" << endl;
            return false;
        }
        if (x > BOARD_WIDTH || y > BOARD_HEIGHT) {
            cout << "err" << endl;
            return false;
        }
        return true;
    }

    void addCircle(double x, double y, double r) {
        if (!isOccupied(x, y, "circle", r)) {
            if (isInBounds(x, y) || isInBounds(x - r, y) || isInBounds(x + r, y) || isInBounds(x, y - r) || isInBounds(x, y + r)) {
                shapes.push_back(new Circle(nextID++, x, y, r));
            }
            else {
                cout << "Error: Circle cannot be placed outside the board.\n";
            }
        }
        else {
            cout << "Error: Circle already has been placed.\n";
        }
        
    }

    void addSquare(double x, double y, double s) {
        if (!isOccupied(x, y, "square", s)) {
            if (isInBounds(x, y) || isInBounds(x + s - 1, y) || isInBounds(x, y + s - 1)) {
                shapes.push_back(new Square(nextID++, x, y, s));
            }
            else {
                cout << "Error: Square cannot be placed outside the board.\n";
            }
        }
        else {
            cout << "Error: Square already has been placed.\n";
        }
        
    }

    void addTriangle(double x, double y, double b, double h) {
        if (!isOccupied(x, y, "triangle", b, h)) {
            int startX = static_cast<int>(x);
            int startY = static_cast<int>(y);
            if (isInBounds(x, y) || isInBounds(x - b / 2, y + h - 1) || isInBounds(x + b / 2, y + h - 1) || isInBounds(x, y + h)) {
                shapes.push_back(new Triangle(nextID++, x, y, b, h));
            }
            else {
                cout << "Error: Triangle cannot be placed outside the board.\n";
            }
        }
        else {
            cout << "Error: Triangle already has been placed.\n";
        }
    }

    void addLine(int x1, int y1, int x2, int y2) {
        // Перевіряємо, чи лінія може бути розміщена на цих координатах
        if (!isOccupied(x1, y1, "line", x2, y2)) {
            // Перевіряємо, чи координати початку і кінця лінії в межах дошки
            if (isInBounds(x1, y1) || isInBounds(x2, y2)) {
                // Якщо все добре, додаємо лінію на дошку
                shapes.push_back(new Line(nextID++, x1, y1, x2, y2));
            }
            else {
                cout << "Error: Line cannot be placed outside the board.\n";
            }
        }
        else {
            cout << "Error: Line already exists at these coordinates.\n";
        }
    }

    void addRectangle(double x, double y, double width, double height) {
        // Перевіряємо, чи місце для прямокутника вільне
        if (!isOccupied(x, y, "rectangle", width, height)) {
            // Перевіряємо, чи прямокутник не виходить за межі дошки
            if (isInBounds(x, y) || isInBounds(x + width - 1, y + height - 1)) {
                // Якщо всі умови виконані, додаємо новий прямокутник
                shapes.push_back(new Rectangle(nextID++, x, y, width, height));
            }
            else {
                cout << "Error: Rectangle cannot be placed outside the board.\n";
            }
        }
        else {
            cout << "Error: Rectangle already exists at this location.\n";
        }
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
            file << shape->getLoad() << endl; 
            
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
            else if (shapeType == "Line:") {
                int id;
                double x1, y1, x2, y2;
                file >> id >> x1 >> y1 >> x2 >> y2;
                addLine(x1, y1, x2, y2);
            }
            else if (shapeType == "Rectangle:") {
                int id;
                double x, y, w, h;
                file >> id >> x >> y >> w >> h;
                addRectangle(x, y, w, h);
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
                
                else if (shapeType == "line") {
                    int x1, y1, x2, y2;
                    cin >> x1 >> y1 >> x2 >> y2;
                    board.addLine(x1, y1, x2, y2);
                }
                else if (shapeType == "rectangle") {
                    double x, y, w, h;
                    cin >> x >> y >> w >> h;
                    board.addRectangle(x, y, w, h);
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

int main() {
    CommandLine cmd;
    cmd.run();
    return 0;
}