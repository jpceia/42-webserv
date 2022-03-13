/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 20:06:03 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/07 21:23:00 by jpceia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/poll.h>
#include <unistd.h>
#include <limits.h>
#include <cstdlib>

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

bool is_executable_file(const std::string& path)
{
    struct stat s;
    return (stat(path.c_str(), &s) == 0 && S_ISREG(s.st_mode) &&
            (s.st_mode & S_IXUSR));
}

struct pollfd create_pollfd(int fd, short int events)
{
    struct pollfd pfd;

    pfd.fd = fd;
    pfd.events = events;
    pfd.revents = 0;
    return pfd;
}

std::string lookup_full_path(const std::string& path)
{
    if (path[0] == '/') // path is already absolute
        return path;
    if (path[0] == '.') // path is relative to current directory
    {
        char buff[PATH_MAX];
        getcwd(buff, PATH_MAX);
        return std::string(buff) + "/" + path.substr(1);
    }
    else // needs to iterate over the path to find the first executable
    {
        // get PATH from environment variable
        std::stringstream env_paths;
        env_paths << getenv("PATH");

        // split PATH into directories
        for (std::string dir; std::getline(env_paths, dir, ':'); )
        {
            std::string full_path = dir + "/" + path;
            if (is_executable_file(full_path))
                return full_path;
        }
        throw std::runtime_error("Could not find executable " + path);
    }
}


std::string exec_cmd(
    const std::string &path,
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
        std::string full_path = lookup_full_path(path);
        execve(full_path.c_str(), &argv[0], &envp[0]);
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
