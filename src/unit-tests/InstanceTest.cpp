#include "gtest/gtest.h"
#include "Instance.h"


TEST(InstanceTest, basicFunctionality) {
  Ptr<Instance::Manager> manager = shippingInstanceManager();
  ASSERT_TRUE( NULL != manager );

	Ptr<Instance> fleet = manager->instanceNew("myFleet", "Fleet");
	ASSERT_TRUE (NULL != fleet);
	Ptr<Instance> conn = manager->instanceNew("myConn", "Conn");
	ASSERT_TRUE (NULL != conn);
	Ptr<Instance> stats = manager->instanceNew("myStats", "Stats");
	ASSERT_TRUE (NULL != stats);
  Ptr<Instance> customer1 = manager->instanceNew("customer1", "Customer");  
	ASSERT_TRUE (NULL != customer1);  
  Ptr<Instance> port1 = manager->instanceNew("port1", "Port");
  ASSERT_TRUE (NULL != port1);
  Ptr<Instance> truckTerm1 = manager->instanceNew("truckTerm1", "Truck terminal");
  ASSERT_TRUE (NULL != truckTerm1);
  Ptr<Instance> boatTerm1 = manager->instanceNew("boatTerm1", "Boat terminal");
  ASSERT_TRUE (NULL != boatTerm1);
  Ptr<Instance> planeTerm1 = manager->instanceNew("planeTerm1", "Plane terminal");
  ASSERT_TRUE (NULL != planeTerm1);
  Ptr<Instance> truckSeg1 = manager->instanceNew("truckSeg1", "Truck segment");
  ASSERT_TRUE (NULL != truckSeg1);
  Ptr<Instance> boatSeg1 = manager->instanceNew("boatSeg1", "Boat segment");
  ASSERT_TRUE (NULL != boatSeg1);
  Ptr<Instance> planeSeg1 = manager->instanceNew("planeSeg1", "Plane segment");
  ASSERT_TRUE (NULL != planeSeg1);
  Ptr<Instance> boatSeg2 = manager->instanceNew("boatSeg2", "Boat segment"); 
  ASSERT_TRUE (NULL != boatSeg2); 
  Ptr<Instance> truckSeg2 = manager->instanceNew("truckSeg2", "Truck segment");
  ASSERT_TRUE (NULL != truckSeg2);
  Ptr<Instance> planeSeg2 = manager->instanceNew("planeSeg2", "Plane segment");
  ASSERT_TRUE (NULL != planeSeg2);
  Ptr<Instance> boatSeg3 = manager->instanceNew("boatSeg3", "Boat segment"); 
  ASSERT_TRUE (NULL != boatSeg3); 
  Ptr<Instance> truckSeg3 = manager->instanceNew("truckSeg3", "Truck segment");
  ASSERT_TRUE (NULL != truckSeg3);
  Ptr<Instance> planeSeg3 = manager->instanceNew("planeSeg3", "Plane segment");
  ASSERT_TRUE (NULL != planeSeg3);

  
  truckSeg1->attributeIs("source", "customer1");
  truckSeg2->attributeIs("source", "port1");
  truckSeg3->attributeIs("source", "truckTerm1");
  truckSeg1->attributeIs("return segment", "truckSeg2");
  boatSeg1->attributeIs("source", "customer1");
  boatSeg2->attributeIs("source", "port1");
  boatSeg3->attributeIs("source", "boatTerm1");
  boatSeg1->attributeIs("return segment", "boatSeg2");
  planeSeg1->attributeIs("source", "customer1");
  planeSeg2->attributeIs("source", "port1");
  planeSeg3->attributeIs("source", "planeTerm1");
  planeSeg1->attributeIs("return segment", "planeSeg2");
  
  truckSeg1->attributeIs("length", "100");
  truckSeg2->attributeIs("difficulty", "1");
  truckSeg3->attributeIs("expedite support", "yes");
  boatSeg1->attributeIs("length", "3456");
  boatSeg2->attributeIs("difficulty", "2.34");
  boatSeg3->attributeIs("expedite support", "yes");
  planeSeg1->attributeIs("length", "2");
  planeSeg2->attributeIs("difficulty", "5.0");
  planeSeg3->attributeIs("expedite support", "yes");
  
  EXPECT_EQ("100", truckSeg1->attribute("length"));
  EXPECT_EQ("1.00", truckSeg2->attribute("difficulty"));
  EXPECT_EQ("yes", truckSeg3->attribute("expedite support"));
  EXPECT_EQ("3456", boatSeg1->attribute("length"));
  EXPECT_EQ("2.34", boatSeg2->attribute("difficulty"));
  EXPECT_EQ("yes", boatSeg3->attribute("expedite support"));
  EXPECT_EQ("2", planeSeg1->attribute("length"));
  EXPECT_EQ("5.00", planeSeg2->attribute("difficulty"));
  EXPECT_EQ("yes", planeSeg3->attribute("expedite support"));
  
  EXPECT_EQ("customer1", truckSeg1->attribute("source"));
  EXPECT_EQ("port1", truckSeg2->attribute("source"));
  EXPECT_EQ("truckTerm1", truckSeg3->attribute("source"));
  EXPECT_EQ("truckSeg2", truckSeg1->attribute("return segment"));
  EXPECT_EQ("truckSeg1", truckSeg2->attribute("return segment"));
  EXPECT_EQ("customer1", boatSeg1->attribute("source"));
  EXPECT_EQ("port1", boatSeg2->attribute("source"));
  EXPECT_EQ("boatTerm1", boatSeg3->attribute("source"));
  EXPECT_EQ("boatSeg2", boatSeg1->attribute("return segment"));
  EXPECT_EQ("boatSeg1", boatSeg2->attribute("return segment"));
  EXPECT_EQ("customer1", planeSeg1->attribute("source"));
  EXPECT_EQ("port1", planeSeg2->attribute("source"));
  EXPECT_EQ("planeTerm1", planeSeg3->attribute("source"));
  EXPECT_EQ("planeSeg2", planeSeg1->attribute("return segment"));
  EXPECT_EQ("planeSeg1", planeSeg2->attribute("return segment"));

	EXPECT_EQ("truckSeg1", customer1->attribute("segment1"));
	EXPECT_EQ("truckSeg2", port1->attribute("segment1"));
	EXPECT_EQ("truckSeg3", truckTerm1->attribute("segment1"));
	EXPECT_EQ("boatSeg1", customer1->attribute("segment2"));
	EXPECT_EQ("boatSeg2", port1->attribute("segment2"));
	EXPECT_EQ("boatSeg3", boatTerm1->attribute("segment1"));
	EXPECT_EQ("planeSeg1", customer1->attribute("segment3"));
	EXPECT_EQ("planeSeg2", port1->attribute("segment3"));
	EXPECT_EQ("planeSeg3", planeTerm1->attribute("segment1"));
	
	
	
	fleet->attributeIs("Truck, speed", "60");
  fleet->attributeIs("Truck, capacity", "50");
  fleet->attributeIs("Truck, cost", "20.00");
	fleet->attributeIs("Boat, speed", "31.23");
  fleet->attributeIs("Boat, capacity", "23");
  fleet->attributeIs("Boat, cost", "10");
  fleet->attributeIs("Plane, speed", "99");
  fleet->attributeIs("Plane, capacity", "1000");
  fleet->attributeIs("Plane, cost", "400.00");
  
  EXPECT_EQ("60.00", fleet->attribute("Truck, speed"));
  EXPECT_EQ("50", fleet->attribute("Truck, capacity"));
  EXPECT_EQ("20.00", fleet->attribute("Truck, cost"));
  EXPECT_EQ("31.23", fleet->attribute("Boat, speed"));
  EXPECT_EQ("23", fleet->attribute("Boat, capacity"));
  EXPECT_EQ("10.00", fleet->attribute("Boat, cost"));
  EXPECT_EQ("99.00", fleet->attribute("Plane, speed"));
  EXPECT_EQ("1000", fleet->attribute("Plane, capacity"));
  EXPECT_EQ("400.00", fleet->attribute("Plane, cost"));

  
  EXPECT_EQ("1", stats->attribute("Customer"));
  EXPECT_EQ("1", stats->attribute("Port"));
  EXPECT_EQ("1", stats->attribute("Truck terminal"));
 	EXPECT_EQ("1", stats->attribute("Boat terminal"));
 	EXPECT_EQ("1", stats->attribute("Plane terminal"));
 	EXPECT_EQ("3", stats->attribute("Truck segment"));
 	EXPECT_EQ("3", stats->attribute("Boat segment"));
 	EXPECT_EQ("3", stats->attribute("Plane segment"));
 	EXPECT_EQ("33.33", stats->attribute("expedite percentage"));
 	
}

TEST(InstanceTest, fleetConnStatsCreation) {
  Ptr<Instance::Manager> manager = shippingInstanceManager();
  ASSERT_TRUE( NULL != manager );
  
  Ptr<Instance> fleet = manager->instanceNew("myFleet", "Fleet");
	ASSERT_TRUE (NULL != fleet);
  Ptr<Instance> conn = manager->instanceNew("myConn", "Conn");
	ASSERT_TRUE (NULL != conn);
	Ptr<Instance> stats = manager->instanceNew("myStats", "Stats");
	ASSERT_TRUE (NULL != stats);
	
	Ptr<Instance> fleetDup = manager->instanceNew("myFleetDup", "Fleet");
	ASSERT_TRUE (fleetDup == fleet);
	Ptr<Instance> connDup = manager->instanceNew("myConnDup", "Conn");
	ASSERT_TRUE (connDup == conn);
	Ptr<Instance> statsDup = manager->instanceNew("myStatsDup", "Stats");
	ASSERT_TRUE (statsDup == stats);
	
	ASSERT_TRUE (NULL == manager->instance("myFleetDup"));
	ASSERT_TRUE (NULL == manager->instance("myConnDup"));
	ASSERT_TRUE (NULL == manager->instance("myStatsDup"));
	ASSERT_TRUE (NULL != manager->instance("myFleet"));
	ASSERT_TRUE (NULL != manager->instance("myConn"));
	ASSERT_TRUE (NULL != manager->instance("myStats"));
  
}












