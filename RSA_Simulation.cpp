#include "RSA_Functions.h"
#include "SelectionNumbers.h"
#include <iostream>
#include <random>
#include <vector>
#include <bitset>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <string>
#include <cctype>
#include <sstream>

using namespace std;


int main(int argc, char* argv[]) {
    if (argc < 2) return 1;

    string mode = argv[1];

    if (mode == "--generate-keys") {
        long long P;
        long long Q;

        /*               Selection of prime numbers p, q ,and e               */
        // Selection of P
        bool IsPrimeP = false;
        while (IsPrimeP == false) {
            
            P = SelectionofNumbers();
            
            MillerRabinTestConstants Constantsfor_P = ConstantsforMillerRabinTest(P);
            vector<int> Probabilityof_P;
            for (int i = 0; i < 20; i++) {
                long long basefor_P = SelectionofBasesMillerRabinTest(P);
                
                int Resultfor_P = VerificationofMillerRabinTest(P, Constantsfor_P, basefor_P);
                
                Probabilityof_P.push_back(Resultfor_P);
            }
            int SumProbabilityof_P = accumulate(Probabilityof_P.begin(), Probabilityof_P.end(), 0);
            if (SumProbabilityof_P == 20) {
                IsPrimeP = true;
            }
            
        }
        // Selection of Q
        bool IsPrimeQ = false;
        while (IsPrimeQ == false) {

            Q = SelectionofNumbers();

            MillerRabinTestConstants Constantsfor_Q = ConstantsforMillerRabinTest(Q);
            vector<int> Probabilityof_Q;
            for (int i = 0; i < 20; i++) {
                long long basefor_Q = SelectionofBasesMillerRabinTest(Q);
                int Resultfor_Q = VerificationofMillerRabinTest(Q, Constantsfor_Q, basefor_Q);
                Probabilityof_Q.push_back(Resultfor_Q);
            }
            int SumProbabilityof_Q = accumulate(Probabilityof_Q.begin(), Probabilityof_Q.end(), 0);

            if (SumProbabilityof_Q == 20) {
                IsPrimeQ = true;
            }

        }
        // Selection of the exponent 
        // gcd(NumberofEuler(n), e) = 1
        // Creation of number of euler:
        __int128 NumberOfEuler = (__int128)(P - 1) * (Q - 1);
        long long e;
        bool IsPrimeE = false;
        
        while (IsPrimeE == false) {

            e = SelectionofNumbersSpecificRange(2, 10000000000000000LL);

            MillerRabinTestConstants Constantsfor_e = ConstantsforMillerRabinTest(e);
            vector<int> Probabilityof_e;
            for (int i = 0; i < 20; i++) {
                long long basefor_e = SelectionofBasesMillerRabinTest(e);
                int Resultfor_e = VerificationofMillerRabinTest(e, Constantsfor_e, basefor_e);
                Probabilityof_e.push_back(Resultfor_e);
            }
            int SumProbabilityof_e = accumulate(Probabilityof_e.begin(), Probabilityof_e.end(), 0);
            if (SumProbabilityof_e == 20 && NumberOfEuler % e != 0) {
                IsPrimeE = true;
            }
        }
        
        /* ################################################################ */

        /*                         Operation for RSA                         */

        // Define the number n created by the multiplication of both primes.
        __int128 n = (__int128)P * Q;
        
        // Apply the Bezeout's theorem to find the inverse of e
        // if gcd(NumberofEuler(n), e) = 1 -> Exists a d such that e*d = 1 mod NumberofEuler(n)
        BezeoutTheoremConstants ConstanstForInverseofE = BezeoutTheoremForD(NumberOfEuler, e);
        __int128 Previous_D = ConstanstForInverseofE.t;
        __int128 D = (__int128)(((Previous_D % NumberOfEuler) + NumberOfEuler) % NumberOfEuler); // Avoid negative numbers

        /* ###############################################################*/

        /*                      Show data                      */
        cout << "{"
             << "\"p\":\"" << P << "\","
             << "\"q\":\"" << Q << "\","
             << "\"n\":\"" << ShowBigNumbers(n) << "\","
             << "\"phi\":\"" << ShowBigNumbers(NumberOfEuler) << "\","
             << "\"e\":\"" << e << "\","
             << "\"d\":\"" << ShowBigNumbers(D) << "\""
             << "}" << endl;
        /* ##########################################################*/
    }
    else if (mode == "--encrypt" && argc >= 6) {
        string message = argv[2];
        long long P = stoll(argv[3]);
        long long Q = stoll(argv[4]);
        long long e = stoll(argv[5]);
        __int128 n = (__int128)P * Q;

        /*                      Encryption                      */

        // Dividing the message
        // Leonardo -> Le - on - na -rd - o
        vector<string> SepareMessage;

        for (size_t i = 0; i < message.size(); i += 2) {

            string aux;
            aux += message[i];

            if (i + 1 < message.size())
                aux += message[i + 1];
            else
                aux += ' ';  

            SepareMessage.push_back(aux);
        }

        // Encrypting each separe message
        string encryptedMessage;
        
        for (size_t i = 0; i < SepareMessage.size(); i++) {
            int M = LettersToNumbers(SepareMessage[i]);
            vector<__int128> Vector_C = ModularExponationForBigNumbers(M, e, P, Q);
            __int128 C = ChineseRemainderTheorem(Vector_C, P, Q, n);
            string Encrypted_C = ConvertTobase64(C);
            encryptedMessage += (Encrypted_C);
            encryptedMessage += "%";
        }

        /* ##########################################################*/

        cout << "{\"encrypted\":\"" << encryptedMessage << "\"}" << endl;
    }
    else if (mode == "--decrypt" && argc >= 6) {
        string encryptedMessage = argv[2];
        long long P = stoll(argv[3]);
        long long Q = stoll(argv[4]);
        __int128 D = StringToBigNumber(argv[5]);
        __int128 n = (__int128)P * Q;

        /*                      Decryption                      */
        stringstream ss(encryptedMessage);
        string segment;
        string DecryptedMessage;

        while (getline(ss, segment, '%')) {
            if (segment.empty()) continue;
            __int128 C = ConvertFromBase64(segment);
            vector<__int128> DecryptedMessageExponation = ModularExponationForBigNumbers(C, D, P, Q);
            __int128 MessageDecrypted = ChineseRemainderTheorem(DecryptedMessageExponation, P, Q, n);
            string ShowMessageDecryted = NumbersToLetters((long long)MessageDecrypted);

            DecryptedMessage += ShowMessageDecryted;
        }

        /* ##########################################################*/

        cout << "{\"decrypted\":\"" << DecryptedMessage << "\"}" << endl;
    }

    return 0;
}