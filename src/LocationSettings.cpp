
#include <LocationSettings.hpp>
#include <Logger.hpp>
#include <Token.hpp>

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

LocationSettings::LocationSettings()
	: _directory(), _alias(), _index(), _allowed_methods(), _cgi_path(),
	  _auto_index()
{
}

LocationSettings::LocationSettings(const LocationSettings &rhs)
	: _directory(rhs._directory), _alias(rhs._alias), _index(rhs._index),
	  _allowed_methods(rhs._allowed_methods), _cgi_path(rhs._cgi_path),
	  _auto_index(rhs._auto_index)
{
}

LocationSettings::~LocationSettings()
{
}

LocationSettings::LocationSettings(std::vector<Token>::iterator &token)
	: _directory(), _alias(), _index(), _allowed_methods(), _cgi_path(),
	  _auto_index()
{
	_directory = token->getString();
	token += 2;

	while (token->getType() != TokenType::CLOSE_BRACKET)
	{
		const Token key = *token;
		token++;

		while (token->getType() != TokenType::SEMICOLON)
		{
			if (key.getString() == "alias")
				parseAlias(*token);
			else if (key.getString() == "index")
				parseIndex(*token);
			else if (key.getString() == "allowed_methods")
				parseAllowedMethods(*token);
			else if (key.getString() == "cgi_path")
				parseCgiPath(*token);
			else if (key.getString() == "return")
				parseReturn(*token);
			else if (key.getString() == "autoindex")
				parseAutoIndex(*token);
			else
				throw std::runtime_error(
					"Parsing Error: Unknown KEY token at: " + key.getString());

			token++;
		}
		token++;
	}
}

void LocationSettings::parseAlias(const Token token)
{
	_alias.append(" " + token.getString());
}

void LocationSettings::parseIndex(const Token token)
{
	_index.append(" " + token.getString());
}

void LocationSettings::parseAllowedMethods(const Token token)
{
	_allowed_methods.append(" " + token.getString());
}

void LocationSettings::parseCgiPath(const Token token)
{
	_cgi_path.append(" " + token.getString());
}

void LocationSettings::parseReturn(const Token token)
{
	_return.append(" " + token.getString());
}

void LocationSettings::parseAutoIndex(const Token token)
{
	if (token.getString() == "on")
		_auto_index = true;
	else if (token.getString() == "off")
		_auto_index = false;
	else
		throw std::runtime_error("Parsing Error: Unknown VALUE token at: " +
								 token.getString());
}

// Functionality:
//		getters:
const std::string &LocationSettings::getDir() const
{
	return (_directory);
}

const std::string &LocationSettings::getAlias() const
{
	return (_alias);
}

const std::string &LocationSettings::getIndex() const
{
	return (_index);
}

const std::string &LocationSettings::getAllowedMethods() const
{
	return (_allowed_methods);
}

bool LocationSettings::getAutoIndex() const
{
	return (_auto_index);
}

const std::string &LocationSettings::getReturn() const
{
	return (_return);
}

//		setters:
void LocationSettings::setDir(const std::string &direcotry)
{
	_directory = direcotry;
}

// THIS IS PRINTING FUNCTION

void LocationSettings::printLocationSettings() const
{
	Logger &logger = Logger::getInstance();

	logger.log(DEBUG, "\tLocation Prefix:\t" +
						  (_directory.empty() ? " -[EMPTY]-" : _directory));
	logger.log(DEBUG,
			   "\t\tAlias:\t\t\t" + (_alias.empty() ? " -[EMPTY]-" : _alias));
	logger.log(DEBUG,
			   "\t\tIndex:\t\t\t" + (_index.empty() ? " -[EMPTY]-" : _index));
	logger.log(DEBUG, "\t\tAllowed_methods:\t" + (_allowed_methods.empty()
													  ? " -[EMPTY]-"
													  : _allowed_methods));
	logger.log(DEBUG, "\t\tCGI Path:\t\t" +
						  (_cgi_path.empty() ? " -[EMPTY]-" : _cgi_path));
	logger.log(DEBUG, "\t\tReturn:\t\t\t" +
						  (_return.empty() ? " -[EMPTY]-" : _return));
	logger.log(DEBUG,
			   "\t\tAutoIndex:\t\t" +
				   (_auto_index ? std::string(" ON") : std::string(" OFF")));
}
