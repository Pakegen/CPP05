#ifndef INTERN_HPP
# define INTERN_HPP

# include <string>
# include <iostream>
# include "AForm.hpp"
# include "ShrubberyCreationForm.hpp"
# include "RobotomyRequestForm.hpp"
# include "PresidentialPardonForm.hpp"

class Intern
{
private:
	AForm *createShrubbery(std::string const &target) const;
	AForm *createRobotomy(std::string const &target) const;
	AForm *createPresidential(std::string const &target) const;

public:
	Intern();
	Intern(Intern const &other);
	Intern &operator=(Intern const &other);
	~Intern();

	AForm *makeForm(std::string const &formName, std::string const &target) const;
};

#endif
