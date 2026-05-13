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

// ==================== 杈呭姪鍑芥暟 ====================
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

// ==================== Date 绫?====================
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

// ==================== Student 绫?====================
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

// ==================== StudentManager 绫?====================
class StudentManager {
private:
    vector<Student> students;
    string filename;
public:
    StudentManager(string fname = "students.csv") : filename(fname) {}

    void loadFromFile() {
        ifstream infile(filename);
        if (!infile.is_open()) {
            cout << "鎻愮ず锛氭暟鎹枃浠?" << filename << " 涓嶅瓨鍦紝灏嗗垱寤烘柊鏂囦欢銆? << endl;
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
        cout << "鎴愬姛浠?" << filename << " 鍔犺浇 " << students.size() << " 鏉¤褰曘€? << endl;
    }

    void saveToFile() const {
        ofstream outfile(filename);
        if (!outfile.is_open()) {
            cout << "閿欒锛氭棤娉曟墦寮€鏂囦欢 " << filename << " 杩涜鍐欏叆銆? << endl;
            return;
        }
        for (const auto& s : students) {
            outfile << s << endl;
        }
        outfile.close();
        cout << "鎴愬姛淇濆瓨 " << students.size() << " 鏉¤褰曞埌 " << filename << "銆? << endl;
    }

    bool isIdExists(const string& id) const {
        for (const auto& s : students) {
            if (s.getId() == id) return true;
        }
        return false;
    }

    void addStudent() {
        cout << "\n===== 娣诲姞瀛︾敓璁板綍 =====" << endl;
        Student s;
        string name, id, gender, dorm;
        int year, month, day;

        cout << "璇疯緭鍏ュ鍚? ";
        cin >> name;
        s.setName(name);

        cout << "璇疯緭鍏ュ鍙? ";
        cin >> id;
        if (isIdExists(id)) {
            cout << "閿欒锛氬鍙?" << id << " 宸插瓨鍦紒" << endl;
            return;
        }
        s.setId(id);

        cout << "璇疯緭鍏ユ€у埆(鐢?濂?: ";
        cin >> gender;
        if (gender != "鐢? && gender != "濂?) {
            cout << "閿欒锛氭€у埆鍙兘涓?鐢?鎴?濂筹紒" << endl;
            return;
        }
        s.setGender(gender);

        cout << "璇疯緭鍏ュ嚭鐢熸棩鏈?骞?鏈?鏃?: ";
        if (!(cin >> year >> month >> day)) {
            clearInputBuffer();
            cout << "閿欒锛氭棩鏈熻緭鍏ユ牸寮忎笉姝ｇ‘锛? << endl;
            return;
        }
        Date d(year, month, day);
        if (!d.isValid()) {
            cout << "閿欒锛氭棩鏈熶笉鍚堟硶锛? << endl;
            return;
        }
        s.setBirthDate(d);

        cout << "璇疯緭鍏ュ鑸嶅彿: ";
        cin >> dorm;
        s.setDorm(dorm);

        students.push_back(s);
        cout << "娣诲姞鎴愬姛锛? << endl;
    }

    void modifyByName() {
        cout << "\n===== 鏍规嵁濮撳悕淇敼璁板綍 =====" << endl;
        string name;
        cout << "璇疯緭鍏ヨ淇敼鐨勫鐢熷鍚? ";
        cin >> name;

        for (auto& s : students) {
            if (s.getName() == name) {
                cout << "鎵惧埌璁板綍锛? << endl;
                s.display();

                cout << "\n璇疯緭鍏ユ柊淇℃伅锛堢洿鎺ュ洖杞︿繚鐣欏師鍊硷級:" << endl;
                string newVal;
                int y, m, d;

                cout << "鏂板鍚?" << s.getName() << "): ";
                cin.ignore();
                getline(cin, newVal);
                if (!newVal.empty()) s.setName(newVal);

                cout << "鏂板鍙?" << s.getId() << "): ";
                getline(cin, newVal);
                if (!newVal.empty()) {
                    if (isIdExists(newVal) && newVal != s.getId()) {
                        cout << "閿欒锛氬鍙峰凡瀛樺湪锛? << endl;
                        return;
                    }
                    s.setId(newVal);
                }

                cout << "鏂版€у埆(" << s.getGender() << "): ";
                getline(cin, newVal);
                if (!newVal.empty()) {
                    if (newVal != "鐢? && newVal != "濂?) {
                        cout << "閿欒锛氭€у埆鏃犳晥锛屼慨鏀瑰彇娑堛€? << endl;
                        return;
                    }
                    s.setGender(newVal);
                }

                cout << "鏂板嚭鐢熸棩鏈?骞?" << s.getBirthDate().getYear() << "): ";
                string yStr;
                getline(cin, yStr);
                if (!yStr.empty()) {
                    if (!safeStoi(yStr, y)) {
                        cout << "閿欒锛氬勾浠借緭鍏ユ棤鏁堬紝淇敼鍙栨秷銆? << endl;
                        return;
                    }
                } else {
                    y = s.getBirthDate().getYear();
                }

                cout << "鏂板嚭鐢熸棩鏈?鏈?" << s.getBirthDate().getMonth() << "): ";
                string mStr;
                getline(cin, mStr);
                if (!mStr.empty()) {
                    if (!safeStoi(mStr, m)) {
                        cout << "閿欒锛氭湀浠借緭鍏ユ棤鏁堬紝淇敼鍙栨秷銆? << endl;
                        return;
                    }
                } else {
                    m = s.getBirthDate().getMonth();
                }

                cout << "鏂板嚭鐢熸棩鏈?鏃?" << s.getBirthDate().getDay() << "): ";
                string dStr;
                getline(cin, dStr);
                if (!dStr.empty()) {
                    if (!safeStoi(dStr, d)) {
                        cout << "閿欒锛氭棩鏈熻緭鍏ユ棤鏁堬紝淇敼鍙栨秷銆? << endl;
                        return;
                    }
                } else {
                    d = s.getBirthDate().getDay();
                }

                Date newDate(y, m, d);
                if (!newDate.isValid()) {
                    cout << "閿欒锛氭棩鏈熶笉鍚堟硶锛屼慨鏀瑰彇娑堛€? << endl;
                    return;
                }
                s.setBirthDate(newDate);

                cout << "鏂板鑸嶅彿(" << s.getDorm() << "): ";
                getline(cin, newVal);
                if (!newVal.empty()) s.setDorm(newVal);

                cout << "淇敼鎴愬姛锛? << endl;
                return;
            }
        }
        cout << "鏈壘鍒板鍚嶄负 " << name << " 鐨勫鐢熴€? << endl;
    }

    void displayAll() const {
        if (students.empty()) {
            cout << "鏆傛棤瀛︾敓璁板綍銆? << endl;
            return;
        }
        cout << "\n===== 鎵€鏈夊鐢熻褰?=====" << endl;
        cout << left << setw(10) << "濮撳悕" << setw(12) << "瀛﹀彿" << setw(6) << "鎬у埆"
             << setw(12) << "鍑虹敓鏃ユ湡" << setw(6) << "骞撮緞" << setw(10) << "瀹胯垗鍙? << endl;
        cout << string(60, '-') << endl;
        for (const auto& s : students) {
            s.display();
        }
    }

    void findStudent() const {
        cout << "\n===== 鏌ユ壘璁板綍 =====" << endl;
        cout << "1. 鎸夊鍙锋煡鎵? << endl;
        cout << "2. 鎸夊鍚嶆煡鎵? << endl;
        cout << "璇烽€夋嫨: ";
        int choice;
        if (!(cin >> choice)) {
            clearInputBuffer();
            cout << "杈撳叆鏃犳晥銆? << endl;
            return;
        }

        if (choice == 1) {
            string id;
            cout << "璇疯緭鍏ュ鍙? ";
            cin >> id;
            for (const auto& s : students) {
                if (s.getId() == id) {
                    cout << "鎵惧埌璁板綍锛? << endl;
                    cout << left << setw(10) << "濮撳悕" << setw(12) << "瀛﹀彿" << setw(6) << "鎬у埆"
                         << setw(12) << "鍑虹敓鏃ユ湡" << setw(6) << "骞撮緞" << setw(10) << "瀹胯垗鍙? << endl;
                    cout << string(60, '-') << endl;
                    s.display();
                    return;
                }
            }
            cout << "鏈壘鍒板鍙蜂负 " << id << " 鐨勫鐢熴€? << endl;
        } else if (choice == 2) {
            string name;
            cout << "璇疯緭鍏ュ鍚? ";
            cin >> name;
            bool found = false;
            for (const auto& s : students) {
                if (s.getName() == name) {
                    if (!found) {
                        cout << "鎵惧埌璁板綍锛? << endl;
                        cout << left << setw(10) << "濮撳悕" << setw(12) << "瀛﹀彿" << setw(6) << "鎬у埆"
                             << setw(12) << "鍑虹敓鏃ユ湡" << setw(6) << "骞撮緞" << setw(10) << "瀹胯垗鍙? << endl;
                        cout << string(60, '-') << endl;
                        found = true;
                    }
                    s.display();
                }
            }
            if (!found) cout << "鏈壘鍒板鍚嶄负 " << name << " 鐨勫鐢熴€? << endl;
        } else {
            cout << "鏃犳晥閫夋嫨銆? << endl;
        }
    }

    void deleteStudent() {
        cout << "\n===== 鍒犻櫎璁板綍 =====" << endl;
        cout << "1. 鎸夊鍙峰垹闄? << endl;
        cout << "2. 鎸夊鍚嶅垹闄? << endl;
        cout << "璇烽€夋嫨: ";
        int choice;
        if (!(cin >> choice)) {
            clearInputBuffer();
            cout << "杈撳叆鏃犳晥銆? << endl;
            return;
        }

        if (choice == 1) {
            string id;
            cout << "璇疯緭鍏ュ鍙? ";
            cin >> id;
            for (auto it = students.begin(); it != students.end(); ++it) {
                if (it->getId() == id) {
                    cout << "鎵惧埌璁板綍锛? << endl;
                    it->display();
                    cout << "纭鍒犻櫎鍚楋紵(y/n): ";
                    char confirm;
                    cin >> confirm;
                    if (confirm == 'y' || confirm == 'Y') {
                        students.erase(it);
                        cout << "鍒犻櫎鎴愬姛锛? << endl;
                    } else {
                        cout << "宸插彇娑堝垹闄ゃ€? << endl;
                    }
                    return;
                }
            }
            cout << "鏈壘鍒板鍙蜂负 " << id << " 鐨勫鐢熴€? << endl;
        } else if (choice == 2) {
            string name;
            cout << "璇疯緭鍏ュ鍚? ";
            cin >> name;
            vector<vector<Student>::iterator> toDelete;
            for (auto it = students.begin(); it != students.end(); ++it) {
                if (it->getName() == name) toDelete.push_back(it);
            }
            if (toDelete.empty()) {
                cout << "鏈壘鍒板鍚嶄负 " << name << " 鐨勫鐢熴€? << endl;
                return;
            }
            if (toDelete.size() > 1) {
                cout << "鎵惧埌 " << toDelete.size() << " 鏉¤褰曪紝鍏ㄩ儴鍒犻櫎鍚楋紵(y/n): ";
            } else {
                cout << "纭鍒犻櫎鍚楋紵(y/n): ";
            }
            char confirm;
            cin >> confirm;
            if (confirm == 'y' || confirm == 'Y') {
                for (int i = (int)toDelete.size() - 1; i >= 0; --i) {
                    students.erase(toDelete[i]);
                }
                cout << "鍒犻櫎鎴愬姛锛? << endl;
            } else {
                cout << "宸插彇娑堝垹闄ゃ€? << endl;
            }
        } else {
            cout << "鏃犳晥閫夋嫨銆? << endl;
        }
    }
};

// ==================== 涓诲嚱鏁?====================
int main() {
    system("chcp 65001");

    StudentManager manager;
    manager.loadFromFile();

    int choice;
    do {
        cout << "\n===== 瀛︾敓淇℃伅绠＄悊绯荤粺 =====" << endl;
        cout << "1. 娣诲姞璁板綍" << endl;
        cout << "2. 鏍规嵁濮撳悕淇敼璁板綍" << endl;
        cout << "3. 鏄剧ず鎵€鏈夎褰? << endl;
        cout << "4. 鏌ユ壘璁板綍" << endl;
        cout << "5. 鍒犻櫎璁板綍" << endl;
        cout << "6. 淇濆瓨鏂囦欢" << endl;
        cout << "0. 閫€鍑? << endl;
        cout << "璇烽€夋嫨鎿嶄綔: ";

        if (!(cin >> choice)) {
            clearInputBuffer();
            cout << "杈撳叆鏃犳晥锛岃閲嶆柊杈撳叆銆? << endl;
            continue;
        }

        switch (choice) {
            case 1: manager.addStudent(); break;
            case 2: manager.modifyByName(); break;
            case 3: manager.displayAll(); break;
            case 4: manager.findStudent(); break;
            case 5: manager.deleteStudent(); break;
            case 6: manager.saveToFile(); break;
            case 0: cout << "鍐嶈锛? << endl; break;
            default: cout << "鏃犳晥閫夋嫨锛岃閲嶆柊杈撳叆銆? << endl;
        }
    } while (choice != 0);

    return 0;
}
