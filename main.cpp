#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>
#include <string>

bool get_top_info_by_process(const char *process_name_key, int field_index, char *info) {
    char cmd[128] = { 0 };
    snprintf(cmd, sizeof(cmd), "top -bn 1 | grep %s", process_name_key);
    FILE *fp = popen(cmd, "r");
    if (!fp) {
        return false;
    }
    char buf[256] = { 0 };
    fgets(buf, sizeof(buf) - 1, fp);
    fclose(fp);
    int index = 0;
    int i = 0;
    int len = 0;
    const char *p = buf;
    while (buf[i]) {
        if (buf[i] != ' ' && (' ' == buf[i + 1])) {
            index++;
        }
        if (field_index == index) {
            len = buf + i - p + 1;
            if (len < 64) {
                memcpy(info, p, len);
            }
            else {
                memcpy(info, p, 63);
            }
            return true;
        }
        if ((buf[i] != ' ') && (' ' == buf[i - 1])) {
            p = buf + i;
        }
        i++;
    }
    len = buf + i - p;
    if (len < 64) {
        memcpy(info, p, len);
    }
    else {
        memcpy(info, p, 63);
    }
    return true;
}
double get_cpu_usage(const char *process_name_key) {
    char info[64] = { 0 };
    static const int CPU_INDEX_IN_TOP = 9;
    if (true == get_top_info_by_process(process_name_key, CPU_INDEX_IN_TOP, info)) {
        return atof(info);
    }
    return 0;
}
int main(int argc, const char **argv) {
    if (argc != 4) {
        printf("usage:./cpu_stat process_name stat_interval(s) stat_count!\n");
        return -1;
    }
    const char *process_name = argv[1];
    int stat_interval = atoi(argv[2]);
    int stat_count = atoi(argv[3]);
    double cpu_real_usage = 0;
    double cpu_avg_usage = 0;
    for (int i = 0;i < stat_count;i++) {
        cpu_real_usage = get_cpu_usage(process_name);
        cpu_avg_usage += cpu_real_usage;
        printf("%s cpu real usage:%lf\n", process_name, cpu_real_usage);
        printf("%s cpu average usage:%lf\n", process_name, cpu_avg_usage / (i + 1));
        sleep(stat_interval);
    }

    return 0;
}