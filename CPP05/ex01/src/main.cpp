#include "Bureaucrat.hpp"
#include "Form.hpp"
#include <iostream>

int main()
{
	try
	{
		Bureaucrat bob("Bob", 50);

		Form form1("Form A", 60, 30);
		std::cout << form1 << std::endl;
		bob.signForm(form1);
		std::cout << form1 << std::endl;

		Form form2("Form B", 40, 20);
		bob.signForm(form2);

		Form invalid("Invalid", 0, 50);
		std::cout << invalid << std::endl;
	}
	catch (std::exception const &e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}

	try
	{
		Form tooLow("Too Low", 151, 100);
		std::cout << tooLow << std::endl;
	}
	catch (std::exception const &e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}
	return 0;
}
