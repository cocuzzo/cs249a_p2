#include <iostream>
#include <time.h>

#include "ActivityImpl.h"

using namespace std;

Fwk::Ptr<Activity::Manager> activityManagerInstance() {
    return ActivityImpl::ManagerImpl::activityManagerInstance();
}

namespace ActivityImpl {

//Definition of static member
Fwk::Ptr<Activity::Manager> ManagerImpl::activityInstance_ = NULL;
    
//Gets the singleton instance of ManagerImpl
Fwk::Ptr<Activity::Manager> ManagerImpl::activityManagerInstance() {
	if (activityInstance_ == NULL) {
	    activityInstance_ = new ManagerImpl();
	}
	return activityInstance_;
}
    
    
Activity::Ptr ManagerImpl::activityNew(const string& name) {
	Activity::Ptr activity = activities_[name];
	if (activity != NULL) {
	    cerr << "Activity already exists!" << endl;
	    return NULL;
	}
	
	activity = new ActivityImpl(name, this);
	activities_[name] = activity;

	return activity;
}

Activity::Ptr ManagerImpl::activity(const string& name) const {
	map<string, Activity::Ptr>::const_iterator it = activities_.find(name);
	if(it != activities_.end() ) {
	    return (*it).second;
	}
	// dont throw an exception (accessor)
	return NULL; 
}
    
void ManagerImpl::activityDel(const string& name) {
	activities_.erase(name);
}
    
void ManagerImpl::lastActivityIs(Activity::Ptr activity) {
	scheduledActivities_.push(activity);
}

void ManagerImpl::nowIs(Shipping::Time t) {
	//find the most recent activites to run and run them in order
	while (!scheduledActivities_.empty()) {
	    
		//figure out the next activity to run
		Activity::Ptr nextToRun = scheduledActivities_.top();

		//if the next time is greater than the specified time, break
		//the loop
		if (nextToRun->nextTime() > t) {
			break;
		}
		
		if(activityTime_ == realTime()){
			//calculate amount of time to sleep
			Shipping::Time diff = Shipping::Time(nextToRun->nextTime().value() - now_.value());
			//sleep 100ms (100,000 microseconds) for every unit of time
			usleep(( (diff.value()) * 1000000.0)); //PEO
		}
		
		now_ = nextToRun->nextTime();

		//run the minimum time activity and remove it from the queue
		scheduledActivities_.pop();

		nextToRun->statusIs(Activity::executing);
		
	}
	//synchronize the time
	now_ = t;
}



} //end namespace ActivityImpl
