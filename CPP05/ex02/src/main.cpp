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

	std::cout << "\n--- Now testing failure cases ---" << std::endl;

	try
	{
		Bureaucrat intern("Intern", 150);
		Bureaucrat director("Director", 1);
		ShrubberyCreationForm unsignedShrub("backyard");

		intern.signForm(unsignedShrub);     // grade 150 > signGrade 145 -> sign fails
		director.executeForm(unsignedShrub); // form still not signed -> should throw
	}
	catch (std::exception const &e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}

	try
	{
		Bureaucrat clerk("Clerk", 20);
		PresidentialPardonForm pardon2("Someone");

		clerk.signForm(pardon2);    // 20 <= 25 -> sign succeeds
		clerk.executeForm(pardon2); // 20 > 5   -> execute should throw
	}
	catch (std::exception const &e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}

	return 0;
}
