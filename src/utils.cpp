/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 20:06:03 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/04 13:03:57 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

int ft_stoi(const std::string& str)
{
    std::stringstream ss(str);
    int i;
    ss >> i;
    return i;
}

bool is_dir(const std::string& path)
{
    struct stat s;
    return (stat(path.c_str(), &s) == 0 && S_ISDIR(s.st_mode));
}

bool is_readable_file(const std::string& path)
{
    struct stat s;
    return (stat(path.c_str(), &s) == 0 && S_ISREG(s.st_mode) &&
            (s.st_mode & S_IRUSR));
}

std::string exec_cmd(
    const std::string &cmd,
    const std::vector<std::string>& args,
    const std::map<std::string, std::string>& env)
{
    std::vector<char *> argv;
    for (std::vector<std::string>::const_iterator it = args.begin();
        it != args.end(); ++it)
        argv.push_back(const_cast<char *>(it->c_str()));
    argv.push_back(NULL);

    std::vector<char *> envp;
    for (std::map<std::string, std::string>::const_iterator it = env.begin();
        it != env.end(); ++it)
        envp.push_back(const_cast<char *>((it->first + "=" + it->second).c_str()));
    envp.push_back(NULL);

    int fd[2];
    pipe(fd);
    pid_t pid = fork();

    if (pid < 0)
        throw std::runtime_error("fork failed");
    if (pid == 0)
    {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        execvpe(cmd.c_str(), &argv[0], &envp[0]);
        throw std::runtime_error("execvpe failed");
    }
    // pid > 0 (parent)
    close(fd[1]);
    std::string body;
    char buf[1024];
    int n = 1;
    while (n > 0)
    {
        n = read(fd[0], buf, sizeof(buf));
        body.append(buf, n);
    }
    close(fd[0]);
    waitpid(pid, NULL, 0);
    return body;
}
