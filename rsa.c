#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "rsa.h"

const char table[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 !@#$%^&*()-_=+[]{}|;:'\",.<>/?";
const char separators[] = "@#$%^&*_=+[]{}|<>/?";

int isPrime(int n) {
    if (n <= 1) return 0;
    for (int i = 2; i * i <= n; i++) if (n % i == 0) return 0;
    return 1;
}

int randNumPrime(int min, int max) {
    srand(time(NULL));
    int result = rand() % (max - min + 1) + min;
    while (!isPrime(result)) result = rand() % (max - min + 1) + min;
    return result;
}

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int modInverse(int a, int m) {
    for (int i = 1; i < m; i++) if ((a * i) % m == 1) return i;
    fprintf(stderr, "Обратный элемент не существует.\n");
    exit(EXIT_FAILURE);
}

int modExp(int base, int exp, int mod) {
    long long result = 1, b = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) result = (result * b) % mod;
        b = (b * b) % mod;
        exp /= 2;
    }
    return (result + mod) % mod;
}

void rsa_generate_keys(int *e, int *n, int *d) {
    int p = randNumPrime(2, 83), q = randNumPrime(2, 89);
    *n = p * q;
    int fi = (p - 1) * (q - 1);
    for (*e = 2; *e < fi; (*e)++) if (gcd(*e, fi) == 1) break;
    *d = modInverse(*e, fi);
    if ((*e * *d) % fi != 1) {
        fprintf(stderr, "Ключи RSA некорректны.\n");
        exit(EXIT_FAILURE);
    }
}

char *rsa_encrypt(const char *data, int e, int n) {
    size_t result_length = strlen(data) * 8 + 1;
    char *result = malloc(result_length);
    result[0] = '\0';
    for (int i = 0; data[i] != '\0'; i++) {
        const char *pos = strchr(table, data[i]);
        if (!pos) {
            fprintf(stderr, "Какие-то символы не найдены в таблице. Проверьте все ли символы латинские\n");
            exit(EXIT_FAILURE);
        }
        int index = pos - table;
        int cValue = modExp(index, e, n);
        char separator = separators[rand() % strlen(separators)];
        char cryptString[16];
        snprintf(cryptString, sizeof(cryptString), "%d%c", cValue, separator);
        strncat(result, cryptString, result_length - strlen(result) - 1);
    }
    return result;
}
