#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <map>
#include <functional>
using namespace std;

bool ContainNum(char* str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        if (isdigit(str[i]))
            return true;
    }
    return false;
}

bool CheckName(char* str)
{
    return !ContainNum(str) && isupper(str[0]);
}

class Movie
{
public:
    char name_movie[50];
    virtual void print() const = 0;
    virtual bool SetMov(char* comm) = 0;

    Movie() : name_movie("") {}

    virtual ~Movie() {}
};

class Game : public Movie
{
public:
    char name_director[50];
    virtual void print() const override
    {
        cout << "game film: " << name_movie << "\n" << "Name director: " << name_director << endl;
    }

    bool SetMov(char* comm) override
    {
        char check[50];
        if (sscanf(comm, "%*s %*s%150[^\n\r]", check) == 1)
            return false;
        if (sscanf(comm, "%s %s", name_director, name_movie) == 2 && CheckName(name_director) && CheckName(name_movie))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    Game() : Movie(), name_director("") {}
};

class Cartoon : public Movie
{
public:
    char tipe[50];
    virtual void print() const override
    {
        cout << "Movie: " << name_movie << "\n" << "Tipe: " << tipe << endl;
    }

    bool SetMov(char* comm) override
    {
        char check[50];
        if (sscanf(comm, "%*s %*s%150[^\n\r]", check) == 1)
            return false;
        if (sscanf(comm, "%s %s", tipe, name_movie) == 2 && CheckName(tipe) && CheckName(name_movie))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    Cartoon() : Movie(), tipe("") {}
};

class Series : public Movie
{
public:
    char name_of_director[50];
    unsigned int count;
    virtual void print() const override
    {
        cout << "Movie: " << name_movie << "\n" << "Name director: " << name_of_director << "\n" << "Count of episodes: " << count << endl;
    }

    bool SetMov(char* comm) override
    {
        char check[50];
        if (sscanf(comm, "%*s %*s %*s%150[^\n\r]", check) == 1)
            return false;
        if (sscanf(comm, " %s %d %s", name_of_director, &count, name_movie) == 3 && CheckName(name_of_director) && CheckName(name_movie))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    Series() : Movie(), name_of_director(""), count(NULL) {}
};

struct Node 
{
    Movie* movie;
    Node* next;
    Node* prev;

    Node(Movie* new_movie) : movie(new_movie), next(nullptr), prev(nullptr) {}
};

struct List 
{
    Node* first;
    Node* last;

    List() : first(nullptr), last(nullptr) {}
    bool is_empty() { return first == nullptr; }

    void push_back(Movie* new_movie) 
    {
        Node* newNode = new Node(new_movie);
        if (is_empty()) 
        {
            first = newNode;
            last = newNode;
            return;
        }
        last->next = newNode;
        newNode->prev = last;
        last = newNode;
    }

    void print() 
    {
        if (is_empty())
            return;

        Node* searNode = first;
        while (searNode) 
        {
            searNode->movie->print();
            searNode = searNode->next;
        }
    }

    void remove_first() 
    {
        if (is_empty())
            return;
        if (first->next == nullptr) 
        {
            delete first;
            first = last = nullptr;
            return;
        }
        Node* searNode = first;
        first = searNode->next;
        first->prev = nullptr;
        delete searNode;
    }

    void remove_last() 
    {
        if (is_empty())
            return;
        if (first == last) 
        {
            remove_first();
            return;
        }
        Node* searNode = last->prev;
        searNode->next = nullptr;
        delete last;
        last = searNode;
    }

    void remove(Node* sernode) 
    {
        if (is_empty())
            return;
        if (sernode == nullptr)
            return;

        if (first == sernode) 
        {
            remove_first();
            return;
        }
        else if (last == sernode) 
        {
            remove_last();
            return;
        }

        Node* searNode = first->next;
        while (searNode && searNode != sernode) 
        {
            searNode = searNode->next;
        }
        if (!searNode) 
        {
            delete searNode;
            return;
        }
        searNode->prev->next = searNode->next;
        searNode->next->prev = searNode->prev;
        delete searNode;
    }

    void freeList() 
    {
        if (is_empty())
            return;
        while (!is_empty())
            this->remove_first();
    }

    bool Search(Node* search,char* comm)
    {
        if (is_empty())
            return false;

        char par[50];
        char oper[50];
        char inp[50];
        char check[50];

        if (sscanf(comm, "%*s %*s %*s%150[^\n\r]", check) == 1)
            return false;
        if (sscanf(comm, "%s %s %s", par, oper, inp) != 3)
            return false;

        map<string, function<bool(Movie*, string, string)>> pars = {
            {"MovName", [](Movie* movie, string oper, string inp) {
                if (oper == "=") {
                    return movie->name_movie == inp;
                }
                return false;
            }}
        };

    if (pars[par](search->movie, oper, inp)) 
    {
        return true;
    }
    return false;
    }

    void remove_all_same(char* comm)
    {
        Node* serNode = this->first;
        while (serNode)
        {
            if (this->Search(serNode, comm))
            {
                if (serNode == this->first)
                {
                    remove(serNode);
                    serNode = this->first;
                }
                else if (serNode == this->first)
                {
                    remove(serNode);
                    serNode = nullptr;
                }
                else
                {
                    serNode = serNode->next;
                    remove(serNode->prev);
                }
            }
            else
            {
                serNode = serNode->next;
            }
        }
    }
};


int main()
{
    List list;
    FILE* file = fopen("command.txt", "r");

    char comm[150];
    char incomm[150];

    while (fgets(incomm, 149, file))
    {
        sscanf(incomm, "%s", comm);
        if (string(comm) == "ADD")
        {
            sscanf(incomm, "%*s %s", comm);
            if (string(comm) == "cartoon")
            {
                Movie* cartoon = new Cartoon();
                sscanf(incomm, "%*s %*s %150[^\n\r]", comm);
                if (cartoon->SetMov(comm))
                {
                    list.push_back(cartoon);
                }
                else
                {
                    delete cartoon;
                    cout << "That's wrong command!\n";
                }
            }
            else if (string(comm) == "game")
            {
                Movie* game = new Game();
                sscanf(incomm, "%*s %*s %150[^\n\r]", comm);
                if (game->SetMov(comm))
                {
                    list.push_back(game);
                }
                else
                {
                    delete game;
                    cout << "That's wrong command!\n";
                }
            }
            else if (string(comm) == "series")
            {
                Movie* series = new Series();
                sscanf(incomm, "%*s %*s %150[^\n\r]", comm);
                if (series->SetMov(comm))
                {
                    list.push_back(series);
                }
                else
                {
                    delete series;
                    cout << "That's wrong command!\n";
                }
            }
        }
        else if (string(comm) == "REM")
        {
            sscanf(incomm, "%*s %150[^\n\r]", comm);
            list.remove_all_same(comm);
        }
        else if (string(comm) == "PRINT")
        {
            list.print();
        }
        else
        {
            wcerr << "Wrong command\n";
        }
    }
}