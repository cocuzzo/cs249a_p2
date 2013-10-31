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
class FleetRep;
class ConnRep;

class ManagerImpl : public Instance::Manager {
public:
	ManagerImpl();

	// Manager method
	Ptr<Instance> instanceNew(const string& name, const string& type);

	// Manager method
	Ptr<Instance> instance(const string& name);

  // Manager method
  void instanceDel(const string& name);
  
  Engine::Ptr engine(){ return engine_; }


private:
  map<string,Ptr<Instance> > instance_;
  
  Engine::Ptr engine_;
  
  Ptr<StatsRep> statsRep_;
  Ptr<FleetRep> fleetRep_;
  Ptr<ConnRep> connRep_;
  
};

class LocationRep : public Instance {
public:
	//typedef Fwk::Ptr<LocationRep const> PtrConst;
	//typedef Fwk::Ptr<LocationRep> Ptr;

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
	//typedef Fwk::Ptr<CustomerRep const> PtrConst;
	//typedef Fwk::Ptr<CustomerRep> Ptr;
	
	CustomerRep(const string& name, ManagerImpl *manager):
		LocationRep(name, manager){ }
};

class PortRep : public LocationRep {
public:
	//typedef Fwk::Ptr<PortRep const> PtrConst;
	//typedef Fwk::Ptr<PortRep> Ptr;
	
	PortRep(const string& name, ManagerImpl *manager) :
		LocationRep(name, manager){ }
};

                                                                                               
class TruckTerminalRep : public LocationRep {
public:
	//typedef Fwk::Ptr<TruckTerminalRep const> PtrConst;
	//typedef Fwk::Ptr<TruckTerminalRep> Ptr;
	
  TruckTerminalRep(const string& name, ManagerImpl *manager) :
  	LocationRep(name, manager)
  {
  	// Nothing else to do.
  }

};

class BoatTerminalRep : public LocationRep {
public:
	//typedef Fwk::Ptr<BoatTerminalRep const> PtrConst;
	//typedef Fwk::Ptr<BoatTerminalRep> Ptr;
	
  BoatTerminalRep(const string& name, ManagerImpl *manager) :
  	LocationRep(name, manager)
  {
  	// Nothing else to do.
  }

};

class PlaneTerminalRep : public LocationRep {
public:
	//typedef Fwk::Ptr<PlaneTerminalRep const> PtrConst;
	//typedef Fwk::Ptr<PlaneTerminalRep> Ptr;
	
  PlaneTerminalRep(const string& name, ManagerImpl *manager) :
  	LocationRep(name, manager)
  {
  	// Nothing else to do.
  }

};

class SegmentRep : public Instance {
public:
	//typedef Fwk::Ptr<SegmentRep const> PtrConst;
	//typedef Fwk::Ptr<SegmentRep> Ptr;
	
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
	//typedef Fwk::Ptr<TruckSegmentRep const> PtrConst;
	//typedef Fwk::Ptr<TruckSegmentRep> Ptr;
	
  TruckSegmentRep(const string& name, ManagerImpl *manager) :
  	SegmentRep(name, manager)
  {
  	// Nothing else to do.
  }

};

class BoatSegmentRep : public SegmentRep {
public:
	//typedef Fwk::Ptr<BoatSegmentRep const> PtrConst;
	//typedef Fwk::Ptr<BoatSegmentRep> Ptr;
	
  BoatSegmentRep(const string& name, ManagerImpl *manager) :
  	SegmentRep(name, manager)
  {
  	// Nothing else to do.
  }

};

class PlaneSegmentRep : public SegmentRep {
public:
	//typedef Fwk::Ptr<PlaneSegmentRep const> PtrConst;
	//typedef Fwk::Ptr<PlaneSegmentRep> Ptr;
	
  PlaneSegmentRep(const string& name, ManagerImpl *manager) :
  	SegmentRep(name, manager)
  {
  	// Nothing else to do.
  }

};

class StatsRep : public Instance {
public:
	//typedef Fwk::Ptr<StatsRep const> PtrConst;
	//typedef Fwk::Ptr<StatsRep> Ptr;
	
	StatsRep(const string& name, ManagerImpl* manager) :
		Instance(name), manager_(manager) {
		stats_ = Stats::StatsNew(manager_->engine().ptr());
	}
	string attribute(const string& name);
	void attributeIs(const string& name, const string& v);
	
private:
	Ptr<ManagerImpl> manager_;
	Stats::Ptr stats_;
};

class ConnRep : public Instance {
public:
	//typedef Fwk::Ptr<ConnRep const> PtrConst;
	//typedef Fwk::Ptr<ConnRep> Ptr;
	
	ConnRep(const string& name, ManagerImpl* manager) :
		Instance(name), manager_(manager) { 
	}
	string attribute(const string& name);
	void attributeIs(const string& name, const string& v);
	
private:
	Ptr<ManagerImpl> manager_;
	
};
class FleetRep : public Instance {
public:
	//typedef Fwk::Ptr<FleetRep const> PtrConst;
	//typedef Fwk::Ptr<FleetRep> Ptr;
	
	FleetRep(const string& name, ManagerImpl* manager) :
		Instance(name), manager_(manager) { 
	}
	
	string attribute(const string& name);
	void attributeIs(const string& name, const string& v);
	
private:
	Ptr<ManagerImpl> manager_;
	
};


/**********************************************************************************
** Rep Layer Class Implementation 
**********************************************************************************/

/* Manager */

ManagerImpl::ManagerImpl() { 
	engine_ = Engine::EngineNew();
	statsRep_ = new StatsRep("", this);
	fleetRep_ = new FleetRep("", this);
	connRep_ = new ConnRep("", this);
}

Ptr<Instance> ManagerImpl::instanceNew(const string& name, const string& type) {
	/*
	if(instance_.find(name) == instance_.end()){
		cerr << "Error: Already have instance named " << name << endl;
	}
	
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
  	if( "" == statsName_ ){
  		Ptr<StatsRep> t = new StatsRep(name, this);
  		instance_[name] = t;
  		statsName_ = name;
  	}
  	else {
  		cerr << "Error: Only 1 allowed stats instance, returning stats instance named " 
  				 << statsName_ << endl;
  	}
    return instance_[statsName_];
  }
  else if (type == "Conn") {
  	if( "" == connName_ ){
    	Ptr<ConnRep> t = new ConnRep(name, this);
    	instance_[name] = t;
    	connName_ = name;
 	 	}
 	 	else {
 	 		cerr << "Error: Only 1 allowed conn instance, returning conn instance named " 
  				 << connName_ << endl;
  	}
 		return instance_[connName_];
 	}
  else if (type == "Fleet") {
    if( "" == fleetName_ ){
    	Ptr<FleetRep> t = new FleetRep(name, this);
    	instance_[name] = t;
    	fleetName_ = name;
 	 	}
 	 	else {
 	 		cerr << "Error: Only 1 allowed fleet instance, returning fleet instance named " 
  				 << fleetName_ << endl;
  	}
 		return instance_[fleetName_];
  }
  else{
  	cerr << "Error: Invalid instance type " << type << endl;
  	return NULL;
  }
  */
  return NULL;
}

Ptr<Instance> ManagerImpl::instance(const string& name) {
  map<string,Ptr<Instance> >::const_iterator t = instance_.find(name);
  return t == instance_.end() ? NULL : (*t).second;
}

void ManagerImpl::instanceDel(const string& name) {
	/*
	if(instance_.find(name) == instance_.end()){
		cerr << "Error: No instance named " << name << endl; 
  	return;
	}
	instance_.erase(name);
	if(statsName_ == name) statsName_ = "";
	else if(connName_ == name) connName_ = "";
	else if(fleetName_ == name) fleetName_ = "";
	*/
}

/* LocationRep Impl */

string LocationRep::attribute(const string& name) {
	//int i = segmentNumber(name);
	
	return "";
}


void LocationRep::attributeIs(const string& name, const string& v) {
   //nothing to do, has no writable attributes
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

/* SegmentRep Impl */

string SegmentRep::attribute(const string& name){

	return "";
}


void SegmentRep::attributeIs(const string& name, const string& v){



}

/* StatsRep Impl */

string StatsRep::attribute(const string& name){

	return "";
}

void StatsRep::attributeIs(const string& name, const string& v){


}

/* FleetRep Impl */

string FleetRep::attribute(const string& name){

	return "";
}

void FleetRep::attributeIs(const string& name, const string& v){

	
}

/* ConnRep Impl */

string ConnRep::attribute(const string& name){

	return "";
}

void ConnRep::attributeIs(const string& name, const string& v){


}


} //end shipping namespace

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
