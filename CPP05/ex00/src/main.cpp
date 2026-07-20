#include "Bureaucrat.hpp"
#include <iostream>

int main()
{
	try
	{
		Bureaucrat bob("Bob", 100);
		std::cout << bob << std::endl;
		bob.incrementGrade();
		std::cout << bob << std::endl;
		bob.decrementGrade();
		std::cout << bob << std::endl;

		Bureaucrat top("Top", 1);
		std::cout << top << std::endl;
		top.incrementGrade();
	}
	catch (std::exception const &e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}

	try
	{
		Bureaucrat bad("Bad", 151);
		std::cout << bad << std::endl;
	}
	catch (std::exception const &e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}

	try
	{
		Bureaucrat zero("Zero", 0);
		std::cout << zero << std::endl;
	}
	catch (std::exception const &e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}

	try
	{
		Bureaucrat lowest("Lowest", 150);
		std::cout << lowest << std::endl;
		lowest.decrementGrade();
	}
	catch (std::exception const &e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}

	return 0;
}
