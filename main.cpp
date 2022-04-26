#include <iostream>
#include <locale>
#include <string>
#include <fstream>
#include <windows.h>
#include <string>

using namespace std;

/*1. ���� ������: �������, ��� ��������, ���, ����������� (�������,
������), ��� ����������� �� ������. ����� ������ �������� ���������� �����
������. ������� ������ ������� ������������ (�� 28 ���) � ������
������������*/

struct Sotrudnik
{
    string surname;
    int year;//��� ��������
    bool man;
    bool education;
    int work_year;//��� ���������� �� ������
};

class Node //����� ��������
{
private:
    Sotrudnik data;//������ ����������
    Node *pNext;//��������� �� ��������� �������

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
    stream << A.data.surname << "\t| " << A.data.year << "\t  | " << (A.data.man?"�������":"�������");
	stream << "\t| " << (A.data.education?"������":"�������") << "\t| " <<  A.data.work_year << endl;
	return stream;
}

class List //����� ��� �������
{
private:
    Node *head;//��������� �� ������ �������
    int Size;//���������� ���������
public:
    List()
    {
        Size = 0;
        head = NULL;
    };
    ~List(){};
    int getSize(){return Size;}; // ���������� ���������

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

void List::Print()//�����
{
    Node *p;
    p = head;
    int i = 1;
    cout <<"N | " << "�������" << " | " << "��� ��������" << " | " << "���" << " | " << "�����������" << " | " << "��� ����������� �� ������" << endl;
	cout <<"--------------------------------------------------------------------------------------------"<<endl;
	while(p != nullptr)
    {
        cout << i++ << " | " << *p;
        p = p->pNext;
    }
    cout << endl;
}

void List::Insert(Sotrudnik data, int index)//���������� ���������� � ����� �����
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

void List::deleteBeggin()//�������� ������� ��������
{
    Node *tmp = head;
    head = head->pNext;
    delete tmp;
    Size--;
}

void List::deleteElement(int index)//�������� ����������� �������� � ������
{
    Node *previous = this->head;
    Node *del;
    for(int i = 0;i < index-2;i++)
        previous = previous->pNext;
    del = previous->pNext;
    previous->pNext = del->pNext;
    delete del;
    Size--;
    cout << "��������� N " << index << " ������" << endl;
}

void List::clearElement()//�������� ���� ���������
{
    {
        while(Size)
            deleteBeggin();
        cout << "��� �������� �������" << endl;
    }
}

void List::addToBeggin(Sotrudnik data)//���������� �������� � ������ ������
{
    head = new Node(data, head);
    Size++;
    cout << "��������� �������� � ������" << endl;
}

void List::addToEnd(Sotrudnik data)//���������� �������� � �����
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
       cout << "��������� �������� � �����" << endl;
   }
}

/*Sotrudnik List::operator[](const int index)//������������ [] ��� ������ ����������� ��������
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

void List::oldSotrudnik()//����� ������� ��������� ����� ������
{
    Node *current = head;
    Node *maxYear = head;
    cout << "����� ������� ��������� ����� ������" << endl << endl;
    cout <<"N | " << "�������" << " | " << "��� ��������" << " | " << "���" << " | " << "�����������" << " | " << "��� ����������� �� ������" << endl;
    cout <<"--------------------------------------------------------------------------------------------"<<endl;
    while(current != nullptr)
    {
        if(current->data.year < maxYear->data.year && current->data.man == true)
            *maxYear = *current;
        current = current->pNext;
    }
    cout << " | " << *maxYear;
}

void List::youngSotrudnik()//������� ����������� �� 28 � ������ ������������
{
    int i = 1;
    Node * current = head;
    cout <<"N | " << "�������" << " | " << "��� ��������" << " | " << "���" << " | " << "�����������" << " | " << "��� ����������� �� ������" << endl;
    cout <<"--------------------------------------------------------------------------------------------"<<endl;
    while(current != nullptr)
    {
        if(current->data.year >= 1993 && current->data.education == true)
                cout << i << " | " << *current;
        current = current->pNext;
        i++;
    }
}

void enterSotrudnik(List &work, int Size)//������� ����� ������ ����������
{
    int index;
    string sn;
    int y;
    int e;
    bool ed;
    bool m;
    int wy;
    cout << "������� ������ ����������: " << endl;
    cout << "�������" << endl;
    cin >> sn;
    cout << "��� ��������" << endl;
    cin >> y;
    cout << "��� ����������: 1 - �������, 0 - �������" << endl;
    cin >> m;
    cout << "�����������: 1 - ������, 0 - �������" << endl;
    cin >> e;
    cout << "��� ����������� �� ������" << endl;
    cin >> wy;
    Sotrudnik sotrudnik = {sn, y, m, e, wy};
    if(Size == 0)
    {
        work.addToBeggin(sotrudnik);
        cout << "��������� �������� � ������" << endl;
    }
    else
    {
        int c;
        system("cls");
        cout << "1) �������� � ������" << endl;
        cout << "2) �������� � ���������� �����" << endl;
        cout << "3) �������� � �����" << endl;
        cin >> c;
        switch(c)
        {
            case(1):
                system("cls");
                work.addToBeggin(sotrudnik);
                break;
            case(2):
                system("cls");
                cout << "����� ����������: " << endl;
                cin >> index;
                if(index > Size+1)
                    cout << "������� ����������� ���! ����� ����� ��������" << endl;
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
    cout << "������� ��� �����:" << endl;
    cin >> filename;
    work.loadBinary(filename);
    int c, i;
    bool exit = true;
    do
    {
        system("cls");
        cout << "����:" << endl;
        cout << "1) �������� �����������" << endl;
        cout << "2) �������� ����������" << endl;
        cout << "3) ������� ����������" << endl;
        cout << "4) ������� ���� �����������" << endl;
        cout << "5) ����� ������ �������� ���������� ����� ������" << endl;
        cout << "6) ������� ������ ������� ������������ (�� 28 ���) � ������ ������������" << endl;
        cout << "0) �����" << endl;
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
                    cout << "������ �������� ���������. ������ �� �������� :(" << endl << endl;
                }
                else
                {
                    cout << "����� ����������, �������� ����� �������:" << endl;
                    cin >> i;
                    if(i > work.getSize())
                        cout << "������� ��������� ���!" << endl;
                    else
                        work.deleteElement(i);
                }
                system("pause");
                break;
            case(4):
                system("cls");
                bool a;
                cout << "������� ���� �����������?" << endl;
                cout << endl;
                cout << "����������� ��������: 1 - ������� ����, 0 - ��������" << endl << endl;
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
                cout << "������ ������ � ���� ���!" << endl;
                system("pause");

        }
        work.saveBinary(filename);
    }
    while(exit);
    return 0;
}
