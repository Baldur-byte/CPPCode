#include <iostream>
#include <string>
using namespace std;

int main() {
    int n = 81;
    //while (cin >> n) { // ע�� while ������ case
        int result = 0;
        for (int i = 0; i <= n; i++) {
            if (i % 10 == 0 || i % 10 == 1 || i % 10 == 5 || i % 10 == 6) {
                bool isFind = true;
                string str1 = to_string(i);
                string str2 = to_string(i * i);
                for (int j = 0; j < str1.size(); j++) {
                    if (str1[str1.size() - 1 - i] != str2[str2.size() - 1 - i]) {
                        isFind = false;
                        break;
                    }
                }
                if (isFind) {
                    cout << i << endl;
                    result++;
                }
            }
        }
        cout << result << endl;
    //}
}
// 64 λ������� printf("%lld")