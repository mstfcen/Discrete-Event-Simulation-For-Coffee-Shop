#include "FileReader.cpp"
#include <iostream>
#include <string>
#include <vector>
#include "Queue.cpp"
#include "Staff.cpp"
#include <set>
#include <bits/stdc++.h>
#include <fstream>

using namespace std;

void initializeStaff(vector<Staff*>&,vector<Staff*>&,vector<Staff*>& ,int);
void updateTimeSequence(multiset<float> &timeSeq,vector<Customer*> customers,string initializeAttribute,int size);
Staff* getAvailableStaffBeforeTime(float time,vector<Staff*> &staffvect,int vectSize);
bool compareOrderReadyTimes(Customer* c1,Customer* c2);
bool compareArrivalTimes(Customer* c1, Customer* c2);
template<typename T> void freeMemory(vector<T> &vect);
void printCustomerUtilizations(fstream &file,string unitType,vector<Customer*> &vect);
void printStaffUtilizations(fstream &file,vector<Staff*> &vect,float runTime);
void printOutput(string,vector<Customer*>&,vector<Staff*>&,vector<Staff*>&,vector<Staff*>&);

int main(int argc, char const *argv[])
{    
    vector<string> inputs = FileReader::readFile(argv[1]);
    //Reading inputs from argv[1] text file

    vector<Customer*> customers;
    vector<Staff*> cashierVect, baristaVectModel1,baristaVectModel2;
    vector<PriorityQueue*> baristaQueues;
    Queue custQueueCashiers(10);
    multiset<float> timeSequence;
    float arrivalTime,orderTime,brewTime,coffeePrice;
    //Creation of variables,vectors etc...
    int cashierCount = stoi(inputs[0]);
    int customerCount = stoi(inputs[1]);

    
    for (int i = 0; i < cashierCount/3+1; i++)
    {
        baristaQueues.push_back(new PriorityQueue(customerCount));
    }
    //This vector holds customer
    
   
    for(int i =2; i<inputs.size();i++)
    {
        arrivalTime = stof(inputs[i]);
        orderTime = stof(inputs[++i]);
        brewTime = stof(inputs[++i]);
        coffeePrice = stof(inputs[++i]);
        customers.push_back(new Customer(arrivalTime,orderTime,brewTime,coffeePrice));
    }
    
    initializeStaff(cashierVect,baristaVectModel1,baristaVectModel2,cashierCount);
    //Initializes vectors of staffs for furthermore use.
  
    updateTimeSequence(timeSequence,customers,"arrival",customerCount);
    //Clears timeSequence set and inserts each customers arrivalTime

    int counter = 0;
    for(float time:timeSequence)
    {
        Staff* emptyStaff = getAvailableStaffBeforeTime(time,cashierVect,cashierCount);
        //Returns NULL if no staff is available  // Returns Staff object if there is available staff
        Customer * cust;
        if(counter<customerCount&&time==customers[counter]->arrivalTime)
        {
            cust= customers[counter++];
        } //If its customers arrival time then iterate to that customer

        if (time==cust->arrivalTime)
        { 
            if (emptyStaff!=NULL)
            //If there is an available staff -> take order
            {
                emptyStaff->takeOrder(cust,time);   //Take customers order
                timeSequence.insert(emptyStaff->emptyTime); //Insert staff's next emptytime to timeSequence
            }
            else 
            // emptyStaff==NULL ->This means there is no available staff -> enqueue cust in queue
            {
                custQueueCashiers.enqueue(cust);    //Enqueue customer
            }   
        }
        else if(!custQueueCashiers.isEmpty())
        //If there are customers in queue and time!=cust->arrivalTime this means this time is
        //staffs empty time so there is an emptystaff -> dequeue cust and take order
        {
            cust = custQueueCashiers.dequeue(); //Dequeue customer
            emptyStaff->takeOrder(cust,time);   //Take customers order
            timeSequence.insert(emptyStaff->emptyTime); //Insert staff's next emptytime to timeSequence
        }       
    }//end of for

    
    sort(customers.begin(),customers.end(),compareOrderReadyTimes); //Sorts customers by their orderReadyTimes
    
    updateTimeSequence(timeSequence,customers,"order",customerCount);    //Clears timeSequence set and inserts each customers orderReadyTime


    counter = 0;
    for (float time:timeSequence)
    {
        Staff* emptyStaff = getAvailableStaffBeforeTime(time,baristaVectModel1,cashierCount/3);
        //Returns NULL if no staff is available  // Returns Staff object if there is available staff
        Customer * cust;
        if(counter<customerCount&&time==customers[counter]->orderReadyTime){
            cust= customers[counter++];
        }//If its customers arrival time then iterate to that customer
        if(time == cust->orderReadyTime)
        {
            if(emptyStaff!=NULL)
            //If there is an available staff -> take order
            {
                emptyStaff->brewCoffee(cust,time,1);    //Take customers order for model1
                timeSequence.insert(emptyStaff->emptyTime); //Insert staff's next emptytime to timeSequence
            }
            else//(f<=emptyStaff->emptyTime)
            {
                //enqueue
                baristaQueues[0]->enqueue(cust);    //Enqueue customer
            }
        }
        else if(!baristaQueues[0]->isEmpty())
        //If there are customers in queue and time!=cust->orderReadyTime this means this time is
        //staffs empty time so there is an emptystaff -> dequeue cust and take order
        {
            cust = baristaQueues[0]->dequeue(); //Dequeue custoemer
            emptyStaff->brewCoffee(cust,time,1);    //Take customers order for model1
            timeSequence.insert(emptyStaff->emptyTime); //Insert staff's next emptytime to timeSequence
        }
    }


    updateTimeSequence(timeSequence,customers,"order",customerCount);
    //Clears timeSequence set and inserts each customers orderReadyTime

    counter = 0;
    for (float time:timeSequence)
    {
        Staff* emptyStaff =  getAvailableStaffBeforeTime(time,baristaVectModel2,cashierCount/3);
        //Returns NULL if no staff is available  // Returns Staff object if there is available staff

        if(counter>=customerCount&&emptyStaff!=NULL&&baristaQueues[emptyStaff->id]->isEmpty())
        //If we took every customers order and there is an available staff and available staff has no customer waiting in queue
        {
            for(int i=0; i<baristaVectModel2.size();i++)
            {//Iterate through each baristas queue
                if(!baristaQueues[i+1]->isEmpty())
                {//If some of the baristas has customers in queue
                    emptyStaff = baristaVectModel2[i];  //Operate on that barista
                    break;
                }
            }
        }


        Customer * cust;
        if(counter<customerCount&&time==customers[counter]->orderReadyTime){
            cust= customers[counter++];
        }//If its customers arrival time then iterate to that customer
        if(time == cust->orderReadyTime)
        {
            emptyStaff = baristaVectModel2[cust->baristaIdModel2-1];    //Each customer can give their order to a specific barista. So I'm operating on that barista
            if(time>=emptyStaff->emptyTime)
            {
                //If there is an available staff -> take order
                emptyStaff->brewCoffee(cust,time,2);    //Take customers order for model2
                timeSequence.insert(emptyStaff->emptyTime); //Insert staff's next emptytime to timeSequence
                
            }
            else
            //If time==cust->orderReadyTime but time<emptyTime this means there is no available staff.
            {
                baristaQueues[cust->baristaIdModel2]->enqueue(cust);    //Enqueue customer
            }
        }
        else if(time==emptyStaff->emptyTime && !baristaQueues[emptyStaff->id]->isEmpty())
        {
            //if time==emptyTime and there are customers waiting in queue
            cust = baristaQueues[emptyStaff->id]->dequeue();    //Dequeue customer
            emptyStaff->brewCoffee(cust,time,2); //Take customers order for model2
            timeSequence.insert(emptyStaff->emptyTime);  ////Insert staff's next emptytime to timeSequence
        }
    }
    sort(customers.begin(),customers.end(),compareArrivalTimes);
    
    float runtimeForModel1 = customers[0]->coffeeReadyTimeModel1;
    float runtimeForModel2 = customers[0]->coffeeReadyTimeModel2;
    fstream myFile;
    myFile.open(argv[2],ios::out);
    for(int i=1;i<customers.size();i++)
    {
        if(runtimeForModel1<customers[i]->coffeeReadyTimeModel1)
        {
            runtimeForModel1 = customers[i]->coffeeReadyTimeModel1;
        }
        if(runtimeForModel2<customers[i]->coffeeReadyTimeModel2)
        {
            runtimeForModel2 = customers[i]->coffeeReadyTimeModel2;
        }
    }
    myFile<<"OUTPUT FOR MODEL 1\n"<<runtimeForModel1<<endl<<custQueueCashiers.getMaxSize()<<endl<<baristaQueues[0]->getMaxSize()<<endl;
    printStaffUtilizations(myFile,cashierVect,runtimeForModel1);
    printStaffUtilizations(myFile,baristaVectModel1,runtimeForModel1);
    printCustomerUtilizations(myFile,"customerModel1",customers);
    myFile<<endl<<"OUTPUT FOR MODEL 2\n"<<runtimeForModel2<<endl<<custQueueCashiers.getMaxSize()<<endl;
    for (int i = 1; i < baristaQueues.size(); i++)
    {
        myFile<<baristaQueues[i]->getMaxSize()<<endl;
    }
    printStaffUtilizations(myFile,cashierVect,runtimeForModel2);
    printStaffUtilizations(myFile,baristaVectModel2,runtimeForModel2);
    printCustomerUtilizations(myFile,"customerModel2",customers);
    myFile.close();    
    

    freeMemory(cashierVect);
    freeMemory(baristaQueues);
    freeMemory(baristaVectModel1);
    freeMemory(baristaVectModel2);
    freeMemory(customers);
    return 0;
}

void initializeStaff(vector<Staff*> &cashVect, vector<Staff*> &baristaVectModel1,vector<Staff*> &baristaVectModel2, int cashierSize)
{
    for(int i=0;i<cashierSize;i++)
    {
        cashVect.push_back(new Staff(i+1));
    }
    for(int i=0;i<int(cashierSize/3);i++)
    {
        baristaVectModel1.push_back(new Staff(i+1));
        baristaVectModel2.push_back(new Staff(i+1));
    }
}

Staff* getAvailableStaffBeforeTime(float time,vector<Staff*> &staffvect,int vectSize)
{
    Staff* tempStaff =NULL;
    for (int i = 0; i < vectSize; i++)
    {
        if(staffvect[i]->emptyTime<=time){
            return staffvect[i];
        }
    }
    return tempStaff;    
}

bool compareOrderReadyTimes(Customer *c1,Customer *c2)
{
    return c1->orderReadyTime<c2->orderReadyTime;
}
bool compareArrivalTimes(Customer* c1, Customer *c2)
{
    return c1->arrivalTime < c2->arrivalTime;
}

void updateTimeSequence(multiset<float> &timeSeq,vector<Customer*> customers,string initializeAttribute,int size)
{
    timeSeq.clear();
    if(initializeAttribute=="arrival")
    {
        for (int i = 0; i < size; i++)
        {
            timeSeq.insert(customers[i]->arrivalTime);
        }
        
    }
    else if(initializeAttribute=="order")
    {
        for (int i = 0; i < size; i++)
        {
            timeSeq.insert(customers[i]->orderReadyTime);
        }
    }
}
template<typename T>
void freeMemory(vector<T>& vect)
{
    for (int i = 0; i < vect.size(); i++)
    {
        delete vect[i];
    }
}

void printCustomerUtilizations(fstream &file,string unitType,vector<Customer*> &vect)
{
    if(unitType=="customerModel1")
    {
        for (int i = 0; i < vect.size(); i++)
        {   
            file<< vect[i]->coffeeReadyTimeModel1-vect[i]->arrivalTime<<endl;
        }
    }
    else if(unitType=="customerModel2")
    {
        for (int i = 0; i < vect.size(); i++)
        {
            file<< vect[i]->coffeeReadyTimeModel2-vect[i]->arrivalTime<<endl;
        }
    }
}
void printStaffUtilizations(fstream &file,vector<Staff*> &vect,float runTime)
{
for (int i = 0; i < vect.size(); i++)
        {
            file << setprecision(2) << fixed << vect[i]->runTime/runTime <<endl;
        }
}