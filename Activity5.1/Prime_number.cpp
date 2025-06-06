#include<cmath>
#include <iostream>

using namespace std;

bool is_prime_numbers(int x){
    for (int j=2; j<=sqrt(x); j++){
        if (x%j==0){
            return false;
        }
    }
    return true;
}

long long sum(int n){
    long long sum_total=0;
    if (n<2){
        return 0;
    }
    else if(n==2){
        return 2;
    }
    else{
        for (int x=2; x<=n; x++){
            if(is_prime_numbers(x)==true){
                sum_total+=x;
            }
        }
    }
    return sum_total;   
}

int main(){
    long long n ;
    cout<<"Sum of the prime numbers: ";
    cin>>n;
    cout<<sum(n)<<endl;
}