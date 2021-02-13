#include "Bank.h"

Bank::Bank(): _bank_name("not_defined"), _user_count(0), _users(nullptr)
{
}

Bank::Bank(std::string bank_name, Account* const users, int user_count)
{
    _bank_name = bank_name;
    _user_count = user_count;

    if(!_user_count)
    {
        _users = nullptr;
        return ;
    }

    _users = new Account [_user_count];
    for( int i = 0 ; i < _user_count ; i++ )
        _users[i] = users[i];
}

Bank::~Bank()
{
    delete [] _users;
}

Bank::Bank(const Bank& rhs)
{
    _bank_name = rhs._bank_name;
    _user_count = rhs._user_count;

    if(!_user_count)
    {
        _users = nullptr;
        return ;
    }

    _users = new Account [_user_count];
    for( int i = 0 ; i < _user_count ; i++ )
        _users[i] = rhs._users[i];
}

Bank& Bank::operator+=(const Bank& rhs)
{
    int i, j, new_size;
    Account* new_users = new Account [_user_count + rhs._user_count];

    for( i = 0 ; i < _user_count ; i++ )
        new_users[i] = _users[i];

    new_size = _user_count;

    for( i = 0 ; i < rhs._user_count ; i++ )
    {
        for( j = 0 ; j < _user_count ; j++ )
            if(rhs._users[i] == new_users[j])
            {
                new_users[j] += rhs._users[i];
                break;
            }
        if(j == _user_count)
            new_users[new_size++] = rhs._users[i];
    }

    delete [] _users;

    if(new_size == _user_count + rhs._user_count)
    {
        _user_count = new_size;
        _users = new_users;
        return *this;
    }
    _user_count = new_size;
    _users = new Account [new_size];
    for( i = 0 ; i < new_size ; i++ )
        _users[i] = new_users[i];
    delete [] new_users;
    return *this;
}

Bank& Bank::operator+=(const Account& new_acc)
{
    int i;
    for( i = 0 ; i < _user_count ; i++ )
        if(_users[i] == new_acc)
        {
            _users[i] += new_acc;
            return *this;
        }
    Account* new_users = new Account [_user_count+1];

    for( i = 0 ; i < _user_count ; i++ )
        new_users[i] = _users[i];
    new_users[i] = new_acc;

    delete [] _users;
    _user_count++;
    _users = new_users;
    return *this;
}

Account& Bank::operator[](int account_id)
{
    for( int i = 0 ; i < _user_count ; i++ )
        if(_users[i] == account_id)
            return _users[i];
    return _users[0];
}

std::ostream& operator<<(std::ostream& os, const Bank& bank)
{
    double balance[12], sum(0);
    int i, j;
    int eligiblePerson(0);

    struct tm some_date = {0};
    some_date.tm_hour = 0;   some_date.tm_min = 0; some_date.tm_sec = 0;
    time_t end_date;

    for( i = 0 ; i < bank._user_count ; i++ )
    {
        some_date.tm_year = 119; some_date.tm_mon = 1; some_date.tm_mday = 1;
        for( j = 0 ; j < 12 ; j++ )
        {
            end_date = mktime(&some_date);
            balance[j] = bank._users[i].balance(end_date);
            if(j)
            {
                balance[j] -= balance[j-1];
                if(balance[j] < 0 && balance[j-1] < 0)
                    break;
            }
            some_date.tm_mon++;
        }
        sum += bank._users[i].balance();
        if(j == 12)
            eligiblePerson++;
    }
    os << bank._bank_name << '\t' << eligiblePerson << '\t' << sum << '\n';
}
