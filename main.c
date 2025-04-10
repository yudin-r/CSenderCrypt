#include <stdio.h>
#include <stdlib.h>
#include "rsa.h"
#include "email_sender.h"

void create_file(const char *content, const char *filename) {
    FILE *file = fopen(filename, "w");
    fprintf(file, "%s", content);
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Использование: %s <email> <тема> <текст>\n", argv[0]);
        return 1;
    }

    const char *to_email = argv[1];
    const char *subject = argv[2];
    const char *original_text = argv[3];

    int e, n, d;
    rsa_generate_keys(&e, &n, &d);
    printf("Открытый ключ: (e=%d, n=%d)\n", e, n);
    printf("Закрытый ключ: (d=%d, n=%d)\n", d, n);

    char buffer[100];
    snprintf(buffer, sizeof(buffer), "%d %d", d, n);

    char *encrypted_text = rsa_encrypt(original_text, e, n);
    printf("Зашифрованный текст: %s\n", encrypted_text);

    create_file(encrypted_text, "data.txt");
    create_file(buffer, "key.txt");

    send_email(to_email, subject, "Зашифрованное сообщение и закрытый ключ во вложениях",
                      "data.txt", "key.txt");

    free(encrypted_text);
}
