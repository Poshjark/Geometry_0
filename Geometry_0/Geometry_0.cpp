#include <iostream>
#include <iomanip>
#include <math.h>

using namespace std;
const double PI = acos(-1.0);

// Класс Точка с двумя приватными координатами x и y.
class Point {
private:
    double x;
    double y;
public:
    Point(double x_input = 0, double y_input = 0) {
        x = x_input;
        y = y_input;
    }
    void move(double x_shift = 0, double y_shift = 0) {
        x = x + x_shift;
        y = y + y_shift;
    }
    double getCoordX() {
        return x;
    }

    double getCoordY() {
        return y;
    }
};

// Структура-обёртка, чтобы возвращать из функции сразу две точки.
// Служит в дальнейшем для нахождения двух точек пересечения.
// Да, я ещё не умею возвращать из функции массивы
// Нет, вектором я пока пользоваться не хочу.
struct TwoPoints
{
    Point first;
    Point second;
};



// Пока не знаю, зачем мне нужен этот класс, вообще я собирался все остальные
// от него наследовать, но как-то не пошло.
// Родительский для Rectangle. Пока работает.
// В планах наследовать от него все. Проблемы с доступом каким-то.
class Object {
private:
    int point_number;
    Point *points;
public:
    string name;
    Object() {
        point_number = 1;
        points = new Point[1];
        points[0] = Point();
        name = "point";
    }
    Object(Point start, Point end) {
        point_number = 2;
        points = new Point[point_number]{ start,end };
        name = "line";
    }
    Object(double width, double lenght) {
        point_number = 4;
        points = new Point[4];
        points[0] = Point(-lenght / 2, -width / 2);
        points[1] = Point(-lenght / 2, width / 2);
        points[2] = Point(lenght / 2, width / 2);
        points[3] = Point(lenght / 2, -width / 2);
        name = "rectangle";
    }
    Object(Point points_input[]) {
        point_number = sizeof(points_input)/sizeof(Point(0,0));
        switch (point_number) {
        case 0:
            name = "null";
            break;
        case 1:
            name = "point";
            break;
        case 2:
            name = "line";
            break;
        case 3:
            name = "tria";
            break;
        case 4:
            name = "rect";
            break;
        default:
            name = "mult";
            break;
        }
        points = new Point[point_number];
        for (int i = 0; i <point_number; i++) {
            points[i] = points_input[i];
        }
        
    }
    ~Object(){
        delete[] points;
}

    
    int get_point_number() {
        return point_number;
    }

    Point* get_points() {
        return points;
    }
    void rotate(Point rotation_center);
};

class Circle {
private:
    double rad;
    Point center;
public:
    Circle(double rad_input = 1, Point center_inp = Point()) {
        rad = rad_input;
    }

    double radius() {
        return rad;
    }

};

class Line{
private:
    Point *start;
    Point *end;
    double k,b;
    double angle;
public:
    Line() {
        start = new Point();
        end = new Point();
        angle = 0;
        k = 0;
        b = 0;

    }
    Line(Point *start_input, Point *end_input)  {
        start = start_input;
        end = end_input;
        k = (start->getCoordY() - end->getCoordY()) / (start->getCoordX() - end->getCoordX());
        b = start->getCoordY() - start->getCoordX() * k;
        angle = atan(k);
    }

    //Меняет местами начало и конец линии(вектора)
    void reverse() {
        Point *temp;
        temp = start;
        start = end;
        end = temp;
        int a = 0;
    }

    Point getStart() {
        return *start;
    }
    Point getEnd() {
        return *end;
    }
    double coeffK() {
        return k;
    }
    double coeffB() {
        return b;
    }
    double get_length() {
        return (pow(start->getCoordX() - end->getCoordX(),2)) + (pow(start->getCoordY() - end->getCoordY(), 2));

    }

    // Угол наклона между прямой, которой принадлежит линия
    // и положительным направлением оси X.
    double get_angle(char form = 'RAD') {
        switch (form) {
        case 'RAD':
            break;
        case 'DEG':
            return angle * 180 / PI;
        default:
            return angle * 180 / PI;
        }
        return angle;
    }


    // Ищет точки пересечения между линией и окружностью
    // Необходима для работы функции поворота относительно центра.
    TwoPoints line_circle_cross(Line line, double c_rad) {
        double k = line.coeffK();
        double b = line.coeffB();
        double x[2]{ 0,0 }, y[2]{ 0,0 };
        bool cond_above = pow(b * k, 2) - 4 * (k + 1) * (b - pow(c_rad, 2)) > 0;
        bool zero = pow(b * k, 2) - 4 * (k + 1) * (b - pow(c_rad, 2)) == 0;
        if (cond_above) {
            x[0] = (-b + sqrt(pow(b * k, 2) - 4 * (k + 1) * (b - pow(c_rad, 2))));
            x[1] = (-b + sqrt(pow(b * k, 2) - 4 * (k + 1) * (b - pow(c_rad, 2))));
        }
        else if (zero) {
            x[0] = (-b + sqrt(pow(b * k, 2) - 4 * (k + 1) * (b - pow(c_rad, 2))));
            x[1] = x[0];
        }
        double y_pre[2];
        for (int i = 0; i < 2; i++) {
            bool condition;
            y_pre[i] = k * x[i] + b;
            condition = pow(x[i], 2) + pow(y[i], 2) == pow(c_rad, 2);
            if (condition) {
                y[i] = y_pre[i];
            }
        }
        return TwoPoints{ Point(x[0],y[0]),Point(x[1],y[1]) };
    }

    // Процедура поворота линии. Угол поворота в градусах.
    void rotate_line(Line* line, double degree) {
        double k_local = line->coeffK();
        double start_angle = line->get_angle('DEG');
        double new_angle = start_angle + degree;
        k = tan(start_angle);
        TwoPoints new_points = line_circle_cross(*line, line->get_length() / 2);
        start = &new_points.first;
        end = &new_points.second;
    }
};

class Rectangle : public Object{
private:
    double width, length;
    Point *points[4];
public:
    Rectangle():Object() {

    }
    Rectangle(double width_inp, double length_inp) : Object(width_inp, length_inp) {
        width = width_inp;
        length = length_inp;
    }
};

// Работает по аналогии с regex_search, только ищет точку пересечения
// Для работы требует предварительно созданного объекта Point,
// координаты которой и будут точкой пересечения.
bool crossing_point(Line first, Line second, Point* crossPoint) {
    double x = 0;
    crossPoint->move(0- crossPoint->getCoordX(), 0- crossPoint->getCoordY());
    if ((first.coeffB() == 0) && (second.coeffB() == 0)) {
        x = (second.coeffK() - first.coeffK());
    }
    else {
        x =  (first.coeffB() - second.coeffB())/(second.coeffK() - first.coeffK());
    }
    double y = x * first.coeffK() + first.coeffB();
    if ((x <= first.getEnd().getCoordX() && x >= first.getStart().getCoordX())
        ||
        (x <= first.getStart().getCoordX() && x >= first.getEnd().getCoordX())) {
        crossPoint->move(x,y);
        return true;
    }
    else {
        return false;
    }
    return false;
}

void Object::rotate(Point rotation_center) {
    for (int i = 0; i < point_number; i++) {
        Line temp_line = Line(&points[i], &rotation_center);
        Circle temp_circle = Circle(temp_line.get_length(), rotation_center);
        Point cross;
        
    }
}

int main()
{

    Point *start = new Point();
    Point *end = new Point(10,10);
    Line a = Line(start, end);
    Line b = Line(&Point(0,1), &Point(1, 2));
    a.reverse();
    cout << a.getStart().getCoordX()<< '\t'<< a.getStart().getCoordY() << endl;
    cout << a.get_angle() << endl;
    Point cross;
    Point *cross_ptr;
    cross_ptr = &cross;
    if (crossing_point(a, b, cross_ptr)) {
        cout << cross_ptr->getCoordX() << '\t' << cross_ptr->getCoordY() << endl;
    }
    else {
        cout << "No crossing points! between lines!" << endl;
    }
    double ss = 1 - 10;
    Rectangle rect = Rectangle(10, 9);
    for (int i = 0; i < 4; i++) {
        cout << rect.get_points()[i].getCoordX() << 
            '\t' << rect.get_points()[i].getCoordY() << endl;

    }
    cout << rect.name << endl;
    Object asd = Object(Point(1,2), Point(1,22));
    cout << asd.name << endl;
    return 0;
}
