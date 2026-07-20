#include "AForm.hpp"
#include "Bureaucrat.hpp"

AForm::AForm(std::string const &name, int signGrade, int execGrade)
	: name(name), signedStatus(false), signGrade(signGrade), execGrade(execGrade)
{
	if (this->signGrade < 1 || this->execGrade < 1)
		throw AForm::GradeTooHighException();
	if (this->signGrade > 150 || this->execGrade > 150)
		throw AForm::GradeTooLowException();
}

AForm::AForm(AForm const &other)
	: name(other.name),
	  signedStatus(other.signedStatus),
	  signGrade(other.signGrade),
	  execGrade(other.execGrade)
{
}

AForm &AForm::operator=(AForm const &other)
{
	if (this != &other)
		this->signedStatus = other.signedStatus;
	return *this;
}

AForm::~AForm()
{
}

std::string const &AForm::getName() const
{
	return this->name;
}

bool AForm::isSigned() const
{
	return this->signedStatus;
}

int AForm::getSignGrade() const
{
	return this->signGrade;
}

int AForm::getExecGrade() const
{
	return this->execGrade;
}

void AForm::beSigned(Bureaucrat const &bureaucrat)
{
	if (bureaucrat.getGrade() > this->signGrade)
		throw AForm::GradeTooLowException();
	this->signedStatus = true;
}

void AForm::execute(Bureaucrat const &executor) const
{
	if (!this->signedStatus)
		throw AForm::FormNotSignedException();
	if (executor.getGrade() > this->execGrade)
		throw AForm::GradeTooLowException();
	this->executeAction();
}

const char *AForm::GradeTooHighException::what() const throw()
{
	return "AForm grade too high";
}

const char *AForm::GradeTooLowException::what() const throw()
{
	return "AForm grade too low";
}

const char *AForm::FormNotSignedException::what() const throw()
{
	return "Form not signed";
}

std::ostream &operator<<(std::ostream &out, AForm const &form)
{
	out << form.getName()
		<< " (sign grade " << form.getSignGrade()
		<< ", exec grade " << form.getExecGrade()
		<< ", signed: " << (form.isSigned() ? "yes" : "no") << ")";
	return out;
}
