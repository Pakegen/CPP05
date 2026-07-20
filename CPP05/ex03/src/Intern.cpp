#include "Intern.hpp"

Intern::Intern()
{
}

Intern::Intern(Intern const &)
{
}

Intern &Intern::operator=(Intern const &)
{
	return *this;
}

Intern::~Intern()
{
}

AForm *Intern::createShrubbery(std::string const &target) const
{
	return new ShrubberyCreationForm(target);
}

AForm *Intern::createRobotomy(std::string const &target) const
{
	return new RobotomyRequestForm(target);
}

AForm *Intern::createPresidential(std::string const &target) const
{
	return new PresidentialPardonForm(target);
}

AForm *Intern::makeForm(std::string const &formName, std::string const &target) const
{
	typedef AForm *(Intern::*FormCreator)(std::string const &) const;

	std::string const names[3] = {
		"shrubbery creation",
		"robotomy request",
		"presidential pardon"
	};

	FormCreator const creators[3] = {
		&Intern::createShrubbery,
		&Intern::createRobotomy,
		&Intern::createPresidential
	};

	for (int i = 0; i < 3; ++i)
	{
		if (names[i] == formName)
		{
			std::cout << "Intern creates " << formName << std::endl;
			return (this->*creators[i])(target);
		}
	}

	std::cout << "Intern couldn't find the form: " << formName << std::endl;
	return NULL;
}
