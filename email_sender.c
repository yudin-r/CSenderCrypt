#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "smtp_config.h"

void send_email(const char *to_email, const char *subject, const char *body,
                const char *attachment1_path, const char *attachment2_path) {
    SMTPConfig config;
    load_smtp_config(&config);

    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Не удалось инициализировать CURL.\n");
        return;
    }

    curl_mime *mime = curl_mime_init(curl);
    curl_mimepart *part;

    part = curl_mime_addpart(mime);
    curl_mime_data(part, body, CURL_ZERO_TERMINATED);

    part = curl_mime_addpart(mime);
    curl_mime_filedata(part, attachment1_path);
    curl_mime_filename(part, attachment1_path);

    part = curl_mime_addpart(mime);
    curl_mime_filedata(part, attachment2_path);
    curl_mime_filename(part, attachment2_path);

    char smtp_url[256];
    snprintf(smtp_url, sizeof(smtp_url), "smtps://%s:%d", config.smtp_server, config.smtp_port);

    curl_easy_setopt(curl, CURLOPT_URL, smtp_url);
    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
    curl_easy_setopt(curl, CURLOPT_USERNAME, config.username);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, config.password);
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, config.from_email);

    struct curl_slist *recipients = NULL;
    recipients = curl_slist_append(recipients, to_email);
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: multipart/mixed");

    char subject_header[256];
    snprintf(subject_header, sizeof(subject_header), "Subject: %s", subject);
    headers = curl_slist_append(headers, subject_header);

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "Ошибка: %s\n", curl_easy_strerror(res));
    } else {
        printf("Письмо успешно отправлено на адрес %s с вложениями:\n%s\n%s\n",
               to_email, attachment1_path, attachment2_path);
    }

    curl_slist_free_all(recipients);
    curl_slist_free_all(headers);
    curl_mime_free(mime);
    curl_easy_cleanup(curl);
}