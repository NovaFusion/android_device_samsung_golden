/*
 * Copyright (C) 2014 Marcin Chojnacki marcinch7@gmail.com
 * Copyright (C) 2014 NovaFusion https://github.com/NovaFusion
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef TURBAFIFO_H
#define TURBAFIFO_H

#include <pthread.h>
#include <semaphore.h>

class turba_locker
{
public:
    turba_locker();
    virtual ~turba_locker();
    void lock();
    void unlock();

private:
    sem_t semaphore;
    bool locked;
};

template<class T>
struct turba_node
{
    turba_node *next;
    T value;
};

template<class T>
class turba_fifo
{
public:
    turba_fifo() {
        front=NULL;
        back=NULL;
        pthread_mutex_init(&lock,NULL);
    }
    
    virtual ~turba_fifo() {
        pthread_mutex_destroy(&lock);
    }
    
    void push(T value) {
        pthread_mutex_lock(&lock);
        turba_node<T> *temp=new turba_node<T>();
        temp->value=value;
        if (front==NULL) {
            front=temp;
            back=temp;
        }
        else {
            back->next=temp;
            back=temp;
        }
        locker.unlock();
        pthread_mutex_unlock(&lock);
    }
    
    T pop() {
        if(front==NULL) locker.lock();
        pthread_mutex_lock(&lock);
        T ret=front->value;
        front=front->next;
        pthread_mutex_unlock(&lock);
        return ret;
    }

    bool empty() {
        return front==NULL;
    }
 
private:
    turba_node<T> *front;
    turba_node<T> *back;
    turba_locker locker;
    pthread_mutex_t lock;
};

#endif