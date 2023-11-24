#include <Logger.hpp>
#include <Socket.hpp>
#include <SystemException.hpp>

#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

#include <cassert>

Socket::Socket(int server_fd)
	: _addr_len(sizeof(_addr)),
	  _fd(accept(server_fd, (t_sockaddr *)&_addr, &_addr_len))
{
	Logger &logger = Logger::getInstance();

	if (getFD() == SYSTEM_ERROR)
		throw SystemException("Accept");
	logger.log(INFO,
			   " created client socket on fd: " + std::to_string(getFD()));
}

void Socket::setupClient(void)
{
	Logger &logger = Logger::getInstance();

	assert(fcntl(getFD(), F_SETFL, O_NONBLOCK) != SYSTEM_ERROR);
	char address[INET_ADDRSTRLEN];
	if (inet_ntop(AF_INET, &_addr.sin_addr, address, sizeof(address)) == NULL)
		throw SystemException("inet_ntop failed");
	logger.log(INFO, "Connection received from " + std::string(address));
}

Socket::Socket() : _fd(socket(AF_INET, SOCK_STREAM, 0))
{
	Logger &logger = Logger::getInstance();

	if (getFD() == SYSTEM_ERROR)
		throw SystemException("socket creation failed");
	logger.log(DEBUG,
			   "Created server socket on fd: " + std::to_string(getFD()));
}

Socket::~Socket()
{
	assert(close(getFD()) != SYSTEM_ERROR);
}

void Socket::initSockaddrIn(t_sockaddr_in &addr, const std::string &host,
							const std::string &port)
{
	addrinfo hints{};
	addrinfo *result = NULL;
	int status = 0;

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	status = getaddrinfo(host.c_str(), port.c_str(), &hints, &result);
	if (status != 0)
		throw GetAddrInfoException(gai_strerror(status));
	if (result == NULL)
		throw SystemException("getaddrinfo returned NULL");
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	/* getaddrinfo(host.c_str(), port.c_str(), NULL, addr); */
	addr.sin_addr.s_addr = inet_addr(host.c_str());
	addr.sin_port = htons(std::stoi(port)); // TODO: stoi exception
	std::fill_n(addr.sin_zero, sizeof(addr.sin_zero), '\0');
}

void Socket::setupServer(const std::string &host, const std::string &port)
{
	int option = 1;
	if (fcntl(getFD(), F_SETFL, O_NONBLOCK) == SYSTEM_ERROR)
		throw SystemException("fcntl failed");
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) ==
		SYSTEM_ERROR)
		throw SystemException("setsockopt failed");
	initSockaddrIn(_addr, host, port);
	if (bind(getFD(), (t_sockaddr *)&_addr, sizeof(t_sockaddr_in)) ==
		SYSTEM_ERROR)
		throw SystemException("Bind");
	if (listen(getFD(), MAX_PENDING_CONNECTIONS) == SYSTEM_ERROR)
		throw SystemException("Listen");
}

int Socket::getFD() const
{
	return (_fd);
}
