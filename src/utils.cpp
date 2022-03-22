/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpceia <joao.p.ceia@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 20:06:03 by jpceia            #+#    #+#             */
/*   Updated: 2022/03/22 00:11:19 by jpceia           ###   ########.fr       */
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
#include <iostream>

bool compareFunction (std::string a, std::string b)
{
	return a<b;
}

int ft_stoi(const std::string& str)
{
    std::stringstream ss(str);
    int i;
    ss >> i;
    return i;
}

std::string ft_itos(int i)
{
    std::stringstream ss;
    ss << i;
    return ss.str();
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

std::vector<char *> char_ptr_vector(const std::vector<std::string>& args)
{
    std::vector<char *> c_args;
    for (std::vector<std::string>::const_iterator it = args.begin();
        it != args.end(); ++it)
        c_args.push_back(const_cast<char *>(it->c_str()));
    c_args.push_back(NULL);
    return c_args;
}

std::vector<std::string> map_to_vector(const std::map<std::string, std::string>& map)
{
    std::vector<std::string> v;
    for (std::map<std::string, std::string>::const_iterator it = map.begin();
        it != map.end(); ++it)
        v.push_back(it->first + "=" + it->second);
    return v;
}

std::string exec_cmd(
    const std::string &path,
    const std::vector<std::string>& args,
    const std::map<std::string, std::string>& env,
    std::string& input)
{
	pid_t		pid;
	int			saveStdin;
	std::string	newBody;

    (void)args;

    std::vector<std::string> aux = map_to_vector(env);
    std::vector<char *> envp = char_ptr_vector(aux);

	// SAVING STDIN AND STDOUT IN ORDER TO TURN THEM BACK TO NORMAL LATER
	saveStdin = dup(STDIN_FILENO);

	FILE	*fOut = tmpfile();
	long	fdOut = fileno(fOut);
    int fd_in[2];
    pipe(fd_in);
	int		ret = 1;

	write(fd_in[1], input.c_str(), input.size());
	close(fd_in[1]);

	pid = fork();

	if (pid == -1)
		return ("Status: 500\r\n\r\n");
	else if (!pid)
	{
		dup2(fd_in[0], STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		execve(path.c_str(), (char * const * )NULL, &envp[0]);
		write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
	}
	else
	{
		char	buffer[1024] = {0};

		waitpid(-1, NULL, 0);
		lseek(fdOut, 0, SEEK_SET);

		ret = 1;
		while (ret > 0)
		{
			memset(buffer, 0, sizeof (buffer));
			ret = read(fdOut, buffer, sizeof (buffer) - 1);
			newBody += buffer;
		}
	}

	dup2(saveStdin, STDIN_FILENO);
	fclose(fOut);
	close(fd_in[0]);
	close(fdOut);
	close(saveStdin);

	return (newBody);
}
