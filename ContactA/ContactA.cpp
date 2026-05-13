#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <cstdlib>

using namespace std;

bool safeStoi(const string& str, int& out) {
    try {
        size_t pos;
        out = stoi(str, &pos);
        return pos == str.size();
    } catch (...) {
        return false;
    }
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
}

class Date {
private:
    int year, month, day;
public:
    Date(int y = 2000, int m = 1, int d = 1) : year(y), month(m), day(d) {}

    int getYear() const { return year; }
    int getMonth() const { return month; }
    int getDay() const { return day; }

    void setDate(int y, int m, int d) { year = y; month = m; day = d; }

    static bool isLeap(int y) {
        return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    }

    bool isValid() const {
        if (year < 1900 || year > 2100) return false;
        if (month < 1 || month > 12) return false;
        int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        if (isLeap(year)) daysInMonth[2] = 29;
        if (day < 1 || day > daysInMonth[month]) return false;
        return true;
    }

    int calcAge() const {
        time_t now = time(0);
        tm ltm;
        localtime_s(&ltm, &now);
        int curYear = 1900 + ltm.tm_year;
        int curMonth = 1 + ltm.tm_mon;
        int curDay = ltm.tm_mday;

        int age = curYear - year;
        if (curMonth < month || (curMonth == month && curDay < day))
            age--;
        return age;
    }

    friend ostream& operator<<(ostream& out, const Date& d) {
        out << d.year << "," << d.month << "," << d.day;
        return out;
    }

    friend istream& operator>>(istream& in, Date& d) {
        char comma;
        in >> d.year >> comma >> d.month >> comma >> d.day;
        return in;
    }

    string toString() const {
        string y = to_string(year);
        string m = (month < 10 ? "0" : "") + to_string(month);
        string da = (day < 10 ? "0" : "") + to_string(day);
        return y + "-" + m + "-" + da;
    }
};

class Student {
private:
    string name;
    string id;
    string gender;
    Date birthDate;
    string dorm;
public:
    Student() {}
    Student(string n, string i, string g, Date d, string dr)
        : name(n), id(i), gender(g), birthDate(d), dorm(dr) {}

    string getName() const { return name; }
    string getId() const { return id; }
    string getGender() const { return gender; }
    Date getBirthDate() const { return birthDate; }
    string getDorm() const { return dorm; }
    int getAge() const { return birthDate.calcAge(); }

    void setName(const string& n) { name = n; }
    void setId(const string& i) { id = i; }
    void setGender(const string& g) { gender = g; }
    void setBirthDate(const Date& d) { birthDate = d; }
    void setDorm(const string& d) { dorm = d; }

    friend ostream& operator<<(ostream& out, const Student& s) {
        out << s.name << "," << s.id << "," << s.gender << ","
            << s.birthDate << "," << s.dorm;
        return out;
    }

    friend istream& operator>>(istream& in, Student& s) {
        string line;
        if (!getline(in, line)) return in;
        stringstream ss(line);
        getline(ss, s.name, ',');
        getline(ss, s.id, ',');
        getline(ss, s.gender, ',');

        string yearStr, monthStr, dayStr;
        getline(ss, yearStr, ',');
        getline(ss, monthStr, ',');
        getline(ss, dayStr, ',');

        int y = 2000, m = 1, d = 1;
        safeStoi(yearStr, y);
        safeStoi(monthStr, m);
        safeStoi(dayStr, d);
        s.birthDate.setDate(y, m, d);

        getline(ss, s.dorm, ',');
        return in;
    }

    void display() const {
        cout << left << setw(10) << name
             << setw(12) << id
             << setw(6) << gender
             << setw(12) << birthDate.toString()
             << setw(6) << getAge()
             << setw(10) << dorm << endl;
    }
};

class StudentManager {
private:
    vector<Student> students;
    string filename;
public:
    StudentManager(string fname = "students.csv") : filename(fname) {}

    void loadFromFile() {
        ifstream infile(filename);
        if (!infile.is_open()) {
            cout << "[提示] 数据文件 " << filename << " 不存在，将创建新文件。" << endl;
            return;
        }
        students.clear();
        string line;
        while (getline(infile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            Student s;
            ss >> s;
            students.push_back(s);
        }
        infile.close();
        cout << "[成功] 已从 " << filename << " 加载 " << students.size() << " 条记录。" << endl;
    }

    void saveToFile() const {
        ofstream outfile(filename);
        if (!outfile.is_open()) {
            cout << "[错误] 无法打开文件 " << filename << " 进行写入。" << endl;
            return;
        }
        for (const auto& s : students) {
            outfile << s << endl;
        }
        outfile.close();
        cout << "[成功] 已保存 " << students.size() << " 条记录到 " << filename << "。" << endl;
    }

    bool isIdExists(const string& id) const {
        for (const auto& s : students) {
            if (s.getId() == id) return true;
        }
        return false;
    }

    void addStudent() {
        cout << "\n===== 添加学生 =====" << endl;
        Student s;
        string name, id, gender, dorm;
        int year, month, day;

        cout << "姓名: ";
        cin >> name;
        s.setName(name);

        cout << "学号: ";
        cin >> id;
        if (isIdExists(id)) {
            cout << "[错误] 学号 " << id << " 已存在！" << endl;
            return;
        }
        s.setId(id);

        cout << "性别(男/女): ";
        cin >> gender;
        if (gender != "男" && gender != "女") {
            cout << "[错误] 性别只能为 男 或 女！" << endl;
            return;
        }
        s.setGender(gender);

        cout << "出生日期(年 月 日): ";
        if (!(cin >> year >> month >> day)) {
            clearInputBuffer();
            cout << "[错误] 日期格式不正确！" << endl;
            return;
        }
        Date d(year, month, day);
        if (!d.isValid()) {
            cout << "[错误] 日期不合法！" << endl;
            return;
        }
        s.setBirthDate(d);

        cout << "宿舍号: ";
        cin >> dorm;
        s.setDorm(dorm);

        students.push_back(s);
        cout << "[成功] 添加成功！" << endl;
    }

    void modifyByName() {
        cout << "\n===== 根据姓名修改记录 =====" << endl;
        string name;
        cout << "请输入要修改的学生姓名: ";
        cin >> name;

        for (auto& s : students) {
            if (s.getName() == name) {
                cout << "找到记录：" << endl;
                s.display();

                cout << "\n请输入新信息（直接回车保留原值）:" << endl;
                string newVal;
                int y, m, d;

                cin.ignore();
                cout << "新姓名(" << s.getName() << "): ";
                getline(cin, newVal);
                if (!newVal.empty()) s.setName(newVal);

                cout << "新学号(" << s.getId() << "): ";
                getline(cin, newVal);
                if (!newVal.empty()) {
                    if (isIdExists(newVal) && newVal != s.getId()) {
                        cout << "[错误] 学号已存在！" << endl;
                        return;
                    }
                    s.setId(newVal);
                }

                cout << "新性别(" << s.getGender() << "): ";
                getline(cin, newVal);
                if (!newVal.empty()) {
                    if (newVal != "男" && newVal != "女") {
                        cout << "[错误] 性别无效，修改取消。" << endl;
                        return;
                    }
                    s.setGender(newVal);
                }

                cout << "新出生年(" << s.getBirthDate().getYear() << "): ";
                string yStr;
                getline(cin, yStr);
                if (!yStr.empty()) {
                    if (!safeStoi(yStr, y)) {
                        cout << "[错误] 年份无效，修改取消。" << endl;
                        return;
                    }
                } else {
                    y = s.getBirthDate().getYear();
                }

                cout << "新出生月(" << s.getBirthDate().getMonth() << "): ";
                string mStr;
                getline(cin, mStr);
                if (!mStr.empty()) {
                    if (!safeStoi(mStr, m)) {
                        cout << "[错误] 月份无效，修改取消。" << endl;
                        return;
                    }
                } else {
                    m = s.getBirthDate().getMonth();
                }

                cout << "新出生日(" << s.getBirthDate().getDay() << "): ";
                string dStr;
                getline(cin, dStr);
                if (!dStr.empty()) {
                    if (!safeStoi(dStr, d)) {
                        cout << "[错误] 日期无效，修改取消。" << endl;
                        return;
                    }
                } else {
                    d = s.getBirthDate().getDay();
                }

                Date newDate(y, m, d);
                if (!newDate.isValid()) {
                    cout << "[错误] 日期不合法，修改取消。" << endl;
                    return;
                }
                s.setBirthDate(newDate);

                cout << "新宿舍号(" << s.getDorm() << "): ";
                getline(cin, newVal);
                if (!newVal.empty()) s.setDorm(newVal);

                cout << "[成功] 修改成功！" << endl;
                return;
            }
        }
        cout << "未找到姓名为 " << name << " 的学生。" << endl;
    }

    void displayAll() const {
        if (students.empty()) {
            cout << "暂无学生记录。" << endl;
            return;
        }
        cout << "\n===== 所有学生记录 =====" << endl;
        cout << left << setw(10) << "姓名" << setw(12) << "学号" << setw(6) << "性别"
             << setw(12) << "出生日期" << setw(6) << "年龄" << setw(10) << "宿舍号" << endl;
        cout << string(60, '-') << endl;
        for (const auto& s : students) {
            s.display();
        }
    }

    void findStudent() const {
        cout << "\n===== 查找记录 =====" << endl;
        cout << "1. 按学号查找" << endl;
        cout << "2. 按姓名查找" << endl;
        cout << "请选择: ";
        int choice;
        if (!(cin >> choice)) {
            clearInputBuffer();
            cout << "输入无效。" << endl;
            return;
        }

        if (choice == 1) {
            string id;
            cout << "请输入学号: ";
            cin >> id;
            for (const auto& s : students) {
                if (s.getId() == id) {
                    cout << "找到记录：" << endl;
                    cout << left << setw(10) << "姓名" << setw(12) << "学号" << setw(6) << "性别"
                         << setw(12) << "出生日期" << setw(6) << "年龄" << setw(10) << "宿舍号" << endl;
                    cout << string(60, '-') << endl;
                    s.display();
                    return;
                }
            }
            cout << "未找到学号为 " << id << " 的学生。" << endl;
        } else if (choice == 2) {
            string name;
            cout << "请输入姓名: ";
            cin >> name;
            bool found = false;
            for (const auto& s : students) {
                if (s.getName() == name) {
                    if (!found) {
                        cout << "找到记录：" << endl;
                        cout << left << setw(10) << "姓名" << setw(12) << "学号" << setw(6) << "性别"
                             << setw(12) << "出生日期" << setw(6) << "年龄" << setw(10) << "宿舍号" << endl;
                        cout << string(60, '-') << endl;
                        found = true;
                    }
                    s.display();
                }
            }
            if (!found) cout << "未找到姓名为 " << name << " 的学生。" << endl;
        } else {
            cout << "无效选择。" << endl;
        }
    }

    void deleteStudent() {
        cout << "\n===== 删除记录 =====" << endl;
        cout << "1. 按学号删除" << endl;
        cout << "2. 按姓名删除" << endl;
        cout << "请选择: ";
        int choice;
        if (!(cin >> choice)) {
            clearInputBuffer();
            cout << "输入无效。" << endl;
            return;
        }

        if (choice == 1) {
            string id;
            cout << "请输入学号: ";
            cin >> id;
            for (auto it = students.begin(); it != students.end(); ++it) {
                if (it->getId() == id) {
                    cout << "找到记录：" << endl;
                    it->display();
                    cout << "确认删除吗？(y/n): ";
                    char confirm;
                    cin >> confirm;
                    if (confirm == 'y' || confirm == 'Y') {
                        students.erase(it);
                        cout << "[成功] 删除成功！" << endl;
                    } else {
                        cout << "已取消删除。" << endl;
                    }
                    return;
                }
            }
            cout << "未找到学号为 " << id << " 的学生。" << endl;
        } else if (choice == 2) {
            string name;
            cout << "请输入姓名: ";
            cin >> name;
            vector<vector<Student>::iterator> toDelete;
            for (auto it = students.begin(); it != students.end(); ++it) {
                if (it->getName() == name) toDelete.push_back(it);
            }
            if (toDelete.empty()) {
                cout << "未找到姓名为 " << name << " 的学生。" << endl;
                return;
            }
            if (toDelete.size() > 1) {
                cout << "找到 " << toDelete.size() << " 条记录，全部删除吗？(y/n): ";
            } else {
                cout << "确认删除吗？(y/n): ";
            }
            char confirm;
            cin >> confirm;
            if (confirm == 'y' || confirm == 'Y') {
                for (int i = (int)toDelete.size() - 1; i >= 0; --i) {
                    students.erase(toDelete[i]);
                }
                cout << "[成功] 删除成功！" << endl;
            } else {
                cout << "已取消删除。" << endl;
            }
        } else {
            cout << "无效选择。" << endl;
        }
    }
};

int main() {
    system("chcp 65001 > nul");

    StudentManager manager;
    manager.loadFromFile();

    int choice;
    do {
        cout << "\n===== 学生信息管理系统 =====" << endl;
        cout << "1. 添加记录" << endl;
        cout << "2. 根据姓名修改记录" << endl;
        cout << "3. 显示所有记录" << endl;
        cout << "4. 查找记录" << endl;
        cout << "5. 删除记录" << endl;
        cout << "6. 保存文件" << endl;
        cout << "0. 退出" << endl;
        cout << "请选择操作: ";

        if (!(cin >> choice)) {
            clearInputBuffer();
            cout << "输入无效，请重新输入。" << endl;
            continue;
        }

        switch (choice) {
            case 1: manager.addStudent(); break;
            case 2: manager.modifyByName(); break;
            case 3: manager.displayAll(); break;
            case 4: manager.findStudent(); break;
            case 5: manager.deleteStudent(); break;
            case 6: manager.saveToFile(); break;
            case 0: cout << "再见！" << endl; break;
            default: cout << "无效选择，请重新输入。" << endl;
        }
    } while (choice != 0);

    return 0;
}
