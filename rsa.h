#ifndef RSA_H
#define RSA_H

int isPrime(int n);

int randNumPrime(int min, int max);

int gcd(int a, int b);

int modInverse(int a, int m);

int modExp(int base, int exp, int mod);

void rsa_generate_keys(int *e, int *n, int *d);

char *rsa_encrypt(const char *data, int e, int n);

char *rsa_decrypt(const char *encrypted_data, int d, int n);

#endif // RSA_H
