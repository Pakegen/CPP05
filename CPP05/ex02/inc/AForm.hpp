#ifndef AFORM_HPP
# define AFORM_HPP

# include <iostream>
# include <string>
# include <exception>

class Bureaucrat;

class AForm
{
private:
	AForm();

	std::string const name;
	bool signedStatus;
	int const signGrade;
	int const execGrade;

protected:
	virtual void executeAction() const = 0;

public:
	AForm(std::string const &name, int signGrade, int execGrade);
	AForm(AForm const &other);
	AForm &operator=(AForm const &other);
	virtual ~AForm();

	std::string const &getName() const;
	bool isSigned() const;
	int getSignGrade() const;
	int getExecGrade() const;

	void beSigned(Bureaucrat const &bureaucrat);
	void execute(Bureaucrat const &executor) const;

	class GradeTooHighException : public std::exception
	{
	public:
		virtual const char *what() const throw();
	};

	class GradeTooLowException : public std::exception
	{
	public:
		virtual const char *what() const throw();
	};

	class FormNotSignedException : public std::exception
	{
	public:
		virtual const char *what() const throw();
	};
};

std::ostream &operator<<(std::ostream &out, AForm const &form);

#endif
