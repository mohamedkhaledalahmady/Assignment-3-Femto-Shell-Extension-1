#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

// parsing command
void parsing_command(char command[], char argv[][10], char *ptr_arg[])
{
    int index = 0, j = 0;
    char buf[10];
    for (int i = 0; i <= strlen(command); i++)
    {
        if (command[i] == ' ' || i == strlen(command))
        {
            buf[j] = '\0';
            strcpy(argv[index], buf);
            ptr_arg[index] = buf;
            j = 0;
            index++;
            continue;
        }
        buf[j] = command[i];
        j++;
    }
    for (int i = 0; i <= index; i++)
        ptr_arg[i] = argv[i];
    ptr_arg[index] = NULL;
}

int main(int argc, char *argv[])
{
    char new_argv[10][10];
    char *ptr_arg[10];
    int status;
    char command[100];
    char ch;
    while (1)
    {
        printf("Ana Gahz Ya Basha $ ");
        fgets(command, 100, stdin);
        int return_value = fork();
        if (return_value < 0)
            printf("Faild\n");
        else if (return_value > 0)
        {
            /* Parent Code */
            wait(&status);
        }
        else if (return_value == 0 && strlen(command) > 1)
        {
            /* Child code */
            int len = strlen(command);
            command[len - 1] = 0;
            parsing_command(command, new_argv, ptr_arg);
            if (execvp(ptr_arg[0], ptr_arg) == -1)
                printf("Unable to excecute this program\n");
            break;
        }
    }
    return 0;
}
