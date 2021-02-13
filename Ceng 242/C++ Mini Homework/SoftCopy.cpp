#include "SoftCopy.h"
/* YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. DO NOT MODIFY THE HEADER FILE AS IT WILL BE
REPLACED BY THE ORIGINAL HEADER DURING TESTING. THE REQUIRED
HEADER FILE IS ALREADY INCLUDED FOR YOU ABOVE THIS COMMENT BLOCK.
THE CONSTRUCTOR and DESTRUCTOR ARE ALREADY IMPLEMENTED.
START YOUR IMPLEMENTATIONS BELOW THOSE. */


// DONE
SoftCopy::SoftCopy(int ISBN, double price, int startPage, int endPage) : Book(ISBN, price) {
	this->startPage = startPage;
	this->endPage = endPage;
	this->numberOfPages = (endPage - startPage) + 1;
	this->firstHalf = NULL;
	this->secondHalf = NULL;
	this->pageContent = "";
}

// DONE
SoftCopy::~SoftCopy() {
	if (firstHalf != NULL)
		delete firstHalf;
	if (secondHalf != NULL)
		delete secondHalf;
}

SoftCopy::SoftCopy(const SoftCopy& rhs) : firstHalf(NULL), secondHalf(NULL)
{
	*this = rhs;
}

SoftCopy& SoftCopy::operator=(const SoftCopy& rhs)
{
	if(this == &rhs)
		return *this;

	this->ISBN = rhs.ISBN;
	this->price = rhs.price;
	this->next = rhs.next;
	this->startPage = rhs.startPage;
	this->endPage = rhs.endPage;
	this->numberOfPages = rhs.numberOfPages;
	this->pageContent = rhs.pageContent;

	if (firstHalf != NULL)
		delete firstHalf;
	if (secondHalf != NULL)
		delete secondHalf;

	if(rhs.firstHalf)
		this->firstHalf = new SoftCopy(*rhs.firstHalf);
	else
		this->firstHalf = NULL;

	if(rhs.secondHalf)
		this->secondHalf = new SoftCopy(*rhs.secondHalf);
	else
		this->secondHalf = NULL;

	return *this;
}

Book* SoftCopy::Clone() const
{
	Book *clone = new SoftCopy(*this);
	return clone;
}

void SoftCopy::Discount()
{
	price *= 0.75;
}

void SoftCopy::UploadPage(string content, int id)
{
	if(numberOfPages != 1) // we need to go deeper
	{
		int middle = (startPage + endPage) / 2;
		if(id > middle) // secondHalf
		{
			if(secondHalf == NULL) // secondHalf does not exist
				secondHalf = new SoftCopy(ISBN, price, middle+1, endPage);

			secondHalf->UploadPage(content, id);
		}
		else //firstHalf
		{
			if(firstHalf == NULL) // firstHalf does not exist
				firstHalf = new SoftCopy(ISBN, price, startPage, middle);

			firstHalf->UploadPage(content, id);
		}
	}
	else // we found it
		pageContent = content;
}

string SoftCopy::Display(int from, int to) const
{
	if(numberOfPages == 1)
		return pageContent;

	string firstStr, secondStr;
	int middle = (startPage + endPage) / 2;

	if(from <= middle && firstHalf)
		firstStr = firstHalf->Display(from, to);

	if(middle < to && secondHalf)
		secondStr = secondHalf->Display(from, to);

	if(firstStr == "")
		return secondStr;
	if(secondStr == "")
		return firstStr;
	firstStr.push_back('\n');
	firstStr.append(secondStr);
	return firstStr;
}

SoftCopy& SoftCopy::operator+(const SoftCopy& rhs) const
{
	SoftCopy *newBook = new SoftCopy(*this);

	vector<SoftCopy*> stack;
	stack.push_back((SoftCopy *)&rhs);

	while(stack.size())
	{
		SoftCopy *temp = stack[stack.size()-1];
		stack.pop_back();
		if(temp->numberOfPages == 1)
		{
			newBook->UploadPage(temp->pageContent, temp->startPage);
			continue;
		}
		if(temp->secondHalf)
			stack.push_back(temp->secondHalf);
		if(temp->firstHalf)
			stack.push_back(temp->firstHalf);
	}

	return *newBook;
}
