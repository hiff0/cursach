#include <iostream>
#include <locale>
#include <string>
#include <fstream>
#include <windows.h>
#include <string>

using namespace std;

/*1. Поля данных: фамилия, год рождения, пол, образование (среднее,
высшее), год поступления на работу. Найти самого старшего сотрудника среди
мужчин. Вывести список молодых специалистов (до 28 лет) с высшим
образованием*/

struct Sotrudnik
{
    string surname;
    int year;//Год рождения
    bool man;
    bool education;
    int work_year;//Год поступленя на работу
};

class Node //Класс элемента
{
private:
    Sotrudnik data;//Данные сотрудника
    Node *pNext;//Указатель на следующий элемент

public:
    Node()
    {
        data = {"qwerty", 1990, 0, 1, 2010};
        pNext = nullptr;
    };
    Node(Sotrudnik d, Node *nx = nullptr)
    {
        data = d;
        pNext = nx;
    };
    Node(string sn, int y, bool m, bool e, int wy)
    {
        data.surname = sn;
        data.year = y;
        data.man = m;
        data.education = e;
        data.work_year = wy;
    }

    friend ostream &operator << (ostream &stream, Node &A);
    friend class List;
};

ostream &operator << (ostream &stream, Node &A)
{
    stream << A.data.surname << "\t| " << A.data.year << "\t  | " << (A.data.man?"Мужчина":"Женщина");
	stream << "\t| " << (A.data.education?"Высшее":"Среднее") << "\t| " <<  A.data.work_year << endl;
	return stream;
}

class List //Класс для методов
{
private:
    Node *head;//Указатель на первый элемент
    int Size;//Количество элементов
public:
    List()
    {
        Size = 0;
        head = NULL;
    };
    ~List(){};
    int getSize(){return Size;}; // Количество элементов

    void addToBeggin(Sotrudnik);
    void addToEnd(Sotrudnik);
    void Insert(Sotrudnik, int);

    //Sotrudnik operator[](const int index);

    void deleteBeggin();
    void deleteElement(int);
    void clearElement();

    void Print();

    bool saveBinary(string);
    bool loadBinary(string);

    void oldSotrudnik();
    void youngSotrudnik();

};

void List::Print()//Вывод
{
    Node *p;
    p = head;
    int i = 1;
    cout <<"N | " << "Фамилия" << " | " << "Год рождения" << " | " << "Пол" << " | " << "Образование" << " | " << "Год поступления на работу" << endl;
	cout <<"--------------------------------------------------------------------------------------------"<<endl;
	while(p != nullptr)
    {
        cout << i++ << " | " << *p;
        p = p->pNext;
    }
    cout << endl;
}

void List::Insert(Sotrudnik data, int index)//Добавление сотрудника в любое место
{
    if(Size == 0)
        addToBeggin(data);
    else
    {
        if(index-1 == 0)
            addToBeggin(data);
        else
        {
            Node *previous = this->head;
            for(int i = 0;i < index-1;i++)
                previous = previous->pNext;

        Node *newNode = new Node(data, previous->pNext);
        previous->pNext = newNode;
        Size++;
        }
    }
}

void List::deleteBeggin()//Удаление первого элемента
{
    Node *tmp = head;
    head = head->pNext;
    delete tmp;
    Size--;
}

void List::deleteElement(int index)//Удаление конкретного элемента в списке
{
    Node *previous = this->head;
    Node *del;
    for(int i = 0;i < index-2;i++)
        previous = previous->pNext;
    del = previous->pNext;
    previous->pNext = del->pNext;
    delete del;
    Size--;
    cout << "Сотрудник N " << index << " удален" << endl;
}

void List::clearElement()//Удаление всех элементов
{
    {
        while(Size)
            deleteBeggin();
        cout << "Все элементы удалены" << endl;
    }
}

void List::addToBeggin(Sotrudnik data)//Добавление элемента в начало списка
{
    head = new Node(data, head);
    Size++;
    cout << "Сотрудник добавлен в начало" << endl;
}

void List::addToEnd(Sotrudnik data)//Добавление элемента в конец
{
   if(head == nullptr)
   {
       head = new Node(data);
   }
   else
   {
       Node *current = this->head;
       while(current->pNext != nullptr)
       {
           current = current->pNext;
       }
       current->pNext = new Node(data);
       Size++;
       cout << "Сотрудник добавлен в конец" << endl;
   }
}

/*Sotrudnik List::operator[](const int index)//Перезагрузка [] ддя взятия конкретного элемента
{
    int i = 0;
    Node *current = this->head;
    while(current != nullptr)
    {
        if(i == index)
            return current->data;
        current = current->pNext;
        i++;
    }
}*/

bool List::saveBinary(string filename)
{
    ofstream out(filename, ios_base::binary|ios_base::trunc);
    if(!out)
        return false;
    Node *p = head;
    out.write((char*)&Size,sizeof(int));
    while(p!=NULL)
    {
        int tmp = p->data.surname.size();
        out.write((char*)&tmp, sizeof(int));
        out.write(p->data.surname.c_str(), tmp);
        out.write((char*)&p->data.year, sizeof(int));
        out.write((char*)&p->data.man, sizeof(bool));
        out.write((char*)&p->data.education, sizeof(bool));
        out.write((char*)&p->data.work_year, sizeof(int));
        p = p->pNext;
    }
    return true;
}

bool List::loadBinary(string filename)
{
    ifstream in(filename, ios_base::binary);
    if(!in)
        return false;
    int lsize;
    in.read((char*)&lsize,sizeof(int));
    for(int i = 0;i < lsize;i++)
    {
        Sotrudnik tmp = {"",0,true,true,0};
        int p;
        in.read((char*)&p,sizeof(int));
        char* temp = new char[p+1];
        in.read(temp,p);

        temp[p]='\0';
        tmp.surname = temp;
        delete[] temp;
        in.read((char*)&tmp.year,sizeof(int));
        in.read((char*)&tmp.man,sizeof(bool));
        in.read((char*)&tmp.education,sizeof(bool));
        in.read((char*)&tmp.work_year,sizeof(int));
        addToEnd(tmp);
    }
    return true;
}

void List::oldSotrudnik()//Самый старший сотрудник среди мужчин
{
    Node *current = head;
    Node *maxYear = head;
    cout << "Самый старший сотрудник среди мужчин" << endl << endl;
    cout <<"N | " << "Фамилия" << " | " << "Год рождения" << " | " << "Пол" << " | " << "Образование" << " | " << "Год поступления на работу" << endl;
    cout <<"--------------------------------------------------------------------------------------------"<<endl;
    while(current != nullptr)
    {
        if(current->data.year < maxYear->data.year && current->data.man == true)
            *maxYear = *current;
        current = current->pNext;
    }
    cout << " | " << *maxYear;
}

void List::youngSotrudnik()//Молодые специалисты до 28 с высшим образованием
{
    int i = 1;
    Node * current = head;
    cout <<"N | " << "Фамилия" << " | " << "Год рождения" << " | " << "Пол" << " | " << "Образование" << " | " << "Год поступления на работу" << endl;
    cout <<"--------------------------------------------------------------------------------------------"<<endl;
    while(current != nullptr)
    {
        if(current->data.year >= 1993 && current->data.education == true)
                cout << i << " | " << *current;
        current = current->pNext;
        i++;
    }
}

void enterSotrudnik(List &work, int Size)//Функция ввода данных сотрудника
{
    int index;
    string sn;
    int y;
    int e;
    bool ed;
    bool m;
    int wy;
    cout << "Введите данные сотрудника: " << endl;
    cout << "Фамилия" << endl;
    cin >> sn;
    cout << "Год рождения" << endl;
    cin >> y;
    cout << "Пол сотрудника: 1 - мужской, 0 - женский" << endl;
    cin >> m;
    cout << "Образование: 1 - высшее, 0 - среднее" << endl;
    cin >> e;
    cout << "Год поступления на работу" << endl;
    cin >> wy;
    Sotrudnik sotrudnik = {sn, y, m, e, wy};
    if(Size == 0)
    {
        work.addToBeggin(sotrudnik);
        cout << "Сотрудник добавлен в начало" << endl;
    }
    else
    {
        int c;
        system("cls");
        cout << "1) Добавить в начало" << endl;
        cout << "2) Добавить в конкретное место" << endl;
        cout << "3) Добавить в конец" << endl;
        cin >> c;
        switch(c)
        {
            case(1):
                system("cls");
                work.addToBeggin(sotrudnik);
                break;
            case(2):
                system("cls");
                cout << "Номер сотрудника: " << endl;
                cin >> index;
                if(index > Size+1)
                    cout << "Столько сотрудников нет! Нужно номер поменьше" << endl;
                else
                    work.Insert(sotrudnik, index-1);
                break;
            case(3):
                system("cls");
                work.addToEnd(sotrudnik);
                break;
        }
    }
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
    List work;
    string filename;
    cout << "Введите имя файла:" << endl;
    cin >> filename;
    work.loadBinary(filename);
    int c, i;
    bool exit = true;
    do
    {
        system("cls");
        cout << "Меню:" << endl;
        cout << "1) Просмотр сотрудников" << endl;
        cout << "2) Добавить сотрудника" << endl;
        cout << "3) Удалить сотрудника" << endl;
        cout << "4) Удалить всех сотрудников" << endl;
        cout << "5) Найти самого старшего сотрудника среди мужчин" << endl;
        cout << "6) Вывести список молодых специалистов (до 28 лет) с высшим образованием" << endl;
        cout << "0) Выход" << endl;
        cin >> c;
        switch(c)
        {
            system("cls");
            case(1):
                system("cls");
                work.Print();
                system("pause");
                break;
            case(2):
                system("cls");
                enterSotrudnik(work, work.getSize());
                break;
            case(3):
                system("cls");
                if(work.getSize() == 1)
                {
                    work.deleteBeggin();
                    cout << "Удален послений сотрудник. Больше не осталось :(" << endl << endl;
                }
                else
                {
                    cout << "Номер сотрудника, которого нужно удалить:" << endl;
                    cin >> i;
                    if(i > work.getSize())
                        cout << "Столько сотрудник нет!" << endl;
                    else
                        work.deleteElement(i);
                }
                system("pause");
                break;
            case(4):
                system("cls");
                bool a;
                cout << "Удалить всех сотрудников?" << endl;
                cout << endl;
                cout << "Подтвердите действие: 1 - Удалить всех, 0 - Отменить" << endl << endl;
                cin >> a;
                if(a == true)
                    work.clearElement();
                system("pause");
                break;
            case(5):
                system("cls");
                work.oldSotrudnik();
                system("pause");
                break;
            case(6):
                system("cls");
                work.youngSotrudnik();
                system("pause");
                break;
            case(0):
                exit = false;
                break;
            default:
                system("cls");
                cout << "Такого пункта в меню нет!" << endl;
                system("pause");

        }
        work.saveBinary(filename);
    }
    while(exit);
    return 0;
}
