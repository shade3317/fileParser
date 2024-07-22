#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <vector>

using namespace std;



class Node
{
public:
    int    _id;
    int    _parentId;
    int _stackIndex;
    string _name;
    string _value;

    Node(int id, int parentId, string name, string value, int stackIndex)
    {
        _id = id;
        _parentId = parentId;
        _name = name;
        _value = value;
        _stackIndex = stackIndex;
    }
};



int main(int argc, char* argv[])
{
    if (argc != 3) {
        cout << "Неверный формат данных" << endl;
        return 1;
    }

    // Открытие файлов 
    ifstream inFile(argv[1]);
    ofstream outFile(argv[2]);

    string       line;
    int          id = 0;
    int          stackIndex = 0;
    stack<int>   parentIdStack;
    vector<Node> nodes;

    parentIdStack.push(0);


    // Анализ данных
    while (getline(inFile, line))
    {
        while (line.find('=') != string::npos)
        {
            string name = line.substr(0, line.find('=') - 1);
            name.erase(remove_if(name.begin(), name.end(), isspace), name.end());
            id++;

            line = line.substr(line.find('=') + 1);

            string value = "";

            if (line.find('{') != string::npos)
            {
                Node node(id, parentIdStack.top(), name, value, stackIndex);
                nodes.push_back(node);

                stackIndex++;
                parentIdStack.push(id);
                if (line.size() >= 2)
                    line = line.substr(line.find('{') + 1);

                continue;
            }
            else if (line.find('"') != string::npos)
            {
                if (line.size() >= 2)
                {
                    line = line.substr(line.find('"') + 1);
                    if (line.find('"') != string::npos)
                    {
                        value = line.substr(0, line.find('"'));
                        if (line.size() >= 2)
                            line = line.substr(line.find('"') + 1);
                    }
                }
            }

            Node node(id, parentIdStack.top(), name, value, stackIndex);
            nodes.push_back(node);
        }

        if (line.find('}') != string::npos)
        {
            stackIndex--;
            parentIdStack.pop();
        }

    }


    // Вывод
    string tab = "";
    int    oldParentId = 0;
    int    foundRecord_position = 0;

    for (int i = 0; i < nodes.size(); i++)
    {
        tab = "";

        for (int j = 0; j < nodes[i]._stackIndex; j++)
        {
            tab.append("  ");
        }

        // Вывод на экран
        cout << tab << "("
            << nodes[i]._id << ", "
            << nodes[i]._parentId << ", "
            << nodes[i]._name << ", "
            << nodes[i]._value << ")" << "\n";

        // Вывод в файл
        outFile << tab << "("
            << nodes[i]._id << ", "
            << nodes[i]._parentId << ", "
            << nodes[i]._name << ", "
            << nodes[i]._value << ")" << endl;
    }
}