//CPSC 320 Assignment 4
//Dylan Lafontsee, Ahmad Naqshbandi, Don Parker

#include<iostream>
#include<random>
#include<cmath>
#include<limits>
#include<math.h>
using namespace std;

uniform_int_distribution<> r_dist(0,RAND_MAX);
mt19937 r_entry{};

// Seed the random number generator
void my_srandom(int s){
	
	r_entry.seed(s);
	
}

// Return a random integer between 0 and RAND_MAX
int my_random(){
	
	return r_dist(r_entry);
	
}

// Returns a Unifrom Random Variable in range [raneStart, rangeEnd]
double uniform(double rangeStart, double rangeEnd){
	
	return rangeStart+(my_random()/((double)RAND_MAX))*(abs(rangeStart-rangeEnd));
	
}

// Returns an exponential random variable with specified rate
double exponential(double rate){
	
	double data=0;
	while (data==0) data=uniform(0,1);

		
	return (-1)*log(data)/rate;
	
}

double gamma(int n, double lambda)
{
	double x = 1;
	double u;
	
	for(int i = 0; i < n; i++)
	{
		u = 0;
		while (u == 0)
		{
			u = uniform(0,1);
		}
		
		x = x * u;
	}

	x = -1/lambda * log(x);
	
	return x;	
		
}

// Outputs results of the simulation
void simulation(int seed, double rate){
	
	my_srandom(seed);										// Specifying seed for random-number-generator
	
	double t=0;												// Current time variable
	double T=9;												// Closing time
	double sumTimeInSyestem=0;								// Variable to keep track of Sum(d-a)
	int peopleInSystem=0;									// Current no. of people in System
	double timeTillNextArrival=exponential(rate);			// Difference between current time and time of next arrival
	double timeTillNextDeparture=INFINITY;					// Difference between current time and time of next departure
	double totalArrivals=0;									// Total no. of poeple who visited
	
	// Making sure first arrival is before closing
	while (timeTillNextArrival>=T) timeTillNextArrival=exponential(rate);
	
	double timeOfNextArrival=timeTillNextArrival;			// Time when next arrival will happen
	double timeOfNextDeparture=timeTillNextDeparture;		// Time when next departure will happen
	
	// Simulation Loop
	while (timeOfNextArrival<=T || peopleInSystem>0) {
		
		if (timeOfNextArrival<=T && (timeOfNextArrival<timeOfNextDeparture)) {
			
			peopleInSystem++;
			totalArrivals++;
			
			t=timeOfNextArrival;
			
			sumTimeInSyestem=sumTimeInSyestem-t;
			
			timeTillNextArrival=exponential(rate);
			timeOfNextArrival=t+timeTillNextArrival;
			
			// Checking to see if current customer went from 0 to 1; 
			// If it did, timeOfNextDeparture is no longer INFINITY
			// and needs to be changed here for proper comparison
			// of timeOfNextArrival and timeOfNextDeparture in the
			// next iteration.
			if (peopleInSystem==1) {
				
				timeTillNextDeparture=gamma(3, 40);
				timeOfNextDeparture=t+timeTillNextDeparture;
				
			}
			
		}
		
		else {
			
			peopleInSystem--;

			t=timeOfNextDeparture;
			
			sumTimeInSyestem=sumTimeInSyestem+t;
			
			if (peopleInSystem>0) timeTillNextDeparture=gamma(3, 40);
			else timeTillNextDeparture=INFINITY;
			
			timeOfNextDeparture=t+timeTillNextDeparture;
		
		}
				
	}
	
	double averageTimeInSystem=sumTimeInSyestem/totalArrivals;
	
	double overTime;												
	if (t>T) overTime=t-T;
	else overTime=0;
	
	cout<<"Simulation with seed = "<<seed<<" and rate = "<<rate<<":"<<endl;
	cout<<"Average Time in System: "<<averageTimeInSystem<<endl;
	cout<<"Total Customers: "<<totalArrivals<<endl;
	cout<<"Overtime: "<<overTime<<endl;
	cout<<endl;
	
}

int main(){
	
	simulation(2, 10);
	simulation(12, 10);
	simulation(1000, 10);
	
	simulation(2, 15);
	simulation(12, 15);
	simulation(1000, 15);
	
	return 0;
	
}
