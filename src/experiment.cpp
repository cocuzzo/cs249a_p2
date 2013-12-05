#include <cstdlib>
#include <string>
#include <vector>
#include "Activity.h"
#include "Instance.h"

using namespace std;

int main(int argc, char* argv[]) {

    srand (time(NULL));

    bool randomizeShipmentSize = false; // defaults to non-random
    string shipmentRate = "24"; // 1 shipment per hour
    string shipmentSize = "100"; // 100 packages per shipment

    if ( argc > 1 && argc != 3) {
        cout << "bad arguments; usage:" << endl;
        cout << "experiment <rate> <size> (<size> may be 'random')" << endl;
        exit(0);
    } else if (argc == 3) {
        shipmentRate = string(argv[1]);
        shipmentSize = string(argv[2]);
        randomizeShipmentSize = (shipmentSize == "random");
    } // else use defaults
  
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // -- Simulation Entities
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Ptr<Instance::Manager> instanceManager = shippingInstanceManager();

    // -- Create Stats & Fleet
    Ptr<Instance> stats = instanceManager->instanceNew("myStats", "Stats");
    Ptr<Instance> fleet = instanceManager->instanceNew("myFleet", "Fleet");

    fleet->attributeIs("Truck, capacity", "50");
    fleet->attributeIs("Truck, cost", "5");
    fleet->attributeIs("Truck, speed", "60");

    cout << "fleet->attribute('Truck, capacity'): " << fleet->attribute("Truck, capacity") << endl;
    cout << "fleet->attribute('Truck, cost'): " << fleet->attribute("Truck, cost") << endl;
    cout << "fleet->attribute('Truck, speed'): " << fleet->attribute("Truck, speed") << endl;

    // --- Create Instances

    // -- Locations
    vector<Ptr<Instance>> sources;
    vector<Ptr<Instance>> sourceTerms;
    // Destination Terminal
    Ptr<Instance> destTerm = instanceManager->instanceNew("td", "Truck terminal");
    // Destination
    Ptr<Instance> dest = instanceManager->instanceNew("dest", "Customer");

    // Segments
    vector<Ptr<Instance>> sourceOSegs;
    vector<Ptr<Instance>> sourceISegs;
    vector<Ptr<Instance>> termOSegs;
    vector<Ptr<Instance>> termISegs;
    Ptr<Instance> destISeg = instanceManager->instanceNew("iSegd", "Truck segment");
    Ptr<Instance> destOSeg = instanceManager->instanceNew("oSegd", "Truck segment");

    // connect destination to dest terminal and configure dest segments
    destISeg->attributeIs("source", "td");
    destISeg->attributeIs("length", "100");
    destISeg->attributeIs("difficulty", "1");
    destOSeg->attributeIs("source", "dest");
    destOSeg->attributeIs("length", "100");
    destOSeg->attributeIs("difficulty", "1");
    destOSeg->attributeIs("return segment", "iSegd");

    for (int i=0; i<10; i++) {
        // create a source terminal
        string termName = "t" + to_string(i);
        sourceTerms.push_back(instanceManager->instanceNew(termName, "Truck terminal"));

        // create the terminal-terminal segments
        string oTermSegName = "oSeg" + to_string(i);
        string iTermSegName = "iSeg" + to_string(i);
        Ptr<Instance> oTermSeg = instanceManager->instanceNew(oTermSegName, "Truck segment");
        Ptr<Instance> iTermSeg = instanceManager->instanceNew(iTermSegName, "Truck segment");
        oTermSeg->attributeIs("source", termName);
        oTermSeg->attributeIs("length", "100");
        oTermSeg->attributeIs("difficulty", "1");
        iTermSeg->attributeIs("source", "td");
        iTermSeg->attributeIs("length", "100");
        iTermSeg->attributeIs("difficulty", "1");
        iTermSeg->attributeIs("return segment", oTermSegName);
        termOSegs.push_back(oTermSeg);
        termISegs.push_back(iTermSeg);

        // create 10 sources at a time
        for (int j=0; j<10; j++) {
            string sourceName = "s" + to_string(i) + to_string(j);
            Ptr<Instance> source = instanceManager->instanceNew(sourceName, "Customer");
            // -- Define Shipment Schedule
            source->attributeIs("destination", "dest");
            source->attributeIs("transfer rate", shipmentRate);
            if (randomizeShipmentSize) {
                int randomSize = rand() % 1000 + 1;
                source->attributeIs("shipment size", to_string(randomSize));
            } else {
                source->attributeIs("shipment size", shipmentSize);
            }
            sources.push_back(source);
            // connect the source to the associated source terminal
            string oSegName = "oSeg" + to_string(i) + to_string(j);
            string iSegName = "iSeg" + to_string(i) + to_string(j);
            Ptr<Instance> oSeg = instanceManager->instanceNew(oSegName, "Truck segment");
            Ptr<Instance> iSeg = instanceManager->instanceNew(iSegName, "Truck segment");
            oSeg->attributeIs("source", sourceName);
            oSeg->attributeIs("length", "100");
            oSeg->attributeIs("difficulty", "1");
            iSeg->attributeIs("source", termName);
            iSeg->attributeIs("length", "100");
            iSeg->attributeIs("difficulty", "1");
            iSeg->attributeIs("return segment", oSegName);
            sourceOSegs.push_back(oSeg);
            sourceISegs.push_back(iSeg);
        }
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // -- Time-stepped Simulation  
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~  
    Activity::Manager::Ptr activityManager = activityManagerInstance();
    //set the time to 20.0
    activityManager->nowIs(1000.0);

    // Stats Reporting



    
    return 0;
}