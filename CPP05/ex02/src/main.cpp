#include "Bureaucrat.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"
#include <iostream>

int main()
{
	try
	{
		Bureaucrat bob("Bob", 130);
		Bureaucrat boss("Boss", 1);

		ShrubberyCreationForm shrub("garden");
		RobotomyRequestForm robot("Bender");
		PresidentialPardonForm pardon("Arthur");

		bob.signForm(shrub);
		bob.executeForm(shrub);

		boss.signForm(robot);
		boss.executeForm(robot);

		boss.signForm(pardon);
		boss.executeForm(pardon);
	}
	catch (std::exception const &e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}
	return 0;
}
