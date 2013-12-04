#include "Activity.h"
#include "Instance.h"
#include "ShippingTypes.h"

int main(int argc, char* argv[]) {
 try{ 
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // -- Simulation Entities
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Ptr<Instance::Manager> instanceManager = shippingInstanceManager();

    // -- Create Stats & Fleet
    Ptr<Instance> stats = instanceManager->instanceNew("myStats", "Stats");
    Ptr<Instance> fleet = instanceManager->instanceNew("myFleet", "Fleet");

    fleet->attributeIs("Truck, capacity", "50");
    fleet->attributeIs("Truck, cost", "10");
    fleet->attributeIs("Truck, speed", "50");

    cout << "fleet->attribute('Boat, capacity'): " << fleet->attribute("Boat, capacity") << endl;
    cout << "fleet->attribute('Boat, cost'): " << fleet->attribute("Boat, cost") << endl;
    cout << "fleet->attribute('Boat, speed'): " << fleet->attribute("Boat, speed") << endl;
    cout << "fleet->attribute('Plane, capacity'): " << fleet->attribute("Plane, capacity") << endl;
    cout << "fleet->attribute('Plane, cost'): " << fleet->attribute("Plane, cost") << endl;
    cout << "fleet->attribute('Plane, speed'): " << fleet->attribute("Plane, speed") << endl;
    cout << "fleet->attribute('Truck, capacity'): " << fleet->attribute("Truck, capacity") << endl;
    cout << "fleet->attribute('Truck, cost'): " << fleet->attribute("Truck, cost") << endl;
    cout << "fleet->attribute('Truck, speed'): " << fleet->attribute("Truck, speed") << endl;

    /* Network Topology
          A   B
           \ /
            C
            |
            D

    All shipments go from A -> D and B -> D

    */


    // --- Create Instances
    // -- Locations
    Ptr<Instance> a = instanceManager->instanceNew("a", "Customer");  
    Ptr<Instance> b = instanceManager->instanceNew("b", "Customer");  
    Ptr<Instance> c = instanceManager->instanceNew("c", "Truck terminal");  
    Ptr<Instance> d = instanceManager->instanceNew("d", "Customer");  

    // -- Segments
    Ptr<Instance> seg_ac = instanceManager->instanceNew("seg_ac", "Truck segment");  
    Ptr<Instance> seg_ca = instanceManager->instanceNew("seg_ca", "Truck segment");  
    Ptr<Instance> seg_bc = instanceManager->instanceNew("seg_bc", "Truck segment");  
    Ptr<Instance> seg_cb = instanceManager->instanceNew("seg_cb", "Truck segment");  
    Ptr<Instance> seg_cd = instanceManager->instanceNew("seg_cd", "Truck segment");  
    Ptr<Instance> seg_dc = instanceManager->instanceNew("seg_dc", "Truck segment");  

    // Inter-location Connections
    seg_ac->attributeIs("source", "a");
    seg_ca->attributeIs("source", "c");
    seg_ac->attributeIs("return segment", "seg_ca");

    seg_bc->attributeIs("source", "b");
    seg_cb->attributeIs("source", "c");
    seg_bc->attributeIs("return segment", "seg_cb");

    seg_cd->attributeIs("source", "c");
    seg_dc->attributeIs("source", "d");
    seg_cd->attributeIs("return segment", "seg_dc");

    // -- Segment lengths
    seg_ac->attributeIs("length", "100");
    seg_ca->attributeIs("length", "200");
    seg_bc->attributeIs("length", "100");
    seg_cb->attributeIs("length", "200");
    seg_cd->attributeIs("length", "100");
    seg_dc->attributeIs("length", "200");

    // -- Segment difficulties
    seg_ac->attributeIs("difficulty", "1");
    seg_ca->attributeIs("difficulty", "1");
    seg_bc->attributeIs("difficulty", "1");
    seg_cb->attributeIs("difficulty", "1");
    seg_cd->attributeIs("difficulty", "1");
    seg_dc->attributeIs("difficulty", "1");

    // -- Shipment Schedules
    a->attributeIs("transfer rate", "120"); //PEO
    a->attributeIs("shipment size", "50");
    a->attributeIs("destination", "d");

    b->attributeIs("transfer rate", "2");
    b->attributeIs("shipment size", "50");
    b->attributeIs("destination", "d");

    d->attributeIs("transfer rate", "1");
    d->attributeIs("shipment size", "50");
    d->attributeIs("destination", "b");

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // -- Time-stepped Simulation  
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~  
    Activity::Manager::Ptr activityManager = activityManagerInstance();
    
    // advance the time by a full day
    activityManager->nowIs(23.0);

    // now clear all shipment schedules
    a->attributeIs("transfer rate", "0");
    b->attributeIs("transfer rate", "0");
    d->attributeIs("transfer rate", "0");

    // advance simulation by a full day
    activityManager->nowIs(48.0);

    cout << "~~ Simulation Statistics ~~" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "customer location a statistics:" << endl;
    cout << "shipments received: " << a->attribute("shipments received") << " [expected 0]" << endl;
    cout << "average latency:    " << a->attribute("average latency") << " [expected 0]" << endl;
    cout << "total cost:         " << a->attribute("total cost") << " [expected 0]" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "customer location b statistics:" << endl;
    cout << "shipments received: " << b->attribute("shipments received") << " [expected 1]" << endl;
    cout << "average latency:    " << b->attribute("average latency") << " [expected 8]" << endl;
    cout << "total cost:         " << b->attribute("total cost") << " [expected 4000]" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "customer location d statistics:" << endl;
    cout << "shipments received: " << d->attribute("shipments received") << " [expected 4]" << endl;
    cout << "average latency:    " << d->attribute("average latency") << " [expected 4]" << endl;
    cout << "total cost:         " << d->attribute("total cost") << " [expected 8000]" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "segment a -> c statistics:" << endl;
    cout << "shipments received: " << seg_ac->attribute("shipments received") << " [expected 2]" << endl;
    cout << "shipments refused:  " << seg_ac->attribute("shipments refused") << " [expected 0]" << endl;
    cout << "segment capacity:   " << seg_ac->attribute("capacity") << " [expected 10]" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "segment c -> a statistics:" << endl;
    cout << "shipments received: " << seg_ca->attribute("shipments received") << " [expected 0]" << endl;
    cout << "shipments refused:  " << seg_ca->attribute("shipments refused") << " [expected 0]" << endl;
    cout << "segment capacity:   " << seg_ca->attribute("capacity") << " [expected 10]" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "segment b -> c statistics:" << endl;
    cout << "shipments received: " << seg_bc->attribute("shipments received") << " [expected 2]" << endl;
    cout << "shipments refused:  " << seg_bc->attribute("shipments refused") << " [expected 0]" << endl;
    cout << "segment capacity:   " << seg_bc->attribute("capacity") << " [expected 10]" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "segment c -> b statistics:" << endl;
    cout << "shipments received: " << seg_cb->attribute("shipments received") << " [expected 1]" << endl;
    cout << "shipments refused:  " << seg_cb->attribute("shipments refused") << " [expected 0]" << endl;
    cout << "segment capacity:   " << seg_cb->attribute("capacity") << " [expected 10]" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "segment c -> d statistics:" << endl;
    cout << "shipments received: " << seg_cd->attribute("shipments received") << " [expected 4]" << endl;
    cout << "shipments refused:  " << seg_cd->attribute("shipments refused") << " [expected 0]" << endl;
    cout << "segment capacity:   " << seg_cd->attribute("capacity") << " [expected 10]" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "segment d -> c statistics:" << endl;
    cout << "shipments received: " << seg_dc->attribute("shipments received") << " [expected 1]" << endl;
    cout << "shipments refused:  " << seg_dc->attribute("shipments refused") << " [expected 0]" << endl;
    cout << "segment capacity:   " << seg_dc->attribute("capacity") << " [expected 10]" << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  }
  catch(Shipping::Exception& e){
  	cerr << e.what() << endl;
  }  
    return 0;
}