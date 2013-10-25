#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
#include "Instance.h"
#include "Engine.h"

namespace Shipping {

using namespace std;

/**********************************************************************************
** Rep Layer Class Definitions 
**********************************************************************************/
class StatsRep;
class ConnRep;
class FleetRep;

class ManagerImpl : public Instance::Manager {
public:
	ManagerImpl();

	// Manager method
	Ptr<Instance> instanceNew(const string& name, const string& type);

	// Manager method
	Ptr<Instance> instance(const string& name);

  // Manager method
  void instanceDel(const string& name);

private:
  map<string,Ptr<Instance> > instance_;
  
  Ptr<StatsRep> statsRep;
  Ptr<ConnRep> connRep;
  Ptr<FleetRep> fleetRep;
};

class LocationRep : public Instance {
public:

	LocationRep(const string& name, ManagerImpl* manager) :
		Instance(name), manager_(manager)
	{
		// Nothing else to do.
	}

	// Instance method
	string attribute(const string& name);

  // Instance method
  void attributeIs(const string& name, const string& v);

private:
  Ptr<ManagerImpl> manager_;

	int segmentNumber(const string& name);

};

class CustomerRep : public LocationRep {
public:
	CustomerRep(const string& name, ManagerImpl *manager):
		LocationRep(name, manager){ }
};

class PortRep : public LocationRep {
public:
	PortRep(const string& name, ManagerImpl *manager) :
		LocationRep(name, manager){ }
};

                                                                                               
class TruckTerminalRep : public LocationRep {
public:

  TruckTerminalRep(const string& name, ManagerImpl *manager) :
  	LocationRep(name, manager)
  {
  	// Nothing else to do.
  }

};

class BoatTerminalRep : public LocationRep {
public:

  BoatTerminalRep(const string& name, ManagerImpl *manager) :
  	LocationRep(name, manager)
  {
  	// Nothing else to do.
  }

};

class PlaneTerminalRep : public LocationRep {
public:

  PlaneTerminalRep(const string& name, ManagerImpl *manager) :
  	LocationRep(name, manager)
  {
  	// Nothing else to do.
  }

};

class SegmentRep : public Instance {
public:

	SegmentRep(const string& name, ManagerImpl* manager) :
		Instance(name), manager_(manager)
	{
		// Nothing else to do.
	}

	// Instance method
	string attribute(const string& name);

  // Instance method
  void attributeIs(const string& name, const string& v);

private:
  Ptr<ManagerImpl> manager_;


};

class TruckSegmentRep : public SegmentRep {
public:

  TruckSegmentRep(const string& name, ManagerImpl *manager) :
  	SegmentRep(name, manager)
  {
  	// Nothing else to do.
  }

};

class BoatSegmentRep : public SegmentRep {
public:

  BoatSegmentRep(const string& name, ManagerImpl *manager) :
  	SegmentRep(name, manager)
  {
  	// Nothing else to do.
  }

};

class PlaneSegmentRep : public SegmentRep {
public:

  PlaneSegmentRep(const string& name, ManagerImpl *manager) :
  	SegmentRep(name, manager)
  {
  	// Nothing else to do.
  }

};

class StatsRep : public Instance {
public:

	StatsRep(const string& name, ManagerImpl* manager);
	string attribute(const string& name);
	void attributeIs(const string& name, const string& v);
	
private:
	
};
class ConnRep : public Instance {
public:

	ConnRep(const string& name, ManagerImpl* manager);
	string attribute(const string& name);
	void attributeIs(const string& name, const string& v);
	
private:
	
};
class FleetRep : public Instance {
public:

	FleetRep(const string& name, ManagerImpl* manager);
	string attribute(const string& name);
	void attributeIs(const string& name, const string& v);
	
private:
	
};


/**********************************************************************************
** Rep Layer Class Implementation 
**********************************************************************************/

ManagerImpl::ManagerImpl() { }

Ptr<Instance> ManagerImpl::instanceNew(const string& name, const string& type) {
	if (type == "Customer") {
  	Ptr<CustomerRep> t = new CustomerRep(name, this);
    instance_[name] = t;
    return t;
  }
	else if (type == "Port") {
  	Ptr<PortRep> t = new PortRep(name, this);
    instance_[name] = t;
    return t;
  }
  else if (type == "Truck terminal") {
  	Ptr<TruckTerminalRep> t = new TruckTerminalRep(name, this);
    instance_[name] = t;
    return t;
  }
  else if (type == "Boat terminal") {
  	Ptr<BoatTerminalRep> t = new BoatTerminalRep(name, this);
    instance_[name] = t;
    return t;
  }
  else if (type == "Plane terminal") {
  	Ptr<PlaneTerminalRep> t = new PlaneTerminalRep(name, this);
    instance_[name] = t;
    return t;
  }
  else if (type == "Truck segment") {
  	Ptr<TruckTerminalRep> t = new TruckTerminalRep(name, this);
    instance_[name] = t;
    return t;
  }
  else if (type == "Boat segment") {
  	Ptr<BoatTerminalRep> t = new BoatTerminalRep(name, this);
    instance_[name] = t;
    return t;
  }
  else if (type == "Plane segment") {
  	Ptr<PlaneTerminalRep> t = new PlaneTerminalRep(name, this);
    instance_[name] = t;
    return t;
  }
  else if (type == "Stats") {
  	instance_[name] = statsRep;
    return statsRep;
  }
  else if (type == "Conn") {
    instance_[name] = connRep;
    return connRep;
  }
  else if (type == "Fleet") {
    instance_[name] = fleetRep;
    return fleetRep;
  }
  else{
  	return NULL;
  }
}

Ptr<Instance> ManagerImpl::instance(const string& name) {
  map<string,Ptr<Instance> >::const_iterator t = instance_.find(name);

  return t == instance_.end() ? NULL : (*t).second;
}

void ManagerImpl::instanceDel(const string& name) {

}


string LocationRep::attribute(const string& name) {
	int i = segmentNumber(name);
	if (i != 0) {
  	cout << "Tried to read interface " << i;
  }
	return "";
}


void LocationRep::attributeIs(const string& name, const string& v) {
   //nothing to do
}

static const string segmentStr = "segment";
static const int segmentStrlen = segmentStr.length();

int LocationRep::segmentNumber(const string& name) {
	if (name.substr(0, segmentStrlen) == segmentStr) {
      const char* t = name.c_str() + segmentStrlen;
      return atoi(t);
  }
  return 0;
}


}

/*
 * This is the entry point for your library.
 * The client program will call this function to get a handle
 * on the Instance::Manager object, and from there will use
 * that object to interact with the middle layer (which will
 * in turn interact with the engine layer).
 */
Ptr<Instance::Manager> shippingInstanceManager() {
  return new Shipping::ManagerImpl();
}
