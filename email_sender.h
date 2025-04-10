#ifndef EMAIL_SENDER_H
#define EMAIL_SENDER_H

void send_email(const char *to_email, const char *subject, const char *body,
                           const char *attachment1, const char *attachment2);

#endif
