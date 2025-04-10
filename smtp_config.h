#ifndef SMTP_CONFIG_H
#define SMTP_CONFIG_H

typedef struct {
    const char *smtp_server;
    int smtp_port;
    const char *username;
    const char *password;
    const char *from_email;
} SMTPConfig;

void load_smtp_config(SMTPConfig *config);

#endif // SMTP_CONFIG_H
