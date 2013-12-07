#include <cstdlib>
#include <string>
#include <vector>
#include "Activity.h"
#include "Instance.h"

using namespace std;

void printCustomerStats(Ptr<Instance> customer) {
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "CUSTOMER " << customer->name() << " statistics:" << endl;
    cout << "shipments received: " << customer->attribute("shipments received") << endl;
    cout << "average latency:    " << customer->attribute("average latency") << endl;
    cout << "total cost:         " << customer->attribute("total cost") << endl;
}

void printSegmentStats(Ptr<Instance> segment) {
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "SEGMENT " << segment->name() << " statistics:" << endl;
    cout << "shipments received: " << segment->attribute("shipments received") << endl;
    cout << "shipments refused:  " << segment->attribute("shipments refused") << endl;
    cout << "segment capacity:   " << segment->attribute("capacity") << endl;
}

int main(int argc, char* argv[]) {

    srand (time(NULL));

    bool randomizeShipmentSize = false; // defaults to non-random
    string shipmentRate = "24"; // 1 shipment per hour
    string shipmentSize = "100"; // 100 packages per shipment
    double simulationHours = 100.0;
    string termSegCapacity = "10";

    if ( argc > 1 && argc != 5) {
        cout << "bad arguments; usage:" << endl;
        cout << "experiment <rate> <size> <sim hours> (where <size> may be 'random')" << endl;
        exit(0);
    } else if (argc == 5) {
        shipmentRate = string(argv[1]);
        shipmentSize = string(argv[2]);
        randomizeShipmentSize = (shipmentSize == "random");
        simulationHours = ::atof(argv[3]);
        termSegCapacity = string(argv[4]);
    } // else use defaults
  
    try {
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // -- Simulation Entities
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        Ptr<Instance::Manager> instanceManager = shippingInstanceManager();

        // -- Create Stats & Fleet
        Ptr<Instance> stats = instanceManager->instanceNew("myStats", "Stats");
        Ptr<Instance> fleet = instanceManager->instanceNew("myFleet", "Fleet");

        fleet->attributeIs("Truck, capacity", "100");
        fleet->attributeIs("Truck, cost", "1");
        fleet->attributeIs("Truck, speed", "100");

        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << "FLEET PARAMETERS" << endl;
        cout << "truck capacity: " << fleet->attribute("Truck, capacity") << endl;
        cout << "truck cost:     " << fleet->attribute("Truck, cost") << endl;
        cout << "truck speed:    " << fleet->attribute("Truck, speed") << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << "SIMULATION PARAMETERS" << endl;
        cout << "shipment rate:       " << shipmentRate << endl;
        cout << "shipment size:       " << shipmentSize << endl;
        cout << "simulation hours:    " << simulationHours << endl;
        cout << "term. seg. capacity: " << termSegCapacity << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

        cout << "generating 'experiment' shipping network...";
        fflush(stdout);
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
        destISeg->attributeIs("capacity", "10");
        destOSeg->attributeIs("source", "dest");
        destOSeg->attributeIs("length", "100");
        destOSeg->attributeIs("difficulty", "1");
        destOSeg->attributeIs("capacity", "10");
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
            oTermSeg->attributeIs("capacity", termSegCapacity);
            iTermSeg->attributeIs("source", "td");
            iTermSeg->attributeIs("length", "100");
            iTermSeg->attributeIs("difficulty", "1");
            iTermSeg->attributeIs("capacity", termSegCapacity);
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
                    // shipmentSizes.push_back(randomSize);
                    source->attributeIs("shipment size", to_string(randomSize));
                } else {
                    // shipmentSizes.push_back(stoi(shipmentSize));
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
                oSeg->attributeIs("capacity", "10");
                iSeg->attributeIs("source", termName);
                iSeg->attributeIs("length", "100");
                iSeg->attributeIs("difficulty", "1");
                iSeg->attributeIs("capacity", "10");
                iSeg->attributeIs("return segment", oSegName);
                sourceOSegs.push_back(oSeg);
                sourceISegs.push_back(iSeg);
            }
        }
        cout << "done" << endl;

        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        // -- Time-stepped Simulation  
        // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~  
        
        cout << "creating activity manager...";
        fflush(stdout);

        // activity manager uses VIRTUAL TIME by default
        Activity::Manager::Ptr activityManager = activityManagerInstance();
        cout << "done" << endl;

        // uncomment the line below to use REAL TIME instead
        // activityManager->activityTimeIs(Activity::Manager::realTime());

        cout << "starting simulation...";
        fflush(stdout);
        // run the simulation for a while
        activityManager->nowIs(simulationHours);
        cout << "done" << endl;

        // collect some segment averages
        double avgRcvdSourceSegs = 0.0;
        double avgRefdSourceSegs = 0.0;

        double avgRcvdTermSegs = 0.0;
        double avgRefdTermSegs = 0.0;

        double rcvdDestSeg = 0.0;
        double refdDestSeg = 0.0;

        for (Ptr<Instance> sourceSeg : sourceOSegs) {
            int numRcvd = atoi(sourceSeg->attribute("shipments received").c_str());
            int numRefd = atoi(sourceSeg->attribute("shipments refused").c_str());
            avgRcvdSourceSegs += static_cast<double>(numRcvd);
            avgRefdSourceSegs += static_cast<double>(numRefd);
        }
        avgRcvdSourceSegs = avgRcvdSourceSegs / sourceOSegs.size();
        avgRefdSourceSegs = avgRefdSourceSegs / sourceOSegs.size();

        for (Ptr<Instance> termSeg : termOSegs) {
            int numRcvd = atoi(termSeg->attribute("shipments received").c_str());
            int numRefd = atoi(termSeg->attribute("shipments refused").c_str());
            avgRcvdTermSegs += static_cast<double>(numRcvd);
            avgRefdTermSegs += static_cast<double>(numRefd);
        }
        avgRcvdTermSegs = avgRcvdTermSegs / termOSegs.size();
        avgRefdTermSegs = avgRefdTermSegs / termOSegs.size();

        rcvdDestSeg = atof(destISeg->attribute("shipments received").c_str());
        refdDestSeg = atof(destISeg->attribute("shipments refused").c_str());

        // statistics reporting for individual customers & segments
        cout << "~~ Simulation Statistics ~~" << endl;
        printCustomerStats(sources.front());
        printSegmentStats(sourceOSegs.front());
        printSegmentStats(termOSegs.front());
        printSegmentStats(destISeg);
        printCustomerStats(dest);

        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << "~~ Aggregate Segment Statistics ~~" << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << "number of source segments:                        " << sourceOSegs.size() << endl;
        cout << "avg. shipments received across source segments:   " << avgRcvdSourceSegs << endl;
        cout << "avg. shipments refused across source segments:    " << avgRefdSourceSegs << endl;
        cout << "congestions ratio (refused / received):           " << avgRefdSourceSegs / avgRcvdSourceSegs << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << "number of terminal segments:                      " << termOSegs.size() << endl;
        cout << "avg. shipments received across terminal segments: " << avgRcvdTermSegs << endl;
        cout << "avg. shipments refused across terminal segments:  " << avgRefdTermSegs << endl;
        cout << "congestions ratio (refused / received):           " << avgRefdTermSegs / avgRcvdTermSegs << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << "shipments received across destination segment:    " << rcvdDestSeg << endl;
        cout << "shipments refused across destination segment:     " << refdDestSeg << endl;
        cout << "congestions ratio (refused / received):           " << refdDestSeg / rcvdDestSeg << endl;
        cout << "~~~~ End of Statistics ~~~~" << endl;
        
    }
    catch(Shipping::Exception& e){
       cerr << e.what() << endl;
    } 
    return 0;
}