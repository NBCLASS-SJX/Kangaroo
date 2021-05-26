#include "common.h"
#include "logger.h"

int main()
{
    std::string username = get_config_prop_string("mysql.username", "develop");
    std::string password = get_config_prop_string("mysql.password", "password");
    std::string log_dirs = get_config_prop_string("mysql.log_dirs", ".\\logs");
    int keep_days = get_config_prop_int("mysql.keep_days", "7");
    logger_worker_thread_start(log_dirs.c_str(), keep_days, LOG_LEVEL_DEBUG);

    log_queue_t *logger = logger_queue_create("test");
    for (int i = 1; i <= 100; i++) {
        log_debug(logger, "this is %d", i);
        log_info(logger, "this is %d", i);
        log_warn(logger, "this is %d", i);
        log_error(logger, "this is %d", i);
    }
    msleep(1000);

    logger_worker_thread_stop();
    logger_queue_free(logger);
    return 0;
}
