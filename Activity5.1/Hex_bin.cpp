#include <string>
#include <algorithm>
#include <iostream>
#include<cmath>

using namespace std;

bool bin_hex_palindromes(string value){
    string pal_reverse = value;
    reverse(pal_reverse.begin(), pal_reverse.end());

    if (value == pal_reverse){
        return true;
    }

    return false;
}

string int_to_binary(long long i){
    if (i==0){
        return "0";
    }
    string binary;
    while (i>0){
        if (i%2 == 0){
            binary += '0';
        }
        else{
            binary += '1';
        }
        i=i/2;
    };
    return binary;
}


string int_to_hex(long long i){
    if (i==0) return "0";
    string lista;
    int cons;
    while (i>0){
        cons = i % 16;
        if (cons<10){
            lista = to_string(cons) + lista;
        }
        else{
            switch (cons){ 
                case 10: lista = 'A' + lista; break;
                case 11: lista = 'B' + lista; break;
                case 12: lista = 'C' + lista; break;
                case 13: lista = 'D' + lista; break;
                case 14: lista = 'E' + lista; break;
                case 15: lista = 'F' + lista; break;
            }
        }
        i = i / 16;
    }
    return lista;
}

int main(){
    long long n, count = 0;
    cout<<"Hexadecimal: ";
    cin>>n;
    for (long long i=0; i<=pow(2, n); i++){
        string hex= int_to_hex(i);
        string bin = int_to_binary(i);
        if (bin_hex_palindromes(hex) && bin_hex_palindromes(bin)){
            count++;
        }
    }
    cout<<count<<" hexadecimals"<<endl;
}