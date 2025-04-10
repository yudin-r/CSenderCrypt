#include "smtp_config.h"

void load_smtp_config(SMTPConfig *config) {
    *config = (SMTPConfig){
        .smtp_server = "smtp.#####.ru",//replace
        .smtp_port = 465, //replace
        .username = "#######",//replace
        .password = "#######",//replace
        .from_email = "#######" //replace
    };
}
