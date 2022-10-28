#include "Observer.h"

void Observable::registerObserver(Observer& obs)
{
    observers.push_back(&obs);
    printf("[OBSERVERABLE] Registering observer...\n");
}

void Observable::notifyObservers(EventType type, void* obj) 
{
    for (Observer* obs : observers) {
        obs->notify(type, obj);
    }
}