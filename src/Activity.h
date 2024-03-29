#ifndef __ACTIVITY_H__
#define __ACTIVITY_H__

#include <string>

#include "PtrInterface.h"
#include "Ptr.h"

#include "Nominal.h"
#include "Notifiee.h"
#include "ShippingTypes.h"

using std::string;

class Activity : public Fwk::PtrInterface<Activity> {
 public:
    typedef Fwk::Ptr<Activity> Ptr;
    
	/* Notifiee class for Activities */
	class Notifiee : public Fwk::BaseNotifiee<Activity> {
    public:
			typedef Fwk::Ptr<Notifiee> Ptr;

      Notifiee(Activity* act) : Fwk::BaseNotifiee<Activity>(act) {}

      virtual void onNextTime() {}
			virtual void onStatus() {}
    };

    class Manager;

    enum Status {
        free, waiting, ready, executing, nextTimeScheduled, deleted
    };

    virtual Status status() const = 0;
    virtual void statusIs(Status s)  = 0;
    
    virtual Shipping::Time nextTime() const = 0;
    virtual void nextTimeIs(Shipping::Time t) = 0;

    virtual Fwk::Ptr<Notifiee> notifiee() const = 0;

    virtual void lastNotifieeIs(Notifiee* n) = 0;

    virtual string name() const { return name_; }

	protected:
    Activity(const string &name): name_(name) {}

	private:
    string name_;

};

class Activity::Manager : public Fwk::PtrInterface<Activity::Manager> {
	public:
		typedef Fwk::Ptr<Activity::Manager> Ptr;
		
		enum ActivityTime {
			virtualTime_ = 0,
			realTime_ = 1
		};
		static inline ActivityTime virtualTime() { return virtualTime_; }
		static inline ActivityTime realTime() { return realTime_; }
		
		virtual ActivityTime activityTime() = 0;
		virtual void activityTimeIs(ActivityTime _activityTime) = 0;
		
		virtual Fwk::Ptr<Activity> activityNew(const string &name) = 0;
		
		virtual Fwk::Ptr<Activity> activity(const string &name) const = 0;
		
		virtual void activityDel(const string &name) = 0;
		
		virtual void lastActivityIs(Activity::Ptr) = 0;
		
		virtual Shipping::Time now() const = 0;
		virtual void nowIs(Shipping::Time) = 0;

	private:
    /* Up to you */

};

extern Fwk::Ptr<Activity::Manager> activityManagerInstance();

#endif
