#include "RSA_Functions.h"
#include <bits/stdc++.h>
using namespace std;

string ConvertTobase64(__int128 number){
    if (number == 0) return "A";
    string NumberConverted;
    int digits64;
    while (number > 0){
        digits64 = number % 64;
        char base64char;
        if (digits64 >= 0 && digits64 <= 25) {
            base64char = 'A' + digits64;          
        } else if (digits64 >= 26 && digits64 <= 51) {
            base64char = 'a' + (digits64 - 26);  
        } else if (digits64 >= 52 && digits64 <= 61) {
            base64char = '0' + (digits64 - 52);   
        } else if (digits64 == 62) {
            base64char = '+';                    
        } else if (digits64 == 63) {
            base64char = '/';                     
        }
        NumberConverted += base64char;
        number/=64;
    }
    reverse(NumberConverted.begin(),NumberConverted.end());
    return NumberConverted;
}

// Convert Base64 string back into __int128 number for decryption
__int128 ConvertFromBase64(const string& str) {
    __int128 number = 0;
    for (char c : str) {
        int digit = 0;
        if (c >= 'A' && c <= 'Z') digit = c - 'A';
        else if (c >= 'a' && c <= 'z') digit = c - 'a' + 26;
        else if (c >= '0' && c <= '9') digit = c - '0' + 52;
        else if (c == '+') digit = 62;
        else if (c == '/') digit = 63;
        number = number * 64 + digit;
    }
    return number;
}


int LettersToNumbers(string word){

    int number = 0;
    for (size_t i = 0; i < word.size(); i++){

        char lowerletter = tolower(word[i]);
        int number_letter;

        if (lowerletter == ' ')
            number_letter = 36;
        else
            number_letter = lowerletter - 'a' + 10;

        if (i == 0)
            number += number_letter * 100;
        else
            number += number_letter;
    }
    return number;
}

string NumbersToLetters(long long number){

    string number_string = to_string(number);
    if (number_string.size() % 2 != 0) number_string = "0" + number_string;
    string finalword;

    for(size_t i = 0; i < number_string.size(); i += 2){

        string letters;
        letters += number_string[i];
        letters += number_string[i+1];

        int numberletter = stoi(letters);

        if (numberletter == 36)
            finalword += ' ';
        else
            finalword += ('a' + (numberletter - 10));
    }

    return finalword;
}

// Modular Exponentiation for numbers less than 64 bits 
long long ModularExponation(long long modulos,long long base, long long exponent ){
    long long x = 1LL;
    base %= modulos;
    while (exponent>0){
        if (exponent & 1){
            x = (long long)(((__int128)x * base) % modulos);
        }
        base = (long long)(((__int128)base * base) % modulos);
        exponent>>=1;
    }
    return x;
}


// Modular Exponentiation for big numbers bigger than 64 bits 
// I apply the concept of form vectors of moduli to make the multiplication of numbers less than 128bits 
vector<__int128 > ModularExponationForBigNumbers(__int128 base, __int128 exponent ,long long p, long long q){
    //Intialitation of the vectors
    // x = (1 mod p , 1 mod q)
    vector<__int128 > x = {1,1};

    // base is the number n ( n= pq ), so its to much big, so i reduced it to this form
    vector<__int128> basemod = {base %p, base %q};
    
    // Now the elements in both vector and then the multiplications don't exceed the range of 128 bits
    while (exponent > 0){
        if(exponent & 1){
            // For the first element 
            __int128 Aux1 = basemod[0]* x[0];
            x[0] = (Aux1% p);

            // For the second element
            __int128 Aux2 = basemod[1]* x[1];
            x[1] = (Aux2% q);
            
            
        }
        // For the first element 
        __int128 Aux1 = basemod[0]* basemod[0];
        basemod[0] = (Aux1% p);

        // For the second element
        __int128 Aux2 = basemod[1]* basemod[1];
        basemod[1] = (Aux2% q);
        
        exponent>>=1;
    }
    // Solution to avoid negative numbers -> x = ((x mod n) +n) mod n

    // For the first element
    x[0] = (((x[0]% p)+ p)%p);

    // For the second element
    x[1] = (((x[1]% q) +q)% q);

    return x;
}



// Bezeout's Theorem
// I use the bezeout's theorem to calculate the inverse of some numbers
// Formula:
// gcd(p,q) = p(s) + q(t)
// gcd(p,q) = gcd(q, p mod q), then
// gcd(q, p mod q) = q(s) + (p mod q)(t) 
// p mod q = p-q(p/q)
// gcd(q, p mod q) = q(s) + (p-q(p/q))t
// gcd(q, p mod q) = q(s) + p(t)-tq(p/q)
// gcd(q, p mod q) = p(t) + q(s-t(p/q)) , then
//  s1 = t0
//  t1 = s0-t0(p/q)
BezeoutTheoremConstants BezeoutTheoremForD(__int128 NumberofEuler, __int128 e){
    if (e == 0){
        return {NumberofEuler,1,0};
    }
    BezeoutTheoremConstants constants = BezeoutTheoremForD(e, NumberofEuler% e);
    
    __int128 s = constants.t;
    __int128 t = constants.s - (constants.t * (NumberofEuler/e));
    return {constants.gcd,s,t};
}




// Show big numbers grather than 64 bits 
string ShowBigNumbers(__int128 number){
    if (number == 0) return "0";
    string bignumber;
    while (number != 0){
        int digit = (int) (number %10);
        bignumber = char('0' + digit) + bignumber;
        number /=10;
    }
    return bignumber;
}

// Convert decimal string into __int128 number
__int128 StringToBigNumber(const string& str) {
    __int128 val = 0;
    for (char c : str) {
        if (c >= '0' && c <= '9') val = val * 10 + (c - '0');
    }
    return val;
}

// Chinese Remainder Theorem
// I use the Chinese remainder theorem to return the vectors in their original number form 

__int128 ChineseRemainderTheorem(vector<__int128> numbers,long long p, long long q,__int128 n){
    BezeoutTheoremConstants Modulos = BezeoutTheoremForD(p,q);
    // For Element 1 
    __int128 t = ((Modulos.t % p)+p)%p;
    __int128 FinalElement1 = (__int128)(t * numbers[0] * q);

    // For element 2
    __int128 s = ((Modulos.s % q)+q)%q;
    __int128 FinalElement2 = (__int128)(s * numbers[1]*p);

    // Final element 
    __int128 FinalElement = (__int128)((FinalElement1 + FinalElement2)% n);
    return FinalElement;

}