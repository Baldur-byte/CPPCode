#include <iostream>
using namespace std;

string dic1[] = { "零", "壹", "贰", "叁", "肆", "伍", "陆", "柒", "捌", "玖" };
string dic2[] = { "", "拾", "佰", "仟" };
string dic3[] = { "元", "万", "亿", "万亿"};
string dic4[] = { "整", "角", "分" };

int main() {
    double input = 0.29;
    int num = (int)input;
    int end = input * 100 - num * 100;
    string head = "";
    string tail = "";
    if (end > 0) {
        if (end / 10 > 0) {
            tail += dic1[end / 10] + dic4[1];
        }
        if (end % 10 > 0) {
            tail += dic1[end / 10] + dic4[2];
        }
    }
    else {
        tail = "整";
    }
    string result = "";
    int index3 = 0;
    while (num > 0) {
        string temp = "";
        int t = num % 10000;
        num /= 10000;
        int a = t % 10;
        int b = t % 100 - a;
        int c = t % 1000 - a - b;
        int d = t - a - b - c;
        if (a > 0) {
            temp = dic1[a] + dic2[0] + temp;
        }
        if (b > 0) {
            if (b / 10 == 1) {
                temp = dic2[1] + temp;
            }
            else {
                temp = dic1[b / 10] + dic2[1] + temp;
            }
        }
        else {
            if (a != 0 && (c > 0 || d > 0 || num > 0)) {
                temp = dic1[b / 10] + temp;
            }
        }
        if (c > 0) {
            temp = dic1[c / 100] + dic2[2] + temp;
        }
        else {
            if (b != 0 && (d > 0 || num > 0)) {
                temp = dic1[c / 100] + temp;
            }
        }
        if (d > 0) {
            temp = dic1[d / 1000] + dic2[3] + temp;
        }
        else {
            if (c != 0 && num > 0) {
                temp = dic1[d / 1000] + temp;
            }
        }
        temp += dic3[index3];
        result = temp + result;
        index3++;
    }
    cout << head << result << tail << endl;
}
// 64 位输出请用 printf("%lld")