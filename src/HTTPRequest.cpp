#include <HTTPRequest.hpp>
#include <string>
#include <sstream>

HTTPRequest::HTTPRequest() {
}

HTTPRequest::~HTTPRequest() {
}

void	HTTPRequest::setMethodType(const std::string& requestLine) {
	std::istringstream	stream_line(requestLine);
	std::string word;
	stream_line >> word;
	if (word == "GET")
		_methodType = HTTPMethod::GET;
	else if (word == "POST")
		_methodType = HTTPMethod::POST;
	else if (word == "DELETE")
		_methodType = HTTPMethod::DELETE;
	else
		_methodType = HTTPMethod::UNKNOWN;
}

HTTPMethod HTTPRequest::getMethodType(void) const {
	return (_methodType);
}

void	HTTPRequest::setPath(const std::string& requestLine) {
	std::istringstream	stream_line(requestLine);
	std::string word;
	stream_line >> word >> word;
	_path = word;
}

const std::string& HTTPRequest::getPath(void) const {
	return (_path);
}

void	HTTPRequest::setVersion(const std::string& requestLine) {
	std::istringstream	stream_line(requestLine);
	std::string word;
	stream_line >> word >> word >> word;
	_version = word;
}

const std::string& HTTPRequest::getVersion(void) const {
	return (_version);
}

void	HTTPRequest::setHeader(const std::string& headerLine) {
	size_t		startPos = 0;
	size_t		foundPos = headerLine.find(": ", startPos);
	std::string	key;
	std::string	value;
	size_t		length;

	if (foundPos != std::string::npos) {
		length = foundPos - startPos;
		std::string substring = headerLine.substr(startPos, length);
		key = substring;
	}
	startPos = foundPos + 2;
	length = headerLine.length() - startPos;
	value = headerLine.substr(startPos, length);

	_headers[key] = value;
}

std::string&	HTTPRequest::getValue(const std::string& key) {
	// what if the key or value does not exist??
	return (_headers[key]);
}

void	HTTPRequest::setBody(const std::string& body) {
	_body = body;
}

const std::string&	HTTPRequest::getBody(void) const {
	return (_body);
}

void HTTPRequest::parse(const std::string& HTTPRequestString) {
	std::string inputString = HTTPRequestString;
	bool		bodyLine = false;
	bool		firstLine = true;
	size_t		startPos = 0;

	while (startPos != std::string::npos)
	{
		size_t	foundPos = inputString.find("\r\n", startPos);
		if (foundPos != std::string::npos)
		{
			size_t length = foundPos - startPos;
			std::string substring = inputString.substr(startPos, length);
			if (substring == "" && bodyLine)
				break ;
			if (substring == "")
			{
				bodyLine = true;
				startPos = foundPos + 2;
				continue ;
			}
			if (firstLine)
			{
				setMethodType(substring);
				setPath(substring);
				setVersion(substring);
				firstLine = false;
			}
			else if (bodyLine)
			{
				setBody(substring);
				break ;
			}
			else
			{
				setHeader(substring);
			}
			startPos = foundPos + 2;
		}
		else
			break ;
	}
}
