// thx to: https://stackoverflow.com/questions/6898337/determine-programmatically-if-a-program-is-running
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>

#include <signal.h>         // kill

pid_t proc_find(const char* name)
{
    DIR* dir;
    struct dirent* ent;
    char* endptr;
    char buf[512];

    if (!(dir = opendir("/proc"))) {
        perror("can't open /proc");
        return -1;
    }

    while((ent = readdir(dir)) != NULL) {
        /* if endptr is not a null character, the directory is not
         * entirely numeric, so ignore it */
        long lpid = strtol(ent->d_name, &endptr, 10);
        if (*endptr != '\0') {
            continue;
        }

        /* try to open the cmdline file */
        snprintf(buf, sizeof(buf), "/proc/%ld/cmdline", lpid);
        FILE* fp = fopen(buf, "r");

        if (fp) {
            if (fgets(buf, sizeof(buf), fp) != NULL) {
                /* check the first token in the file, the program name */
                char* first = strtok(buf, " ");
                if (!strcmp(first, name)) {
                    fclose(fp);
                    closedir(dir);
                    return (pid_t)lpid;
                }
            }
            fclose(fp);
        }

    }

    closedir(dir);
    return -1;
}

pid_t try_start(char *cmd)
{
    // todo: change to non-blocking, not child process
    return system(cmd);
}

int try_kill(pid_t pid)
{
    int ret = kill(pid, SIGINT);
    if (ret != 0)
        ret = kill(pid, SIGKILL);   // try again

    return ret;
}

#define BUFSIZE 256

int get_cmd_string(char buf[], char* argv[], int argc)
{
    char *p = buf;

    for(int i=1; i<argc; i++) {
        if (i>1)
            p = stpcpy(p, " ");
        p = stpcpy(p, argv[i]);

        if (p > buf+BUFSIZE) {
            fprintf(stderr, "Error: Argument list too long. it should be %d characters total\n", BUFSIZE);
            return -1;
        }
    }
    // printf("'%s'\n", buf);

    return strlen(buf);
}


int main(int argc, char* argv[])
{
    if (argc == 1) {
        printf("Usage: %s proc_name args ...\n", argv[0]);
        return -1;
    }

    char *name = argv[1];
	pid_t pid = proc_find(name);
	if (pid == -1) {
		// printf("%s: No process found, so new process starts\n", name);

        char cmdline[BUFSIZE] = "";
        int ret = get_cmd_string(cmdline, argv, argc);
        if (ret < 1) {
            fprintf(stderr, "Error: Cannot get command string from arguments: '%s'\n", argv);
            return -1;
        }
        // printf("cmdline: '%s'\n", cmdline);

        if (try_start(cmdline) < 0) {
            fprintf(stderr, "Error: Cannot start '%s'\n", argv[1]);
            return -1;
        }
	} else {
        printf("%s: %d found, so trying to kill it\n", name, pid);

        if (try_kill(pid) != 0) {
            fprintf(stderr, "Error: Cannot kill '%s', PID=%d\n", argv[1], pid);
            return -1;
        }
	}
    return 0;
}
