//@	{"targets":[{"name":"pipe.o","type":"object"}]}

#include "pipe.hpp"

#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <array>
#include <string>
#include <stack>

using namespace Anja;

static void sigpipe(int signal,siginfo_t* info, void* context)
	{
	}

static void sigpipeSetFilter()
	{
	struct sigaction sa;
	sa.sa_sigaction=sigpipe;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags=SA_SIGINFO;
	sa.sa_restorer=nullptr;

	struct sigaction sa_old;

	sigaction(SIGPIPE,&sa,&sa_old);
	}


static size_t read_helper(int fd,void* buffer, size_t count)
	{
	auto pos=reinterpret_cast<uint8_t*>(buffer);
	size_t n_read=0;
	while(n_read!=count)
		{
		auto n=::read(fd,pos,count-n_read);
		if(n==0)
			{return n_read;}
		if(n==-1)
			{
			if(!( errno==EINTR || errno==EAGAIN || errno==EWOULDBLOCK))
				{throw "I/O error";}
			n=0;
			}
		pos+=n;
		n_read+=n;
		}
	return n_read;
	}

static constexpr auto PIPE_READ_END=0;
static constexpr auto PIPE_WRITE_END=1;

void Anja::pipe(const char* const* command,PipeCallbackFn cb,void* cb_obj)
	{
	sigpipeSetFilter();
	int pipe_stdout[2];
	::pipe(pipe_stdout);
	int pipe_execerror[2];
	::pipe(pipe_execerror);
	auto pid=fork();
	if(pid==0)
		{
		close(pipe_execerror[PIPE_READ_END]);
		fcntl(pipe_execerror[PIPE_WRITE_END],F_SETFD,FD_CLOEXEC);
		close(pipe_stdout[PIPE_READ_END]);
		dup2(pipe_stdout[PIPE_WRITE_END],STDOUT_FILENO);
		if(execvp(command[0],const_cast<char* const*>(command))==-1)
			{
			int status=errno;
			auto res=write(pipe_execerror[PIPE_WRITE_END],&status,sizeof(status));
			_exit(static_cast<int>(res));
			}
		}
	
	int status=0;
	close(pipe_execerror[PIPE_WRITE_END]);
	close(pipe_stdout[PIPE_WRITE_END]);
	if(read(pipe_execerror[PIPE_READ_END],&status,sizeof(status))==sizeof(status))
		{
		close(pipe_execerror[PIPE_READ_END]);
		close(pipe_stdout[PIPE_READ_END]);
		int s;
		waitpid(static_cast<int>(pid),&s,0);
		throw "Error starting child process";
		}

	constexpr size_t N=4096;
	size_t n=0;
	int8_t buffer[N];
	do
		{
		n=read_helper(pipe_stdout[PIPE_READ_END],buffer,N);
	//	FIXME: We need RAII here
		cb(cb_obj,buffer,n);
		}
	while(n==N);
	
	close(pipe_execerror[PIPE_READ_END]);
	close(pipe_stdout[PIPE_READ_END]);
	int s;
	waitpid(static_cast<int>(pid),&s,0);
	s=WEXITSTATUS(s);
	if(s!=0)
		{throw "Child process failed";}
	}
