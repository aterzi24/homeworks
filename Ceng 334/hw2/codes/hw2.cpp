#include <iostream>
#include <vector>
#include <fstream>
#include <pthread.h>
#include <unistd.h>

#include "monitor.h"

using namespace std;

struct PersonInfo
{
    int id;
    int weight;
    int initialFloor;
    int destinationFloor;
    int priority;
    char direction;
};

struct ElevatorInfo
{
    int tt, it, iot;
    ElevatorInfo(int tt, int it, int iot):
        tt(tt), it(it), iot(iot)
    {}
};

class ElevatorMonitor:public Monitor
{
    Condition elevatorIdle;
    Condition outsideElevator;
    Condition insideElevator;
    Condition elevatorWaitForInsiders;
    vector<PersonInfo> *outsideQueue;
    vector<int> destinationQ;
    vector<int> list;
    int *insideQueue;
    int currentFloor;
    int numOfPeople;
    int totalWeight;
    int weightCapacity;
    int personCapacity;
    int numOfFloor;
    int leftCount;
    int numOfThread;
    char stateOfElevator;
    bool became_idle;

    void printElevatorInfo()
    {
        if(stateOfElevator == 'i')
        {
            printf("Elevator (Idle, %d, %d, %d ->)\n", totalWeight, numOfPeople, currentFloor);
            return ;
        }

        string str;
        if(stateOfElevator == 'u')
            str = "Moving-up";
        else
            str = "Moving-down";

        cout<<"Elevator ("<<str;
        printf(", %d, %d, %d -> ", totalWeight, numOfPeople, currentFloor);

        int dSize = destinationQ.size();
        for( int i = 0 ; i < dSize-1 ; i++ )
            cout<<destinationQ[i]<<',';

        cout<<destinationQ[dSize-1]<<")\n";
    }

    void addFloorToQ(int floor)
    {
        if(stateOfElevator == 'u')
        {
            for( int i = 0 ; i < destinationQ.size() ; i++ )
            {
                if(destinationQ[i] == floor)
                    return ;
                else if(floor < destinationQ[i])
                {
                    destinationQ.insert(destinationQ.begin() + i, floor);
                    return ;
                }
            }
            destinationQ.push_back(floor);
            return ;
        }
        // stateOfElevator == 'd'
        for( int i = 0 ; i < destinationQ.size() ; i++ )
        {
            if(destinationQ[i] == floor)
                return ;
            else if(floor > destinationQ[i])
            {
                destinationQ.insert(destinationQ.begin() + i, floor);
                return ;
            }
        }
        destinationQ.push_back(floor);
    }

    void addFloorToInside(int destinationFloor)
    {
        insideQueue[destinationFloor]++;
    }

    void addFloorToOutside(PersonInfo *info)
    {
        int initialFloor = info->initialFloor;
        outsideQueue[initialFloor].push_back(*info);
    }

public:
    ElevatorMonitor():elevatorIdle(this), outsideElevator(this), insideElevator(this),
    elevatorWaitForInsiders(this)
    {
        currentFloor = numOfPeople = totalWeight = numOfFloor = 0;
        weightCapacity = personCapacity = leftCount = 0;
        numOfThread = 0;
        stateOfElevator = 'i';
        became_idle = true;
        insideQueue = nullptr;
        outsideQueue = nullptr;
    }

    ~ElevatorMonitor()
    {
        delete [] insideQueue;
        delete [] outsideQueue;
    }

    void assignAll(int nd, int np, int wc, int pc)
    {
        __synchronized__;

        weightCapacity = wc;
        personCapacity = pc;
        numOfFloor = nd;
        numOfThread = np;

        insideQueue = new int [nd]();   //initialize with 0
        outsideQueue = new vector<PersonInfo> [nd];
    }

    int makeRequest(PersonInfo *info)
    {
        __synchronized__;

        int id = info->id;
        int weight = info->weight;
        int initialFloor = info->initialFloor;
        int destinationFloor = info->destinationFloor;
        int priority = info->priority;
        char direction = info->direction;

        if(currentFloor == initialFloor)
        {
            if(stateOfElevator == 'i')
            {
                cout<<"Person ("<<id<<", "<<(priority==1?"hp":"lp")<<", "<<initialFloor<<" -> "<<destinationFloor<<", "<<weight<<") made a request\n";
                printElevatorInfo();

                numOfPeople++;
                totalWeight += weight;
                stateOfElevator = direction;
                destinationQ.push_back(destinationFloor);
                addFloorToInside(destinationFloor);

                cout<<"Person ("<<id<<", "<<(priority==1?"hp":"lp")<<", "<<initialFloor<<" -> "<<destinationFloor<<", "<<weight<<") entered the elevator\n";
                printElevatorInfo();

                return 2;
            }
            if(direction != stateOfElevator)
                return 0;

            cout<<"Person ("<<id<<", "<<(priority==1?"hp":"lp")<<", "<<initialFloor<<" -> "<<destinationFloor<<", "<<weight<<") made a request\n";
            printElevatorInfo();

            if(weight + totalWeight <= weightCapacity && numOfPeople < personCapacity)
            {
                numOfPeople++;
                totalWeight += weight;
                addFloorToQ(destinationFloor);
                addFloorToInside(destinationFloor);

                cout<<"Person ("<<id<<", "<<(priority==1?"hp":"lp")<<", "<<initialFloor<<" -> "<<destinationFloor<<", "<<weight<<") entered the elevator\n";
                printElevatorInfo();

                return 2;
            }
            else
                return 0;

        }
        if(stateOfElevator == 'i')
        {
            cout<<"Person ("<<id<<", "<<(priority==1?"hp":"lp")<<", "<<initialFloor<<" -> "<<destinationFloor<<", "<<weight<<") made a request\n";
            stateOfElevator = currentFloor>initialFloor?'d':'u';
            destinationQ.push_back(initialFloor);
            addFloorToOutside(info);
            printElevatorInfo();

            return 1;
        }
        if(direction != stateOfElevator)
            return 0;

        char comparison = currentFloor>initialFloor?'u':'d';
        if(comparison == stateOfElevator)
            return 0;

        cout<<"Person ("<<id<<", "<<(priority==1?"hp":"lp")<<", "<<initialFloor<<" -> "<<destinationFloor<<", "<<weight<<") made a request\n";
        addFloorToQ(initialFloor);
        addFloorToOutside(info);
        printElevatorInfo();

        return 1;
    }

    void personWait()
    {
        __synchronized__;

        elevatorIdle.wait();
    }

    bool personOutside()
    {
        __synchronized__;

        outsideElevator.wait();
        return became_idle;
    }

    bool sameFloor(int floor)
    {
        __synchronized__;

        return floor == currentFloor;
    }

    bool canEnter(PersonInfo *info)
    {
        __synchronized__;

        int id = info->id;
        int weight = info->weight;
        int initialFloor = info->initialFloor;
        int destinationFloor = info->destinationFloor;
        int priority = info->priority;

        int size = list.size();
        for( int i = 0 ; i < size ; i++ )
            if(list[i] == id)
            {
                numOfPeople++;
                totalWeight += weight;
                addFloorToQ(destinationFloor);
                addFloorToInside(destinationFloor);

                cout<<"Person ("<<id<<", "<<(priority==1?"hp":"lp")<<", "<<initialFloor<<" -> "<<destinationFloor<<", "<<weight<<") entered the elevator\n";
                printElevatorInfo();

                return true;
            }
        return false;
    }

    void personInside()
    {
        __synchronized__;

        insideElevator.wait();
    }

    void personLeft(PersonInfo *info)
    {
        __synchronized__;

        int id = info->id;
        int weight = info->weight;
        int initialFloor = info->initialFloor;
        int destinationFloor = info->destinationFloor;
        int priority = info->priority;

        int size = insideQueue[currentFloor];
        numOfPeople--;
        numOfThread--;
        leftCount++;
        totalWeight -= weight;

        cout<<"Person ("<<id<<", "<<(priority==1?"hp":"lp")<<", "<<initialFloor<<" -> "<<destinationFloor<<", "<<weight<<") has left the elevator\n";
        printElevatorInfo();

        if(leftCount == size)
        {
            leftCount = 0;
            elevatorWaitForInsiders.notify();
        }
    }

    bool is_idle()
    {
        __synchronized__;

        return stateOfElevator == 'i';
    }

    bool move()
    {
        __synchronized__;

        if(stateOfElevator == 'u')
            currentFloor++;
        else if(stateOfElevator == 'd')
            currentFloor--;

        became_idle = false;

        if(currentFloor != destinationQ[0])
        {
            printElevatorInfo();
            return false;
        }

        destinationQ.erase(destinationQ.begin());
        if(destinationQ.empty())
        {
            stateOfElevator = 'i';
            became_idle = true;
            outsideQueue[currentFloor].clear();
        }
        printElevatorInfo();
        if(insideQueue[currentFloor])
        {
            insideElevator.notifyAll();
            elevatorWaitForInsiders.wait();
            insideQueue[currentFloor] = 0;
        }

        if(became_idle)
            return false;

        if(!outsideQueue[currentFloor].empty())
        {
            if(numOfPeople == personCapacity)
            {
                outsideQueue[currentFloor].clear();
                return true;
            }
            int expNum = numOfPeople;
            int expWeight = totalWeight;

            for( int i = 0 ; i < outsideQueue[currentFloor].size() ; i++ )
                if(outsideQueue[currentFloor][i].priority == 1)
                    if(expWeight+outsideQueue[currentFloor][i].weight <= weightCapacity)
                    {
                        if(stateOfElevator != outsideQueue[currentFloor][i].direction)
                            continue;
                        list.push_back(outsideQueue[currentFloor][i].id);
                        expNum++;
                        expWeight += outsideQueue[currentFloor][i].weight;
                        if(expNum == personCapacity)
                        {
                            outsideQueue[currentFloor].clear();
                            return true;
                        }
                    }

            for( int i = 0 ; i < outsideQueue[currentFloor].size() ; i++ )
                if(outsideQueue[currentFloor][i].priority == 2)
                    if(expWeight+outsideQueue[currentFloor][i].weight <= weightCapacity)
                    {
                        if(stateOfElevator != outsideQueue[currentFloor][i].direction)
                            continue;
                        list.push_back(outsideQueue[currentFloor][i].id);
                        expNum++;
                        expWeight += outsideQueue[currentFloor][i].weight;
                        if(expNum == personCapacity)
                            break;
                    }

            outsideQueue[currentFloor].clear();
            return true;
        }
        return false;
    }

    void wakeupOutsiders()
    {
        __synchronized__;

        outsideElevator.notifyAll();
    }

    void cleanList()
    {
        __synchronized__;

        list.clear();
    }

    bool finished()
    {
        __synchronized__;

        return !numOfThread;
    }

    void wakeupWaiters()
    {
        __synchronized__;

        elevatorIdle.notifyAll();
    }

};

ElevatorMonitor emobj;

void *elevatorController(void *p)
{
    ElevatorInfo *elevator = (ElevatorInfo *) p;
    int tt = elevator->tt;
    int it = elevator->it;
    int iot = elevator->iot;

    bool result;

    while(1)
    {
        while(emobj.is_idle())
            usleep(it);

        while(1)
        {
            usleep(tt);
            result = emobj.move();
            if(emobj.is_idle())
                break;
            if(result)//only outsiders
            {
                emobj.wakeupOutsiders();
                usleep(iot);
                emobj.cleanList();
            }
        }

        if(emobj.finished())
            return 0;

        emobj.wakeupOutsiders();
        emobj.wakeupWaiters();
    }
}

void *personThread(void *p)
{
    PersonInfo *info = (PersonInfo *) p;
    int initialFloor = info->initialFloor;
    int destinationFloor = info->destinationFloor;
    info->direction = destinationFloor>initialFloor?'u':'d';

    int r;
    bool result, became_idle;

    do
    {
        a1:
        while( !(r = emobj.makeRequest(info)) )
            emobj.personWait();

        if(r == 2)
            break;

        do{
            became_idle = emobj.personOutside();
            if(became_idle)
                goto a1;
// it is better for outsiders to reach makeRequest before waiters so I needed to do that. I would not use goto if it was not necessary.
        }while(!emobj.sameFloor(initialFloor));

        result = emobj.canEnter(info);

        if(!result)
            emobj.personWait();

    }while(!result);

    do{
        emobj.personInside();
    }while(!emobj.sameFloor(destinationFloor));

    emobj.personLeft(info);
    return 0;
}

int main(int argc, char *argv[])
{
    ifstream inFile;
    int nd, np, wc, pc, tt, it, iot;

    inFile.open(argv[1]);
    inFile>>nd>>np>>wc>>pc>>tt>>it>>iot;

    ElevatorInfo elevator(tt, it, iot);

    emobj.assignAll(nd, np, wc, pc);

    PersonInfo *persons = new PersonInfo[np];
    pthread_t *tids = new pthread_t[np+1];

    for( int i = 0 ; i < np ; i++ )
    {
        inFile>>persons[i].weight>>persons[i].initialFloor>>persons[i].destinationFloor>>persons[i].priority;
        persons[i].id = i;
    }

    pthread_create(tids, NULL, elevatorController, (void *)&elevator);

    for( int i = 0 ; i < np ; i++ )
        pthread_create(tids+(i+1), NULL, personThread, (void *)(persons+i));

    for( int i = 0 ; i < np + 1 ; i++ )
        pthread_join(tids[i], NULL);

    delete [] tids;
    delete [] persons;
    inFile.close();
    return 0;
}
