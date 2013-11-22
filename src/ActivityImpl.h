#ifndef __ACTIVITY_IMPL_H__
#define __ACTIVITY_IMPL_H__

#include "Activity.h"

Fwk::Ptr<Activity::Manager> activityManagerInstance();

namespace ActivityImpl {

class ActivityImpl : public Activity {
	protected:
    ActivityImpl(const string& name, Ptr<class ManagerImpl> manager)
				: Activity(name), status_(free), nextTime_(0.0), notifiee_(NULL), manager_(manager) {}
  	
  	Fwk::Ptr<class ManagerImpl> manager() const { return manager_; }

		virtual Status status() const { return status_; }
		virtual void statusIs(Status s) {
	    status_ = s;
	    if (notifiee_ != NULL) {
				notifiee_->onStatus();
	    }
		}

		virtual Time nextTime() const { return nextTime_; }
		virtual void nextTimeIs(Time t) {
	    nextTime_ = t;
	    if (notifiee_ != NULL) {
	    notifiee_->onNextTime();
	    }
		}

		virtual Notifiee::Ptr notifiee() const { return notifiee_; }

		virtual void lastNotifieeIs(Notifiee* n) {
	    ActivityImpl* me = const_cast<ActivityImpl*>(this);
	    me->notifiee_ = n;
		}
	
  private:
  	friend class ManagerImpl;
  	Status status_;
		Time nextTime_;
		Notifiee* notifiee_;
    Fwk::Ptr<class ManagerImpl> manager_;

};

class ManagerImpl : public Activity::Manager {
	public:
		typedef Ptr<ManagerImpl> Ptr;
		
		static Fwk::Ptr<Activity::Manager> activityManagerInstance();
		
		virtual ActivityTime activityTime() { return activityTime_; }  
		virtual void activityTimeIs(ActivityTime _activityTime){ activityTime_ = _activityTime; }
		
		virtual void engineIs(Fwk::Ptr<class Engine> _engine) { engine_ = _engine; }
		
		virtual Activity::Ptr activityNew(const string& name);
		virtual Activity::Ptr activity(const string& name) const;
		virtual void activityDel(const string& name);
		
		virtual Time now() const { return now_; }
		virtual void nowIs(Time time);
		
		virtual void lastActivityIs(Activity::Ptr activity);
		
	protected:
		ManagerImpl() : now_(0.0) {
			activityTime_ = virtualTime();
		}
		
		//Data members
		priority_queue<Activity::Ptr, vector<Activity::Ptr>, ActivityComp> scheduledActivities_;
		map<string, Activity::Ptr> activities_; //pool of all activities
		Time now_;
		ActivityTime activityTime_;
		Fwk::Ptr<Engine> engine_;
		
		//singleton instance
		static Fwk::Ptr<Activity::Manager> activityInstance_;	

};

}

#endif /* __ACTIVITY_IMPL_H__ */

