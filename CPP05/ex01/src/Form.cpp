#include "Form.hpp"
#include "Bureaucrat.hpp"

Form::Form(std::string const &name, int signGrade, int execGrade)
	: name(name), signedStatus(false), signGrade(signGrade), execGrade(execGrade)
{
	if (this->signGrade < 1 || this->execGrade < 1)
		throw Form::GradeTooHighException();
	if (this->signGrade > 150 || this->execGrade > 150)
		throw Form::GradeTooLowException();
}

Form::Form(Form const &other)
	: name(other.name),
	  signedStatus(other.signedStatus),
	  signGrade(other.signGrade),
	  execGrade(other.execGrade)
{
}

Form &Form::operator=(Form const &other)
{
	if (this != &other)
		this->signedStatus = other.signedStatus;
	return *this;
}

Form::~Form()
{
}

std::string const &Form::getName() const
{
	return this->name;
}

bool Form::isSigned() const
{
	return this->signedStatus;
}

int Form::getSignGrade() const
{
	return this->signGrade;
}

int Form::getExecGrade() const
{
	return this->execGrade;
}

void Form::beSigned(Bureaucrat const &bureaucrat)
{
	if (bureaucrat.getGrade() > this->signGrade)
		throw Form::GradeTooLowException();
	this->signedStatus = true;
}

const char *Form::GradeTooHighException::what() const throw()
{
	return "Form grade too high";
}

const char *Form::GradeTooLowException::what() const throw()
{
	return "Form grade too low";
}

std::ostream &operator<<(std::ostream &out, Form const &form)
{
	out << form.getName()
		<< " (sign grade " << form.getSignGrade()
		<< ", exec grade " << form.getExecGrade()
		<< ", signed: " << (form.isSigned() ? "yes" : "no") << ")";
	return out;
}
