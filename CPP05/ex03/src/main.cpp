#include "Intern.hpp"
#include "Bureaucrat.hpp"
#include <iostream>

int main()
{
	Intern intern;
	Bureaucrat boss("Boss", 1);

	AForm *robot = intern.makeForm("robotomy request", "Bender");
	AForm *shrub = intern.makeForm("shrubbery creation", "home");
	AForm *pardon = intern.makeForm("presidential pardon", "Arthur");
	AForm *invalid = intern.makeForm("unknown form", "Nobody");

	if (robot)
	{
		boss.signForm(*robot);
		boss.executeForm(*robot);
		delete robot;
	}
	if (shrub)
	{
		boss.signForm(*shrub);
		boss.executeForm(*shrub);
		delete shrub;
	}
	if (pardon)
	{
		boss.signForm(*pardon);
		boss.executeForm(*pardon);
		delete pardon;
	}
	delete invalid;
	return 0;
}
