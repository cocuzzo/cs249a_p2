#ifndef __ACTIVITY_IMPL_H__
#define __ACTIVITY_IMPL_H__

#include <queue>
#include <map>

#include "Activity.h"
#include "ShippingTypes.h"

Fwk::Ptr<Activity::Manager> activityManagerInstance();

namespace ActivityImpl {

class ActivityComp : public binary_function<Activity::Ptr, Activity::Ptr, bool> {
	public:
  	ActivityComp() {}
		bool operator()(Activity::Ptr a, Activity::Ptr b) const {
				return (a->nextTime() > b->nextTime());
		}
};

class ActivityImpl : public Activity {
	protected:
    ActivityImpl(const string& name, Fwk::Ptr<class ManagerImpl> manager)
				: Activity(name), status_(free), notifiee_(NULL), manager_(manager) {}
  	
  	Fwk::Ptr<class ManagerImpl> manager() const { return manager_; }

		virtual Status status() const { return status_; }
		virtual void statusIs(Status s) {
	    status_ = s;
	    if (notifiee_ != NULL) {
				notifiee_->onStatus();
	    }
		}

		virtual Shipping::Time nextTime() const { return nextTime_; }
		virtual void nextTimeIs(Shipping::Time t) {
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
		Shipping::Time nextTime_;
		Notifiee* notifiee_;
    Fwk::Ptr<class ManagerImpl> manager_;

};

class ManagerImpl : public Activity::Manager {
	public:
		typedef Fwk::Ptr<ManagerImpl> Ptr;
		
		static Fwk::Ptr<Activity::Manager> activityManagerInstance();
		
		virtual ActivityTime activityTime() { return activityTime_; }  
		virtual void activityTimeIs(ActivityTime _activityTime){ activityTime_ = _activityTime; }
		
		virtual Activity::Ptr activityNew(const string& name);
		virtual Activity::Ptr activity(const string& name) const;
		virtual void activityDel(const string& name);
		
		virtual Shipping::Time now() const { return now_; }
		virtual void nowIs(Shipping::Time time);
		
		virtual void lastActivityIs(Activity::Ptr activity);
		
	protected:
		ManagerImpl() : now_(0.0) {
			activityTime_ = virtualTime();
		}
		
		//Data members
		std::priority_queue<Activity::Ptr, vector<Activity::Ptr>, ActivityComp> scheduledActivities_;
		std::map<string, Activity::Ptr> activities_; //pool of all activities
		Shipping::Time now_;
		ActivityTime activityTime_;
		
		//singleton instance
		static Fwk::Ptr<Activity::Manager> activityInstance_;	

};

}

#endif /* __ACTIVITY_IMPL_H__ */

