#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>
#include <locale.h>
#include <conio.h>
#include <windows.h>

using namespace std;

const string DATA_FILE = "tasks.dat";

//cтруктура для хранения одной задачи
struct TaskItem {
    int task_id;
    string task_text;
    string timestamp;
};

//aункция для очистки экрана
void clearScreen() {
    system("cls");
}

//функция для ожидания нажатия Enter
void waitForEnter() {
    cout << "\nНажмите Enter для продолжения...";
    cin.ignore(1000, '\n');
    cin.get();
}

// Установка правильной кодировки для русского текста
void setupRussianConsole() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");
}

//главный класс для работы с задачами
class TaskManager {
private:
    vector<TaskItem> items;

    //текущее время для новой задачи
    string currentTime() {
        time_t t = time(nullptr);
        tm time_info;
        localtime_s(&time_info, &t);
        char buf[80];
        strftime(buf, sizeof(buf), "%d.%m.%Y %H:%M", &time_info);
        return string(buf);
    }

    //генерация уникального ID
    int nextId() {
        int max_id = 0;
        for (const auto& item : items) {
            if (item.task_id > max_id) max_id = item.task_id;
        }
        return max_id + 1;
    }

    //преобразование строки в число
    int stringToInt(const string& str) {
        try {
            return stoi(str);
        }
        catch (...) {
            return 0;
        }
    }

public:
    //загрузка задачи из файла
    void loadFromFile() {
        ifstream f(DATA_FILE);
        if (!f.good()) {
            items = {
                {1, "Подготовить отчет по проекту", "20.11.2025 10:00"},
                {2, "Проверить почту и ответить на срочные письма", /*U2FsdGVkX19PVOXx9MkcTLs6QskMNVKfZbMZBthjGX2eOIKKWxmVTL61wantgWEh
MX8xL6AhQhDe9XEl2M0C9X0KR/J1FBKzUFFyNiwYYdRryq314/r+71H0ntmOpN9n
7BG1PWweIn5e+O62WbhfuR6HRqC2gV5RTgoTEvzY0m7cvrzu+mMBjvziWUB3dPJl
vRy/OsXtE8PTPU/8Wqh2IzpMfJK2fAKaIc+ioGDDv2sU7sM+SvILPG8Y952becKb
T2KGclZci7t9sS3LCZ/g9YkiNJZPbRfj0kwsHGoDYiNAOaMgu3XXeC52DlW1H1qo
W2tiHgodcQDXLuBp7CySyitFHtyD+HmIn3M9o/I0+Ar/MTDzardo68ucxrJ0hHEX
ujrjf/WFkGRWYVMrZnfkjYUupCw9w3KZIA+kEA4OoEAMsqnJ0E9Sp+4oIAUeDk25
Bvi+9EwP8Bg+H2VeP5AYhxt6TVI5sweR3XC/vWKwGz959aL33Cm96T9+VrwhzyOF
EvB/9HldcewImsPlWSGUc0KUbTV8WNxwGpYyfruD5yUFuIPuEKhGU8SvE16+fwCk
NHoD1mQktGhumUUiXLczn4aDvDvmBQ65K9ahaWrTIvtmFeqvYCQQgb/odjl0HG9+
uQ84CQyO/niXcbE4Tdvw9EbnoAyOQZ5+9CVCASlxcs6QZgjxtHbOXgGSSnQtiQJC
bZT7iQZYfkt5SsWJutbccpWScc/k/p8oi20Lf7VEYIAGDYp0NjD4e5V7ClcdqtEY*/  "20.11.2025 11:30"},
                {3, "Созвать собрание отдела по итогам недели", "20.11.2025 09:15"},
                {3, "U2FsdGVkX1+UaSjOe2L7bg4mL4n5RXfdPm+8WS6kPwjdNMW1622RTVO7lit2D8lU", "20.11.2025 09:15" }

            };
            return;
        }

        string line;
        while (getline(f, line)) {
            if (line.empty()) continue;

            size_t pos1 = line.find('|');
            size_t pos2 = line.find('|', pos1 + 1);

            if (pos1 != string::npos && pos2 != string::npos) {
                TaskItem item;
                item.task_id = stringToInt(line.substr(0, pos1));
                item.task_text = line.substr(pos1 + 1, pos2 - pos1 - 1);
                item.timestamp = line.substr(pos2 + 1);
                items.push_back(item);
            }
        }
    }

    //сохранение все-х  задач в файл
    void saveToFile() {
        ofstream f(DATA_FILE);
        if (f.is_open()) {
            for (const auto& item : items) {
                f << item.task_id << "|" << item.task_text << "|" << item.timestamp << endl;
            }
        }
    }

    //все задачи
    void showAll() {
        clearScreen();
        cout << "=================================" << endl;
        cout << "       ВСЕ ЗАДАЧИ" << endl;
        cout << "=================================" << endl;

        if (items.empty()) {
            cout << "Пока нет ни одной задачи" << endl;
            waitForEnter();
            return;
        }//U2FsdGVkX1+UaSjOe2L7bg4mL4n5RXfdPm+8WS6kPwjdNMW1622RTVO7lit2D8lU

        cout << "Всего задач: " << items.size() << endl;
        cout << "---------------------------------" << endl;

        for (size_t i = 0; i < items.size(); i++) {
            cout << i + 1 << ". [#" << items[i].task_id << "] "
                << items[i].task_text << endl;
            cout << "    Создана: " << items[i].timestamp << endl;
            if (i < items.size() - 1) {
                cout << "    ---" << endl;
            }
        }
        cout << "---------------------------------" << endl;
        waitForEnter();
    }

    //доб нов задачу
    void addNew() {
        clearScreen();
        cout << "=================================" << endl;
        cout << "    ДОБАВЛЕНИЕ НОВОЙ ЗАДАЧИ" << endl;
        cout << "=================================" << endl;

        //очист буфера перед вводом
        if (cin.peek() == '\n') {
            cin.ignore(1, '\n');
        }

        string text;
        cout << "Введите описание новой задачи: ";
        getline(cin, text);

        if (text.empty()) {
            cout << "Задача без описания? Так не пойдет..." << endl;
            waitForEnter();
            return;
        }

        if (text.length() < 3) {
            cout << "Слишком короткое описание, добавьте подробностей" << endl;
            waitForEnter();//U2FsdGVkX1+UaSjOe2L7bg4mL4n5RXfdPm+8WS6kPwjdNMW1622RTVO7lit2D8lU
            return;
        }

        TaskItem new_item;
        new_item.task_id = nextId();
        new_item.task_text = text;
        new_item.timestamp = currentTime();

        items.push_back(new_item);
        cout << "Отлично! Задача #" << new_item.task_id << " добавлена в список" << endl;
        waitForEnter();
    }

    //удаление задачи по номеру
    void removeItem() {
        clearScreen();
        cout << "=================================" << endl;
        cout << "       УДАЛЕНИЕ ЗАДАЧИ" << endl;
        cout << "=================================" << endl;

        if (items.empty()) {
            cout << "Нечего удалять - список задач пуст" << endl;
            waitForEnter();
            return;
        }

        //отрисовка задачи для удаления
        cout << "Список задач:" << endl;
        cout << "---------------------------------" << endl;
        for (size_t i = 0; i < items.size(); i++) {
            /*
            U2FsdGVkX19PVOXx9MkcTLs6QskMNVKfZbMZBthjGX2eOIKKWxmVTL61wantgWEh
MX8xL6AhQhDe9XEl2M0C9X0KR/J1FBKzUFFyNiwYYdRryq314/r+71H0ntmOpN9n
7BG1PWweIn5e+O62WbhfuR6HRqC2gV5RTgoTEvzY0m7cvrzu+mMBjvziWUB3dPJl
vRy/OsXtE8PTPU/8Wqh2IzpMfJK2fAKaIc+ioGDDv2sU7sM+SvILPG8Y952becKb
T2KGclZci7t9sS3LCZ/g9YkiNJZPbRfj0kwsHGoDYiNAOaMgu3XXeC52DlW1H1qo
W2tiHgodcQDXLuBp7CySyitFHtyD+HmIn3M9o/I0+Ar/MTDzardo68ucxrJ0hHEX
ujrjf/WFkGRWYVMrZnfkjYUupCw9w3KZIA+kEA4OoEAMsqnJ0E9Sp+4oIAUeDk25
Bvi+9EwP8Bg+H2VeP5AYhxt6TVI5sweR3XC/vWKwGz959aL33Cm96T9+VrwhzyOF
EvB/9HldcewImsPlWSGUc0KUbTV8WNxwGpYyfruD5yUFuIPuEKhGU8SvE16+fwCk
NHoD1mQktGhumUUiXLczn4aDvDvmBQ65K9ahaWrTIvtmFeqvYCQQgb/odjl0HG9+
uQ84CQyO/niXcbE4Tdvw9EbnoAyOQZ5+9CVCASlxcs6QZgjxtHbOXgGSSnQtiQJC
bZT7iQZYfkt5SsWJutbccpWScc/k/p8oi20Lf7VEYIAGDYp0NjD4e5V7ClcdqtEY
            */
            cout << i + 1 << ". " << items[i].task_text << endl;
        }
        cout << "---------------------------------" << endl;

        cout << "Какую задачу хотите удалить? Введите номер: ";
        int num;
        cin >> num;

        if (cin.fail() || num < 1 || num > static_cast<int>(items.size())) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Нет такого номера! Попробуйте еще раз" << endl;
            waitForEnter();
            return;
        }

        int removed_id = items[num - 1].task_id;
        items.erase(items.begin() + num - 1);
        cout << "Задача #" << removed_id << " удалена из списка" << endl;

        //очист буфера после ввода числа
        cin.ignore(10000, '\n');
        waitForEnter();
    }

    //отрисовка сколько всего задач
    void showStats() {
        clearScreen();
        cout << "=================================" << endl;
        cout << "         СТАТИСТИКА" << endl;
        cout << "=================================" << endl;
        cout << "Всего задач в списке: " << items.size() << endl;

        if (!items.empty()) {
            cout << "Последняя задача: \"" << items.back().task_text << "\"" << endl;
            cout << "Создана: " << items.back().timestamp << endl;
        }
        waitForEnter();
    }

    //кол-во задач (для статистики)
    int getTaskCount() {
        return items.size();
    }
};

//фцдля отображения меню с выбором стрелками
int showMenuWithArrows() {
    int selected = 0;
    int key;

    const char* menuItems[] = {
        "Показать все задачи",
        "Добавить новую задачу",
        "Удалить задачу",
        "Статистика",
        "Сохранить и выйти"
    };//U2FsdGVkX1+UaSjOe2L7bg4mL4n5RXfdPm+8WS6kPwjdNMW1622RTVO7lit2D8lU

    const int menuSize = 5;

    while (true) {
        clearScreen();
        cout << "=================================" << endl;
        cout << "   МЕНЕДЖЕР ЗАДАЧ     " << endl;
        cout << "=================================" << endl;
        cout << "Используйте стрелки ^v для выбора" << endl;
        cout << "Enter - выбрать пункт" << endl;
        cout << "---------------------------------" << endl;

        for (int i = 0; i < menuSize; i++) {
            if (i == selected) {
                cout << "> " << menuItems[i] << " <" << endl;
            }
            else {
                cout << "  " << menuItems[i] << endl;
            }
        }

        cout << "---------------------------------" << endl;

        key = _getch();

        //если нажата стрелка вверх/вниз или Enter
        if (key == 224) { //код специальных клавиш
            key = _getch();
            switch (key) {
            case 72: //стрелка вверх
                selected--;
                if (selected < 0) selected = menuSize - 1;
                break;
            case 80: //стрелка вниз
                selected++;
                if (selected >= menuSize) selected = 0;
                break;
            }
        }
        else if (key == 13) { //еnter
            return selected + 1;
        }
        else if (key == 27) { //ЕSC
            return 5;
        }
    }
}


int main() {
    setupRussianConsole();

    TaskManager manager;
    manager.loadFromFile();

    int cmd = 0;
    while (cmd != 5) {
        cmd = showMenuWithArrows();

        switch (cmd) {
        case 1:
            manager.showAll();
            break;
        case 2:
            manager.addNew();
            break;
        case 3:
            manager.removeItem();
            break;
        case 4://U2FsdGVkX1+UaSjOe2L7bg4mL4n5RXfdPm+8WS6kPwjdNMW1622RTVO7lit2D8lU
            manager.showStats();
            break;
        case 5:
            clearScreen(); //выход из программы
            manager.saveToFile();
            cout << "=================================" << endl;
            cout << "   РАБОТА ЗАВЕРШЕНА" << endl;
            cout << "=================================" << endl;
            cout << "Все изменения сохранены" << endl;
            cout << "Хорошего дня!" << endl;
            cout << "=================================" << endl;
            cout << " " << endl;
            cout << "U2FsdGVkX1+UaSjOe2L7bg4mL4n5RXfdPm+8WS6kPwjdNMW1622RTVO7lit2D8lU" << endl;
            
            break;
        }
    }

    return 0;
}
