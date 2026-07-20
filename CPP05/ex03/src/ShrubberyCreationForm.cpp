#include "ShrubberyCreationForm.hpp"
#include "Bureaucrat.hpp"
#include <fstream>
#include <iostream>

ShrubberyCreationForm::ShrubberyCreationForm(std::string const &target)
	: AForm("ShrubberyCreationForm", 145, 137), target(target)
{
}

ShrubberyCreationForm::ShrubberyCreationForm(ShrubberyCreationForm const &other)
	: AForm(other), target(other.target)
{
}

ShrubberyCreationForm &ShrubberyCreationForm::operator=(ShrubberyCreationForm const &other)
{
	if (this != &other)
	{
		AForm::operator=(other);
		this->target = other.target;
	}
	return *this;
}

ShrubberyCreationForm::~ShrubberyCreationForm()
{
}

void ShrubberyCreationForm::executeAction() const
{
	std::ofstream file((this->target + "_shrubbery").c_str());
	if (!file)
		throw ShrubberyCreationForm::FileOpenException();

	file << "       /\\\n";
	file << "      /**\\\n";
	file << "     /****\\\n";
	file << "    /******\\\n";
	file << "   /********\\\n";
	file << "  /**********\\\n";
	file << " /************\\\n";
	file << "      ||||\n";
	file << "      ||||\n";
}

const char *ShrubberyCreationForm::FileOpenException::what() const throw()
{
	return "Could not create shrubbery file";
}
