#ifndef RSA_FUNCTIONS_H
#define RSA_FUNCTIONS_H

#include <bits/stdc++.h>

using namespace std;


// Constants for the bezeout's theorem 
struct BezeoutTheoremConstants{
    __int128 gcd;
    __int128 s;
    __int128 t;
};


string ConvertTobase64(__int128 number);
__int128 ConvertFromBase64(const string& str);
int LettersToNumbers(string word);
string NumbersToLetters(long long number);

long long ModularExponation(long long modulos,long long base, long long exponent );

vector<__int128 > ModularExponationForBigNumbers(__int128 base, __int128 exponent ,long long p, long long q);
BezeoutTheoremConstants BezeoutTheoremForD(__int128 NumberofEuler, __int128 e);

string ShowBigNumbers(__int128 number);
__int128 StringToBigNumber(const string& str);
__int128 ChineseRemainderTheorem(vector<__int128> numbers,long long p, long long q,__int128 n);

#endif