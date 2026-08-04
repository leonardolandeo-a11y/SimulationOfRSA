#ifndef SELECTION_NUMBERS_H
#define SELECTION_NUMBERS_H

#include <bits/stdc++.h>
#include "RSA_Functions.h"

using namespace std;

struct MillerRabinTestConstants {
    long long s;  
    long long d;
};

long long SelectionofNumbers();
long long SelectionofNumbersSpecificRange(long long a, long long b);

long long SelectionofBasesMillerRabinTest(long long n);
MillerRabinTestConstants ConstantsforMillerRabinTest(long long number);
int VerificationofMillerRabinTest(long long modulos, MillerRabinTestConstants ConstantsMillerRabinTest, long long base);

#endif