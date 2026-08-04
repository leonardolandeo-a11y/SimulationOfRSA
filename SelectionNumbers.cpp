#include "SelectionNumbers.h"
#include "RSA_Functions.h"

// Selection of big numbers 
// I create number in the range of 10**10 - 10**12
long long SelectionofNumbers(){
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<long long> dist(10000000000LL, 1000000000000LL);
    long long number = dist(gen);
    while (number % 2 == 0 || number % 5 == 0 || number % 3 == 0 || number % 7 == 0){
        number = dist(gen);
    }
    
    return number;
}

// Selection of big number in a specific range 
long long SelectionofNumbersSpecificRange(long long a, long long b){
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<long long> dist(a, b);
    long long number = dist(gen);
    while (number % 2 == 0 || number % 5 == 0 || number % 3 == 0 || number % 7 == 0){
        number = dist(gen);
    }
    
    return number;
}


/*                 Miller-Rabin test                */

// Selection of bases a in a**(2**s *d) = 1 mod n
// it works perfectly for the Miller-Rabin test 
long long SelectionofBasesMillerRabinTest(long long n){
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<long long> dist(2LL, n-1LL);
    return dist(gen);
}

// Constants s and d in a**(2**s *d) for any number
MillerRabinTestConstants ConstantsforMillerRabinTest(long long number){
    long long s = 0;
    long long CompositeforMillerRabinTest = number - 1;
    while ((CompositeforMillerRabinTest) % 2 == 0){
        CompositeforMillerRabinTest >>= 1;
        s++;
    }
    long long d = CompositeforMillerRabinTest;
    return {s, d};
}

// Verification using the miller rabin test
// The objective: (x**d)**2...  ( elevated by 2 s times) using the next lemma:
// if m is prime and x**2 is congruent to 1 mod m -> x**2 mod m is congruent to 1 or m-1
int VerificationofMillerRabinTest(long long modulos, MillerRabinTestConstants ConstantsMillerRabinTest, long long base){

    long long s = ConstantsMillerRabinTest.s;
    long long d = ConstantsMillerRabinTest.d;

    long long x = ModularExponation(modulos, base, d);
    if (x == 1 || x == (modulos - 1)){
            return 1;
    }
    for (long long i = 0; i < s - 1; i++){
        x = (__int128)((x * x) % modulos);

        if (x == (modulos - 1)){
            return 1;
        }
        if (x == 1){
            return 0;
        }
    }
    return 0;
}

/* ################################################ */