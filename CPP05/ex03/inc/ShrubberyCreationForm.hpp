#ifndef SHRUBBERYCREATIONFORM_HPP
# define SHRUBBERYCREATIONFORM_HPP

# include "AForm.hpp"
# include <string>

class ShrubberyCreationForm : public AForm
{
private:
	std::string target;

	virtual void executeAction() const;

public:
	ShrubberyCreationForm(std::string const &target);
	ShrubberyCreationForm(ShrubberyCreationForm const &other);
	ShrubberyCreationForm &operator=(ShrubberyCreationForm const &other);
	virtual ~ShrubberyCreationForm();

	class FileOpenException : public std::exception
	{
	public:
		virtual const char *what() const throw();
	};
};

#endif
