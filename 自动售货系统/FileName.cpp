#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

struct product {
    string name;
    int price;
    int count;
    product(string _name, int _price) {
        name = _name;
        price = _price;
        count = 0;
    }
};

struct money {
    int count;
    int num;
    money(int _num) {
        num = _num;
        count = 0;
    }
};

class Sailer {
public:
    Sailer();
    ~Sailer() {}
    void excute_r(string param);
    void excute_p();
    void excute_b();
    void excute_c();
    void excute_q(string param);
private:
    vector<product> Products;
    vector<money> Moneys;
};

Sailer::Sailer() {
    Products.push_back(product("A1", 2));
    Products.push_back(product("A2", 3));
    Products.push_back(product("A3", 4));
    Products.push_back(product("A4", 5));
    Products.push_back(product("A5", 8));
    Products.push_back(product("A6", 6));
    Moneys.push_back(money(1));
    Moneys.push_back(money(2));
    Moneys.push_back(money(3));
    Moneys.push_back(money(10));
}

void Sailer::excute_r(string param) {
    istringstream ss(param);
    string p, m;
    getline(ss, p, ' ');
    getline(ss, m, ' ');

    istringstream sp(p), sm(m);
    vector<int> vp, vm;
    string temp;
    while (getline(sp, temp, '-')) {
        vp.push_back(stoi(temp));
    }
    while (getline(sm, temp, '-')) {
        vm.push_back(stoi(temp));
    }

    for (int i = 0; i < 6; i++) {
        Products[i].count = vp[i];
    }
    for (int i = 0; i < 4; i++) {
        Moneys[i].count = vm[i];
    }
    cout << "S001:Initialization is successful" << endl;
}

void Sailer::excute_p() {

}

void Sailer::excute_b() {

}

void Sailer::excute_c() {

}

void Sailer::excute_q(string param) {
    if (param.size() > 1 || (param[0] == '0' || param[0] == '1')) {
        cout << "E010:Parameter error" << endl;
    }
}

void excute(string command, Sailer& s) {
    switch (command[0]) {
    case 'r':
        s.excute_r(command.substr(command.find_first_of(' ') + 1));
        break;
    case 'p':
        s.excute_p();
        break;
    case 'b':
        s.excute_b();
        break;
    case 'c':
        s.excute_c();
        break;
    case 'q':
        s.excute_q(command.substr(command.find_first_of(' ') + 1));
        break;
    }
}

void getcommands(vector<string>& commands, string command) {
    istringstream ss(command);
    string temp;
    while (getline(ss, temp, ';')) {
        commands.push_back(temp);
    }
}

int main() {
    string c = "r 22-18-21-21-7-20 3-23-10-6;c;q0;p 1;b A6;c;b A5;b A1;c;q1;p 5;";
    istringstream ss(c);
    string input;
    while (getline(ss, input)) { // 注意 while 处理多个 case
        Sailer s = Sailer();
        vector<string> commands;
        getcommands(commands, input);
        for (int i = 0; i < commands.size(); i++) {
            excute(commands[i], s);
        }
    }
}
// 64 位输出请用 printf("%lld")