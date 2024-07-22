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
    string _name;
    string _value;

    Node(int id, int parentId, string name, string value)
    {
        _id = id;
        _parentId = parentId;
        _name = name;
        _value = value;
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
    stack<int>   parentIdStack;
    vector<Node> nodes;

    parentIdStack.push(0);


    // Анализ данных
    while (getline(inFile, line))
    {
        while (line.find('=') != string::npos)
        {
            string name = line.substr(0, line.find('=') - 1);
            id++;
            line = line.substr(line.find('=') + 1);

            string value = "";

            if (line.find('{') != string::npos)
            {
                Node node(id, parentIdStack.top(), name, value);
                nodes.push_back(node);

                parentIdStack.push(id);
                line = line.substr(line.find('{') + 2);

                continue;
            }
            else if (line.find('"') != string::npos)
            {
                line = line.substr(line.find('"') + 1);
                if (line.find('"') != string::npos)
                {
                    value = line.substr(0, line.find('"'));
                    line = line.substr(line.find('"') + 2);
                }
            }

            Node node(id, parentIdStack.top(), name, value);
            nodes.push_back(node);
        }

        if (line.find('}') != string::npos)
        {
            parentIdStack.pop();
        }

    }


    // Вывод
    string tab = "";
    int    oldParentId = 0;
    int    foundRecord_position = 0;

    for (int i = 0; i < nodes.size(); i++)
    {
        // Поиск родителей для определения количества пробелов (если родитель изменился) 
        if (oldParentId != nodes[i]._parentId)
        {
            foundRecord_position = i;
            tab = "";
            oldParentId = nodes[i]._parentId;

            while (true)
            {
                if (nodes[foundRecord_position]._parentId == 0) {
                    break;
                }

                int findRecord_id = nodes[foundRecord_position]._parentId;
                const auto p = find_if(nodes.begin(), nodes.end(),
                    [findRecord_id](const Node& a) { return a._id == findRecord_id; });

                foundRecord_position = p - begin(nodes);
                tab.append("  ");
            }
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