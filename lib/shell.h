#ifndef SHELL_H
#define SHELL_H

void shell_init();
void shell_prompt();
void shell_handle_input(char c);
void shell_execute(const char* input);
void shell_cmd_help();

#endif