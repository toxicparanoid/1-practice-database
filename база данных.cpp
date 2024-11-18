#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Person {
    int id;
    string name;
    int age;
    string phone;

    string serialize() {
        return to_string(id) + "," + name + "," + to_string(age) + "," + phone;
    }

    static Person deserialize(const string& data) {
        istringstream iss(data);
        string token;
        Person p;

        getline(iss, token, ',');
        p.id = stoi(token);
        getline(iss, p.name, ',');
        getline(iss, token, ',');
        p.age = stoi(token);
        getline(iss, p.phone, ',');

        return p;
    }
};

class Database {
private:
    vector<Person> people;
    int next_id = 1;
    const string filename = "database.txt";

    void load() {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                Person p = Person::deserialize(line);
                people.push_back(p);
                if (p.id >= next_id) {
                    next_id = p.id + 1;
                }
            }
        }
        file.close();
    }

    void save() {
        ofstream file(filename);
        for (const auto& p : people) {
            file << p.serialize() << endl;
        }
        file.close();
    }

public:
    Database() {
        load();
    }

    ~Database() {
        save();
    }

    void insert(const string& name, int age, const string& phone) {
        Person p{ next_id++, name, age, phone };
        people.push_back(p);
    }

    void deleteById(int id) {
        auto it = remove_if(people.begin(), people.end(), [id](Person& p) { return p.id == id; });
        people.erase(it, people.end());
    }

    void select(const string& condition) {
        for (const auto& p : people) {
            bool matches = true;

            if (!condition.empty()) {
                string token;
                istringstream ss(condition);
                while (std::getline(ss, token, ' ')) {
                    if (token == "AND") continue; // Ignore AND
                    if (token == "OR") continue;  // Ignore OR

                    string field, op, value;
                    istringstream conditionStream(token);
                    getline(conditionStream, field, '=');
                    getline(conditionStream, op, '=');
                    getline(conditionStream, value);

                    if (field == "name") {
                        if (p.name != value) matches = false;
                    }
                    else if (field == "age") {
                        if (to_string(p.age) != value) matches = false;
                    }
                    else if (field == "phone") {
                        if (p.phone != value) matches = false;
                    }
                }
            }

            if (matches) {
                cout << "ID: " << p.id << ", Name: " << p.name << ", Age: " << p.age << ", Phone: " << p.phone << endl;
            }
        }
    }
};

int main() {
    Database db;

    while (true) {
        cout << "Команды: insert, delete, select, exit" << endl;
        string command;
        cin >> command;

        if (command == "insert") {
            string name, phone;
            int age;
            cout << "Введите имя: ";
            cin >> name;
            cout << "Введите возраст: ";

            cin >> age;
            cout << "Введите номер телефона: ";
            cin >> phone;
            db.insert(name, age, phone);
        }
        else if (command == "delete") {
            int id;
            cout << "Введите ID для удаления: ";
            cin >> id;
            db.deleteById(id);
        }
        else if (command == "select") {
            string condition;
            cout << "Введите условие или оставьте пустым: ";
            cin.ignore();
            getline(std::cin, condition);
            db.select(condition);
        }
        else if (command == "exit") {
            break;
        }
        else {
            cout << "Неизвестная команда!" << endl;
        }
    }

    return 0;
}
