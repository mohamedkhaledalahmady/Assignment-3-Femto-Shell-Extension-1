#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // for setenv

#define INTERNAL_COMMAND 0
#define EXTERNAL_COMMAND 1

/* local variables and their values */
char local_var[10][10];
char local_var_value[10][10];
int index_local_var = 0;

/* Ths function to chech if internal or external command */
int check_command_type(char command[])
{
    int i = 0;
    while (command[i])
    {
        if (command[i] == '=')
            return INTERNAL_COMMAND;
        i++;
    }
    return EXTERNAL_COMMAND;
}

void parsing_external_command(char command[], char argv[][10], char *ptr_arg[])
{
    int index = 0, j = 0;
    char buf[10];
    int mask = 0;
    for (int i = 0; i <= strlen(command); i++)
    {
        // to handle multiple spaces
        if (command[i] == ' ' && mask == 1)
            continue;

        else if (command[i] == ' ' || i == strlen(command) && mask == 0)
        {
            mask = 1;
            buf[j] = '\0';
            strcpy(argv[index], buf);
            ptr_arg[index] = buf;
            j = 0;
            index++;
        }
        else
        {
            mask = 0;
            buf[j] = command[i];
            j++;
        }
    }
    for (int i = 0; i <= index; i++)
        ptr_arg[i] = argv[i];
    ptr_arg[index] = NULL;
}

void parsing_internal_command(char command[], char local_var[][10], char local_var_value[][10], int *index)
{
    int len = strlen(command);
    int j = 0, index1 = *index, index2 = *index;
    char buf[10];
    *index += 1;
    for (int i = 0; i < len; i++)
    {
        if (command[i] == '=')
        {
            buf[j] = '\0';
            strcpy(local_var[index1], buf);
            j = 0;
            index1++;
            i++;
        }
        if (i == len - 1)
        {
            buf[j] = command[i];
            j++;
            buf[j] = '\0';
            strcpy(local_var_value[index2], buf);
            j = 0;
            index2++;
        }
        buf[j] = command[i];
        j++;
    }
}

void set_command(int index, char local_var[][10], char local_var_value[][10])
{
    if (index == 0)
        printf("no local variable yet!\n");
    for (int i = 0; i < index; i++)
        printf("local_var[%d]: %s = %s\n", i, local_var[i], local_var_value[i]);
}

int search_in_local_var(char variable[])
{
    int i = 0;

    for (int j = 0; j < index_local_var; j++)
    {
        if (strcmp(variable, local_var[i]) != 0)
            i++;
    }
    if (i == index_local_var)
    {
        return -1;
    }
    else
        return i;
}

void export_command(char *ptr_arg[])
{
    int i = search_in_local_var(ptr_arg[1]);
    if (i != -1)
    {
        setenv(ptr_arg[1], local_var_value[i], 1);
    }
    else
        printf("%s not exist\n", ptr_arg[1]);
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
            if (check_command_type(command) == INTERNAL_COMMAND)
            {
                parsing_internal_command(command, local_var, local_var_value, &index_local_var);
            }
            else if (strcmp(command, "set") == 0)
                set_command(index_local_var, local_var, local_var_value);
            else if (check_command_type(command) == EXTERNAL_COMMAND)
            {
                parsing_external_command(command, new_argv, ptr_arg);
                if (strcmp(ptr_arg[0], "export") == 0)
                {
                    export_command(ptr_arg);
                }
                else
                {
                    if (execvp(ptr_arg[0], ptr_arg) == -1)
                        printf("%s: command not found\n", ptr_arg[0]);
                    break;
                }
            }
        }
    }
    return 0;
}
