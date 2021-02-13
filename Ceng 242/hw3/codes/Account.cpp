#include "Account.h"

void mySortAccount(Transaction *ptr, int accSize)
{
    int i, j;
    Transaction temp;
    bool anySwap;

    for( i = 0 ; i < accSize - 1 ; i++ )
    {
        anySwap = false;
        for( j = 0 ; j < accSize - 1 - i ; j++ )
            if(ptr[j] > ptr[j+1])
            {
                temp = ptr[j];
                ptr[j] = ptr[j+1];
                ptr[j+1] = temp;
                anySwap = true;
            }

        if(!anySwap)
            return ;
    }
}

Account::Account(): _id(-1), _activity(nullptr), _monthly_activity_frequency(nullptr)
{
}

Account::Account(int id, Transaction** const activity, int* monthly_activity_frequency)
{
    int i, j, accSize;

    _id = id;

    _monthly_activity_frequency = new int[12];
    _activity = new Transaction* [12]();

    for( i = 0 ; i < 12 ; i++ )
    {
        accSize = monthly_activity_frequency[i];
        _monthly_activity_frequency[i] = accSize;
        if(accSize)
        {
            _activity[i] = new Transaction [accSize];

            for( j = 0 ; j < accSize ; j++ )
                _activity[i][j] = activity[i][j];

                mySortAccount(_activity[i], accSize);
        }
    }
}

Account::~Account()
{
    if(_activity != nullptr)
        for( int i = 0 ; i < 12 ; i++ )
            delete [] _activity[i];

    delete [] _activity;
    delete [] _monthly_activity_frequency;
}

Account::Account(const Account& rhs)
{
    _id = -1;
    _activity = nullptr;
    _monthly_activity_frequency = nullptr;
    *this = rhs;
}

Account::Account(const Account& rhs, time_t start_date, time_t end_date)
{
    int i, j, k, accSize, state, new_size;

    state = 0;

    _id = rhs._id;

    _monthly_activity_frequency = new int [12];
    _activity = new Transaction* [12]();

    for( i = 0 ; i < 12 ; i++ )
    {
        if(state == 2)
        {
            _monthly_activity_frequency[i] = 0;
            _activity[i] = nullptr;
            continue;
        }
        new_size = 0;
        accSize = rhs._monthly_activity_frequency[i];
        for( j = 0 ; j < accSize ; j++ )
        {
            if(state == 0)
            {
                if(rhs._activity[i][j] > start_date)
                {
                    state = 1;
                    if(rhs._activity[i][j] < end_date)
                        new_size++;
                    else
                    {
                        state = 2;
                        break;
                    }
                }
            }
            else if(state == 1)
            {
                if(rhs._activity[i][j] < end_date)
                    new_size++;
                else
                {
                    state = 2;
                    break;
                }
            }
        }

        _monthly_activity_frequency[i] = new_size;

        if(new_size == 0)
        {
            _activity[i] = nullptr;
            continue;
        }

        _activity[i] = new Transaction [new_size];

        k = 0;
        for( j = 0 ; j < accSize ; j++ )
            if(rhs._activity[i][j] > start_date && rhs._activity[i][j] < end_date)
                _activity[i][k++] = rhs._activity[i][j];

    }
}

Account::Account(Account&& rhs)
{
    _id = rhs._id;
    _monthly_activity_frequency = rhs._monthly_activity_frequency;
    _activity = rhs._activity;

    rhs._id = -1;
    rhs._monthly_activity_frequency = nullptr;
    rhs._activity = nullptr;
}

Account& Account::operator=(Account&& rhs)
{
    if(&rhs == this)
        return *this;

    _id = rhs._id;
    rhs._id = -1;

    delete [] _monthly_activity_frequency;
    _monthly_activity_frequency = rhs._monthly_activity_frequency;
    rhs._monthly_activity_frequency = nullptr;

    if(_activity)
        for( int i = 0 ; i < 12 ; i++ )
            delete [] _activity[i];
    delete [] _activity;
    _activity = rhs._activity;
    rhs._activity = nullptr;

    return *this;
}

Account& Account::operator=(const Account& rhs)
{
    if(&rhs == this)
        return *this;

    int accSize;

    _id = rhs._id;

    if(_monthly_activity_frequency == nullptr)
        _monthly_activity_frequency = new int [12];

    if(_activity == nullptr)
        _activity = new Transaction* [12]();

    for( int i = 0 ; i < 12 ; i++ )
    {
        delete [] _activity[i];
        accSize = rhs._monthly_activity_frequency[i];
        _monthly_activity_frequency[i] = accSize;

        if(accSize)
            _activity[i] = new Transaction [accSize];

        for( int j = 0 ; j < accSize ; j++ )
            _activity[i][j] = rhs._activity[i][j];
    }
    return *this;
}

bool Account::operator==(const Account& rhs) const
{
    return _id == rhs._id;
}

bool Account::operator==(int id) const
{
    return _id == id;
}

Account& Account::operator+=(const Account& rhs)
{
    if(rhs._activity == nullptr || rhs._monthly_activity_frequency == nullptr)
        return *this;

    if(_activity == nullptr || _monthly_activity_frequency == nullptr)
    {
        *this = rhs;
        return *this;
    }

    int i, iterator1, iterator2, size1, size2, new_size;

    Transaction *new_activity;

    for( i = 0 ; i < 12 ; i++ )
    {
        size1 = _monthly_activity_frequency[i];
        size2 = rhs._monthly_activity_frequency[i];

        if(size1 + size2 == 0)
        {
            delete [] _activity[i];
            _activity[i] = nullptr;
            continue;
        }
        iterator1 = 0;
        iterator2 = 0;

        new_activity = new Transaction [size1+size2];
        new_size = 0;

        while(iterator1 < size1 && iterator2 < size2)
        {
            if(_activity[i][iterator1] < rhs._activity[i][iterator2])
                new_activity[new_size++] = _activity[i][iterator1++];

            else if(_activity[i][iterator1] > rhs._activity[i][iterator2])
                new_activity[new_size++] = rhs._activity[i][iterator2++];

            else
            {
                new_activity[new_size++] = _activity[i][iterator1++];
                new_activity[new_size++] = rhs._activity[i][iterator2++];
            }
        }
        while(iterator1 < size1)
            new_activity[new_size++] = _activity[i][iterator1++];

        while(iterator2 < size2)
            new_activity[new_size++] = rhs._activity[i][iterator2++];

        _monthly_activity_frequency[i] = new_size;

        delete [] _activity[i];
        _activity[i] = new_activity;
        new_activity = nullptr;
    }
    return *this;
}

double Account::balance()
{
    if(_monthly_activity_frequency == nullptr)
        return 0;

    int accSize;
    double sum = 0;

    for( int i = 0 ; i < 12 ; i++ )
    {
        accSize = _monthly_activity_frequency[i];
        for( int j = 0 ; j < accSize ; j++ )
            sum = _activity[i][j] + sum;
    }
    return sum;
}

double Account::balance(time_t end_date)
{
    if(_monthly_activity_frequency == nullptr)
        return 0;

    int accSize, j;
    double sum = 0;

    for( int i = 0 ; i < 12 ; i++ )
    {
        accSize = _monthly_activity_frequency[i];
        for( j = 0 ; j < accSize ; j++ )
            if(_activity[i][j] < end_date)
                sum = _activity[i][j] + sum;
            else
                break;
        if(j != accSize)
            break;
    }
    return sum;
}

double Account::balance(time_t start_date, time_t end_date)
{
    if(_monthly_activity_frequency == nullptr)
        return 0;

    int accSize, j;
    double sum = 0;

    for( int i = 0 ; i < 12 ; i++ )
    {
        accSize = _monthly_activity_frequency[i];
        for( j = 0 ; j < accSize ; j++ )
            if(_activity[i][j] > start_date)
            {
                if(_activity[i][j] < end_date)
                    sum = _activity[i][j] + sum;
                else
                    break;
            }
        if(j != accSize)
            break;
    }
    return sum;
}

std::ostream& operator<<(std::ostream& os, const Account& account)
{
    if(account._activity == nullptr || account._monthly_activity_frequency == nullptr)
    {
        os << -1 << '\n';
        return os;
    }

    os << account._id << '\n';
    for( int i = 0 ; i < 12 ; i++ )
        for( int j = 0 ; j < account._monthly_activity_frequency[i] ; j++ )
            os << account._activity[i][j];

    return os;
}
