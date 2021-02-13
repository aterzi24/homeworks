#include "WebHistory.hpp"

WebHistory::WebHistory()
{
    // Does nothing.
}

WebHistory::WebHistory(std::string historyText)
{
	// history = url timestamp | url timestamp | url timestamp...
    // urls are string and timestamps are non-negative integers.
    std::string delimiter = " | ";

	std::string tabInfo;
    std::string url;
    std::string timestamp;
    size_t pos = 0;

    while (true)
	{
        pos = historyText.find(delimiter);

        bool breakTheLoop = (pos == std::string::npos);

        tabInfo = historyText.substr(0, pos);
        historyText.erase(0, pos + delimiter.length());

        pos = tabInfo.find(" ");

        url = tabInfo.substr(0, pos);
        timestamp = tabInfo.substr(pos + 1, tabInfo.length() - pos);

		Node<Tab> *newPage = new Node<Tab>(Tab(url, std::atoi(timestamp.c_str())), NULL, NULL);
        insertInOrder(newPage);

        if (breakTheLoop)
		{
			break;
		}
    }
}

void WebHistory::printHistory()
{
	std::cout << "Your web history:" << std::endl;
	std::cout << "------------------------------------------------" << std::endl;
	std::cout << std::endl;
	if (history.getHead()->next == history.getTail())
	{
		std::cout << "History is empty." << std::endl;
		std::cout << std::endl;
	}
	else
	{
		Node<Tab> *node = history.getFirstNode();

		while (node != history.getTail())
		{
		    std::cout << "Page: " << node->element.getUrl() << std::endl;
			std::cout << "Last Visited: " << node->element.getTimestamp() << std::endl;
			std::cout << std::endl;
		    node = node->next;
		}
	}
	std::cout << "------------------------------------------------" << std::endl;
	std::cout << std::endl;
}

WebHistory::WebHistory(std::string url, int timestamp)
{
  // TODO
  //Node<Tab> *newPage = new Node<Tab>(Tab(url, timestamp), NULL, NULL);
  insertInOrder(new Node<Tab>(Tab(url, timestamp), NULL, NULL));
}

WebHistory::~WebHistory()
{
	// TODO
  history.removeAllNodes();
  history.removeNode(history.getHead());
  history.removeNode(history.getTail());
}

void WebHistory::insertInOrder(Node<Tab> *newPage)
{
	// TODO
  if( history.isEmpty() )
  {
    history.insertAtTheFront(newPage->element);
    return ;
  }
  Node<Tab> *node = history.getFirstNode();
  int nm = newPage->element.getTimestamp();
  while (node != history.getTail())
  {
    if( nm > node->element.getTimestamp() )
    {
      history.insertAfterGivenNode(newPage->element, node->prev);
      return ;
    }
    node = node->next;
  }
  history.insertAtTheEnd(newPage->element);
}

void WebHistory::goToPage(std::string url, int timestamp)
{
	// TODO
  if( history.isEmpty() )
  {
    history.insertAtTheFront(Tab(url, timestamp));
    return ;
  }
  Node<Tab> *node = history.getFirstNode();
  while (node != history.getTail())
  {
    if( timestamp > node->element.getTimestamp() )
    {
      history.insertAfterGivenNode(Tab(url, timestamp), node->prev);
      return ;
    }
    node = node->next;
  }
  history.insertAtTheEnd(Tab(url, timestamp));
}

void WebHistory::clearHistory()
{
	// TODO
  history.removeAllNodes();
}

void WebHistory::clearHistory(int timestamp)
{
	// TODO
  Node<Tab> *node = history.getFirstNode();
  if( node == NULL )
    return ;
  while( node != history.getTail() )
  {
    if( timestamp >= node->element.getTimestamp() )
    {
      while( node != history.getTail() )
      {
        node = node->next;
        history.removeNode(node->prev);
      }
      return ;
    }
    node = node->next;
  }

}

WebHistory WebHistory::operator+(const WebHistory &rhs) const
{
	// TODO
  WebHistory new_history;
  new_history.history = LinkedList<Tab>(this->history);
  Node<Tab> *node = rhs.history.getFirstNode();
  if( node == NULL )
    return new_history;
  while( node != rhs.history.getTail() )
  {
    new_history.insertInOrder(node);
    node = node->next;
  }
  return new_history;
}

int WebHistory::timesVisited(std::string pageName)
{
	// BONUS
  return 0;
}

std::string WebHistory::mostVisited()
{
	// BONUS
  return "";
}
